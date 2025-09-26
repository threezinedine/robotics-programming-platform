import os
import logging
from Engine import Logging, LogLevel

logger = logging.getLogger(__name__)
logger.setLevel(logging.DEBUG)


class EditorLoggerHandler(logging.Handler):
    def emit(self, record: logging.LogRecord) -> None:
        level = LogLevel.DEBUG

        if record.levelno == logging.DEBUG:
            level = LogLevel.DEBUG
        elif record.levelno == logging.INFO:
            level = LogLevel.INFO
        elif record.levelno == logging.WARNING:
            level = LogLevel.WARNING
        elif record.levelno == logging.ERROR:
            level = LogLevel.ERROR
        else:
            level = LogLevel.FATAL

        Logging.Log(
            level,
            record.getMessage(),
            os.path.relpath(record.pathname, os.path.join(os.getcwd(), "..")),
            record.lineno,
        )


# Add the custom handler to the root logger
logger.addHandler(EditorLoggerHandler())
