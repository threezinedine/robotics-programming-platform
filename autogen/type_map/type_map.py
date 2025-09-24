from .type_map_const import maps


class TypeMap:
    """
    Mainly used to convert C++ types into Python types. The mapping can be extended by the user rather than
    fixing it in the code.
    """

    def __init__(self) -> None:
        self._typeMappings: list[tuple[list[str], str]] = []

    def AddMapping(self, cppRegex: str, pyType: str) -> None:
        """
        Append a new configuration setup for type mapping.

        Parameters
        ----------
            cppRegex (str)
                The C++ type regex to match.

            pyType (str)
                The corresponding Python type.
        """
        for typeMapping in self._typeMappings:
            if typeMapping[1] == pyType:
                typeMapping[0].append(cppRegex)
                return

        self._typeMappings.append(([cppRegex], pyType))

    def LoadMappings(self) -> None:
        """
        Load the current type mappings from the constant file. Do not use the AddMapping method after calling this
        method to avoid duplicates.
        """
        for mapping in maps:
            for cppRegex in mapping[0]:
                self.AddMapping(cppRegex, mapping[1])

    def Convert(self, cppType: str) -> str:
        """
        Processing the C++ type and convert it into the corresponding Python type.

        Parameters
        ----------
            cppType (str)
                The C++ type to convert.

        Returns
        -------
            str: The corresponding Python type.
        """
        cppType = cppType.replace("const ", "").replace("&", "").strip()

        for typeMapping in self._typeMappings:
            for cppRegex in typeMapping[0]:
                import re

                if re.fullmatch(cppRegex, cppType):
                    return typeMapping[1]

        return cppType
