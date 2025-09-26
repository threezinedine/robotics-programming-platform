import os
import json
from dacite import from_dict
from dataclasses import dataclass, field, asdict
from constants import APPLICATION_FILE_NAME, APPLICATION_FOLDER_NAME
from utils.dependency_injection import Depend
from utils.signal import Signal
from utils.logger import logger


@dataclass
class Version:
    major: int = field(default=1)
    minor: int = field(default=0)
    patch: int = field(default=0)


@dataclass
class Application:
    """
    The user's customed data for the application.
    """

    version: Version = field(default_factory=Version)

    recentProjects: list[str] = field(default_factory=list)  # type: ignore
    """
    The 5 most recent projects, stored as absolute paths.
    """


@Depend(Application)
class ApplicationModel:
    """
    The model for the application data.
    """

    def __init__(self, application: Application) -> None:
        self.application = application
        self.appDataFolder = os.getenv("APPDATA")
        assert (
            self.appDataFolder is not None
        ), "APPDATA environment variable is not set."

        self.appFolder = os.path.join(self.appDataFolder, APPLICATION_FOLDER_NAME)

        self.appFile = os.path.join(self.appFolder, APPLICATION_FILE_NAME)

        self.recentProjectsSignal = Signal()

    @property
    def RecentProjectsSignal(self) -> Signal:
        return self.recentProjectsSignal

    def Create(self) -> None:
        """
        Initialize the application data as configured. The data will be saved into
        user's APPDATA folder.
        """
        logger.debug("Loading application data...")
        if not os.path.exists(self.appFolder):
            os.makedirs(self.appFolder)
            logger.info(f"Created application folder at {self.appFolder}")

        appFile = os.path.join(self.appFolder, APPLICATION_FILE_NAME)
        if not os.path.exists(appFile):
            with open(appFile, "w") as f:
                f.write(json.dumps(asdict(Application()), indent=4))

    def Load(self) -> None:
        """
        Load the application data from the file.
        """
        with open(self.appFile, "r") as f:
            self.application = from_dict(
                data_class=Application,
                data=json.load(f),
            )
