import os


class Constants:
    """
    The place where all constants are defined.
    """

    CONFIGURE_LOGGER_NAME = "CONFIGURE"

    # Project directories
    ABSOLUTE_BASE_DIR = os.getcwd()

    AUTOGEN_DIR = "autogen"
    ABSOLUTE_AUTOGEN_DIR = os.path.join(ABSOLUTE_BASE_DIR, AUTOGEN_DIR)
