import os
from constants import APPLICATION_FILE_NAME, APPLICATION_FOLDER_NAME
from .assertion import Assert


class ApplicationAssert(Assert):
    def __init__(self) -> None:
        super().__init__()

    def _Assert(self) -> None:
        appDataDir = os.getenv("APPDATA")
        assert appDataDir is not None, "APPDATA environment variable is not set."

        appFolderPath = os.path.join(appDataDir, APPLICATION_FOLDER_NAME)
        appFilePath = os.path.join(appFolderPath, APPLICATION_FILE_NAME)

        assert os.path.exists(appFolderPath), "Application folder does not exist."
        assert os.path.exists(appFilePath), "Application file does not exist."
