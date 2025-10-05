import os
import json
import subprocess
from dataclasses import dataclass
import typing

from .constants import Constants
from .logger import logger
from .utils.run_command import RunCommand


@dataclass
class Dependency:
    """
    The simple data for extracting the data from dependencies.json.
    """

    name: str
    url: str
    commit: typing.Optional[str] = None


def InstallCppDependencies() -> None:
    """
    Installs C/C++ dependencies using git (not submodules).
    """

    dependenciesJson = os.path.join(Constants.ABSOLUTE_BASE_DIR, "dependencies.json")

    dependencies: list[Dependency] = []

    with open(dependenciesJson, "r", encoding="utf-8") as f:
        dependenciesData = json.load(f)

        dependencies = [Dependency(**dep) for dep in dependenciesData]

    for dep in dependencies:
        dependenciesDir = os.path.join(
            Constants.ABSOLUTE_BASE_DIR, "externals", dep.name
        )

        try:
            if os.path.exists(dependenciesDir):
                logger.info(f"Dependency '{dep.name}' already exists. Skipping...")
                continue

            command = f"git clone {dep.url} {dependenciesDir} {'--recurse-submodules' if not Constants.IsWindowsPlatform() else ''}"

            logger.info(f"Installing C/C++ dependency '{dep.name}'...")

            RunCommand(command)

            if dep.commit is not None:
                RunCommand(f"git checkout {dep.commit}", cwd=dependenciesDir)

            logger.debug(f"Dependency '{dep.name}' installed successfully.")
        except Exception as e:
            logger.error(f"Failed to install C/C++ dependency '{dep.name}'.")
            raise RuntimeError("Failed to install C/C++ dependencies.") from e
