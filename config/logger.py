"""
Where the logger is defined, the logger is used throughout configuration (not on the core).
The default logging level of the logger is `INFO`, but can be changed to `DEBUG` for more verbose output.

Usage:
```python
from config.logger import logger

logger.debug("This is a debug message")
logger.info("This is an info message")
logger.warning("This is a warning message")
logger.error("This is an error message")
```
"""

import logging
from .constants import Constants

logger = logging.getLogger(Constants.CONFIGURE_LOGGER_NAME)
formatter = logging.Formatter("[%(levelname)s] - %(message)s")
console_handler = logging.StreamHandler()
console_handler.setFormatter(formatter)

logger.addHandler(console_handler)
