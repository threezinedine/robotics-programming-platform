from Engine import LogLevel, Logging, SingletonManager, HandlerType, Project


def main():
    SingletonManager.Initialize()
    Logging.Setup(HandlerType.CONSOLE.value, LogLevel.DEBUG)
    project = Project.CreateProject('{"name": "TestProject"}')
    Logging.Log(LogLevel.ERROR, f"Project: {project.GetName()}", __file__, 10)
    SingletonManager.Shutdown()


if __name__ == "__main__":
    main()
