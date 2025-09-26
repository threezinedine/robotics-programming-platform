import os
import json
from models import Application, Version
from constants import APPLICATION_FILE_NAME, APPLICATION_FOLDER_NAME
from dataclasses import asdict


class ApplicationBuilder:
    def __init__(self) -> None:
        self.version = Version()

    def SetVersion(self, major: int, minor: int, patch: int) -> "ApplicationBuilder":
        """
        Set the version of the application to be created.
        """
        # Currently not implemented, as Application only has default version.
        self.version = Version(major, minor, patch)

        return self

    def Build(self) -> None:
        """
        Only actually run build of all needed application here. Other defined via methods
        will be delegated to the methods.
        """

        appDataDir = os.getenv("APPDATA")
        if appDataDir is None:
            raise EnvironmentError("APPDATA environment variable is not set.")

        appFolderPath = os.path.join(appDataDir, APPLICATION_FOLDER_NAME)
        appFilePath = os.path.join(appFolderPath, APPLICATION_FILE_NAME)

        os.makedirs(appFolderPath, exist_ok=True)

        with open(appFilePath, "w") as f:
            application = Application(version=self.version)
            f.write(json.dumps(asdict(application), indent=4))
