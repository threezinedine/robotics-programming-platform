from Engine import LogLevel, Logging


def main():
    print(f"Log Level: {LogLevel.DEBUG}")
    print("Hello, Robotics Programming Platform!")
    Logging.Log(LogLevel.INFO, "This is an info message.", __file__, 10)


if __name__ == "__main__":
    main()
