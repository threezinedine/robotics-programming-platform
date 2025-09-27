import os
from typing import TypeAlias
from utils.dependency_injection import Depend
from models import ProjectStateModel


File: TypeAlias = str
Folder: TypeAlias = dict[str, list["Folder"] | list[File]]
ProjectStructure: TypeAlias = list[Folder | File]


@Depend(ProjectStateModel)
class ProjectStructureWidgetViewModel:
    def __init__(self, projectState: ProjectStateModel) -> None:
        self.projectState = projectState
        self.projectStructure: ProjectStructure = []

    def ReloadProjectStructure(self) -> None:
        """
        Extract the project structure with the format:
        ```shell
        - test.py
        - root
          - folder1
            - file1.py
            - file2.py
          - folder2
            - file3.py
        ```
        and jsonify it to be used in the view.
        ```json
        [
            "test.py",
            {
                "root": [
                    {
                        "folder1": [
                            "file1.py",
                            "file2.py"
                        ]
                    },
                    {
                        "folder2": [
                            "file3.py"
                        ]
                    }
                ]
            }
        ]
        ```
        """
        if self.projectState.CurrentProject is None:
            return

        def _ExtractFolder(path: str) -> ProjectStructure:
            """
            The recursive function to extract the folder structure.
            """

            structure: ProjectStructure = []

            for _, dirs, files in os.walk(path):
                for file in files:
                    structure.append(file)

                for dir in dirs:
                    dirPath = os.path.join(path, dir)
                    structure.append({dir: _ExtractFolder(dirPath)})  # type: ignore

            return structure

        self.projectStructure = _ExtractFolder(self.projectState.projectDir)
