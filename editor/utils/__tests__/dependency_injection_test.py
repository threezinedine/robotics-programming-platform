from typing import Generator
import pytest  # type: ignore
from utils import DependencyInjection


class SingletonObject:
    """
    Used only for singleton and transient object tests.

    Parameters
    ----------
    value : int
        The value to set the count to, used for checking passed parameters.
    """

    numberOfCount: int = 0

    def __init__(self, value: int = 0) -> None:
        SingletonObject.numberOfCount += 1
        self.count = SingletonObject.numberOfCount
        self.value = value


@pytest.fixture
def setup() -> Generator[None, None, None]:
    # Reset the singleton registry before each test
    DependencyInjection._singletons = {}  # type: ignore
    SingletonObject.numberOfCount = 0
    yield
    # Cleanup after test
    DependencyInjection._singletons = {}  # type: ignore
    SingletonObject.numberOfCount = 0


def test_register_singleton(setup: None) -> None:
    assert (
        SingletonObject.numberOfCount == 0
    ), "Count should be 0 before any objects are made."

    DependencyInjection.RegisterSingleton(SingletonObject, value=5)

    obj1 = DependencyInjection.GetSingleton(SingletonObject.__name__)

    assert obj1 is not None, "Singleton object should not be None."
    assert (
        SingletonObject.numberOfCount == 1
    ), "Count should be 1 since one singleton instance should have been created."
    assert (
        obj1.value == 5
    ), "Singleton object should have been initialized with value 5."

    obj2 = DependencyInjection.GetSingleton(SingletonObject.__name__)
    assert obj1 is obj2, "Both singleton objects should be the same instance."
    assert (
        SingletonObject.numberOfCount == 1
    ), "Count should be 1 since only one singleton instance should have been created."
    assert (
        obj2.value == 5
    ), "Singleton object should have been initialized with value 5."


def test_register_existed_singleton(setup: None) -> None:
    assert (
        SingletonObject.numberOfCount == 0
    ), "Count should be 0 before registering the singleton again."

    DependencyInjection.RegisterSingleton(int, SingletonObject.__name__, 20)
    DependencyInjection.RegisterSingleton(SingletonObject, value=10)

    obj = DependencyInjection.GetSingleton(SingletonObject.__name__)
    assert obj is not None, "Singleton object should not be None."
    assert (
        SingletonObject.numberOfCount == 1
    ), "Count should still be 1 since only one singleton instance should have been created."

    assert (
        obj.value == 10
    ), "Singleton object should still have the initial value of 10."
