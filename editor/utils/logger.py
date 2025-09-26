import logging
from Engine import Logging, LogLevel

logger = logging.getLogger(__name__)
logger.setLevel(logging.DEBUG)


class EditorLoggerHandler(logging.Handler):
    def emit(self, record: logging.LogRecord) -> None:
        Logging.Log(
            LogLevel(record.levelno),
            record.getMessage(),
            record.pathname,
            record.lineno,
        )


# Add the custom handler to the root logger
logger.addHandler(EditorLoggerHandler())
