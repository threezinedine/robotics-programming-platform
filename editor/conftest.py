import pytest
from typing import Generator
from utils.config import StartupConfig


@pytest.fixture
def uiConfig() -> Generator[None, None, None]:
    """
    A pytest fixture which sets up the UI configuration before running the tests.
    """
    import warnings

    warnings.filterwarnings("ignore", category=DeprecationWarning)

    StartupConfig()
    yield
