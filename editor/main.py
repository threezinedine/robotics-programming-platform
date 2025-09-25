from Engine import (
    LogLevel,
    Logging,
    SingletonManager,
    HandlerType,
    Project,
    ProjectDescription,
    ToString_ProjectDescription,
)


def main():
    SingletonManager.Initialize()
    Logging.Setup(HandlerType.CONSOLE.value, LogLevel.DEBUG)
    projectDest: ProjectDescription = ProjectDescription()
    projectDest.name = "TestProject"
    project = Project.CreateProject(projectDest)
    Logging.Log(
        LogLevel.ERROR,
        f"Project: {project.GetName()} - Json: {ToString_ProjectDescription(projectDest)}",
        __file__,
        10,
    )
    SingletonManager.Shutdown()


if __name__ == "__main__":
    main()
