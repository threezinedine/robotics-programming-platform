from Engine import LogLevel, Logging, SingletonManager


def main():
    SingletonManager.Initialize()
    Logging.Log(LogLevel.INFO, "This is an info message.", __file__, 10)
    SingletonManager.Shutdown()


if __name__ == "__main__":
    main()
