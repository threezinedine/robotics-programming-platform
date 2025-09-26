import os
import json
from dacite import from_dict
from models import Application
from constants import APPLICATION_FILE_NAME, APPLICATION_FOLDER_NAME
from .assertion import Assert


class ApplicationAssert(Assert):
    def __init__(
        self,
        version: str = "1.0.0",
        recentProjects: list[str] | None = None,
    ) -> None:
        super().__init__()
        self.version = version
        self.recentProjects = recentProjects if recentProjects is not None else []

    def _Assert(self) -> None:
        appDataDir = os.getenv("APPDATA")
        assert appDataDir is not None, "APPDATA environment variable is not set."

        appFolderPath = os.path.join(appDataDir, APPLICATION_FOLDER_NAME)
        appFilePath = os.path.join(appFolderPath, APPLICATION_FILE_NAME)

        assert os.path.exists(appFolderPath), "Application folder does not exist."
        assert os.path.exists(appFilePath), "Application file does not exist."

        with open(appFilePath, "r") as f:
            appData = json.load(f)
            appModel = from_dict(data_class=Application, data=appData)
            assert (
                self.version
                == f"{appModel.version.major}.{appModel.version.minor}.{appModel.version.patch}"
            ), f"Expected version {self.version}, got {appModel.version}"

            assert len(appModel.recentProjects) == len(
                self.recentProjects
            ), f"Expected {len(self.recentProjects)} recent projects, got {len(appModel.recentProjects)}"

            assert set(appModel.recentProjects) == set(
                self.recentProjects
            ), f"Recent projects do not match. Expected {self.recentProjects}, got {appModel.recentProjects}"
