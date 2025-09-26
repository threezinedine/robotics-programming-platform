import os
from Engine import ToString_ProjectDescription, ProjectDescription
from constants import PROJECT_FILE_NAME


class ProjectBuilder:
    """
    Assume that the project directory in test is alwayas cwd.
    """

    def __init__(self, projectName: str) -> None:
        self.projectName = projectName
        self.projectDir = os.path.join(os.getcwd(), projectName)
        self.projectFile = os.path.join(self.projectDir, PROJECT_FILE_NAME)

    def Build(self) -> None:
        """
        Only actually run build of all needed environment here. Other defined via methods
        will be delegated to the methods.
        """
        if not os.path.exists(self.projectDir):
            os.makedirs(self.projectDir)

        projectDesc = ProjectDescription()
        projectDesc.name = self.projectName

        with open(self.projectFile, "w") as f:
            f.write(ToString_ProjectDescription(projectDesc))
