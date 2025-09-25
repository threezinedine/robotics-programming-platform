from Engine import LogLevel, Logging, SingletonManager, HandlerType


def main():
    SingletonManager.Initialize()
    Logging.Setup(HandlerType.CONSOLE, LogLevel.DEBUG)
    Logging.Log(LogLevel.ERROR, "This is an info message.", __file__, 10)
    SingletonManager.Shutdown()


if __name__ == "__main__":
    main()
