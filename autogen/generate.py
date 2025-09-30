import os
from jinja2 import Environment, FileSystemLoader
from parser import Parse, Structure
from parser.py_class import PyClass
from parser.py_function import PyFunction
from parser.py_object import PyObject
from type_map.type_map import TypeMap


def Generate(
    inputFiles: list[str],
    templateFile: str,
    testContent: str | None = None,
) -> str:
    """
    Main method of the whole project which analyzes the C/C++ source code and create the code based
    on the provided template.

    Parameters
    ----------
        inputFiles (List[str])
            Paths to the C/C++ source files to be analyzed (absolute paths).

        templateFile (str)
            Path to the template file used for code generation (absolute path).

        testContent (str | None)
            Optional string content for testing purposes.

    Returns
    -------
        str: The generated code as a string.
    """

    templateDir = os.path.dirname(templateFile)
    templateName = os.path.basename(templateFile)

    env = Environment(
        loader=FileSystemLoader(templateDir),
    )
    template = env.get_template(templateName)

    parser: Structure = {
        "enums": [],
        "structs": [],
        "functions": [],
        "classes": [],
    }

    for inputFile in inputFiles:
        Parse(inputFile, parser, testContent)

    typeMap = TypeMap()
    typeMap.LoadMappings()

    template.globals["convertCppTypeToPyType"] = typeMap.Convert

    def IsMethodStatic(parent: PyObject, method: PyObject) -> bool:
        assert isinstance(parent, PyClass)
        assert isinstance(method, PyFunction)
        return method.isStatic or "singleton" in parent.annotations

    def MethodParametersPyi(method: PyObject, isStatic: bool) -> str:
        assert isinstance(method, PyFunction)

        params: list[str] = []

        if not isStatic:
            params.append("self")

        for param in method.parameters:
            params.append(f"{param.name}: {typeMap.Convert(param.type)}")
        return ", ".join(params)

    def MethodParametersCpp(method: PyObject) -> str:
        assert isinstance(method, PyFunction)

        params: list[str] = []

        for param in method.parameters:
            if typeMap.Convert(param.type) == "str":
                params.append(f"const std::string& {param.name}")
            else:
                params.append(f"{param.type} {param.name}")
        return ", ".join(params)

    def MethodParametersCall(method: PyObject) -> str:
        assert isinstance(method, PyFunction)

        params: list[str] = []

        for param in method.parameters:
            if typeMap.Convert(param.type) == "str":
                params.append(f"{param.name}.c_str()")
            else:
                params.append(f"{param.name}")
        return ", ".join(params)

    def MethodParametersTypeList(method: PyObject) -> str:
        assert isinstance(method, PyFunction)

        params: list[str] = []

        for param in method.parameters:
            params.append(param.type)
        return ", ".join(params)

    def IsContainsJsonKeyAnnotation(obj: PyObject) -> str:
        keyNamePrefix = "key:"
        for annotation in obj.annotations:
            if annotation.startswith(keyNamePrefix):
                return annotation[len(keyNamePrefix) :]
        return ""

    def ObjectComment(obj: PyObject, default: str) -> str:
        return obj.comment if obj.comment else default

    allJsonMappedClasses: list[str] = []
    for struct in parser["structs"]:
        if "json" in struct.annotations:
            allJsonMappedClasses.append(struct.name)

    template.globals["isMethodStatic"] = IsMethodStatic
    template.globals["methodParametersPyi"] = MethodParametersPyi
    template.globals["methodParametersCpp"] = MethodParametersCpp
    template.globals["methodParametersCall"] = MethodParametersCall
    template.globals["objectComment"] = ObjectComment
    template.globals["isContainsJsonKeyAnnotation"] = IsContainsJsonKeyAnnotation
    template.globals["methodParametersTypeList"] = MethodParametersTypeList
    template.globals["allJsonMappedClasses"] = allJsonMappedClasses

    return template.render(**parser)
