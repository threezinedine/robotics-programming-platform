from typing import Generator
import pytest  # type: ignore
from utils import DependencyInjection, Depend, AsSingleton, GetObject


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


@Depend(SingletonObject)
class DependentSingleton:
    """
    A singleton object which depends on another singleton object.
    """

    numberOfCount: int = 0

    def __init__(self, singleton: SingletonObject) -> None:
        DependentSingleton.numberOfCount += 1
        self.singleton = singleton


@pytest.fixture
def setup() -> Generator[None, None, None]:
    # Reset the singleton registry before each test
    DependencyInjection._singletons = {}  # type: ignore
    SingletonObject.numberOfCount = 0
    DependentSingleton.numberOfCount = 0
    yield
    # Cleanup after test
    DependencyInjection._singletons = {}  # type: ignore
    SingletonObject.numberOfCount = 0
    DependentSingleton.numberOfCount = 0


def test_register_singleton(setup: None) -> None:
    assert (
        SingletonObject.numberOfCount == 0
    ), "Count should be 0 before any objects are made."

    DependencyInjection.RegisterSingleton(SingletonObject, value=5)

    assert (
        SingletonObject.numberOfCount == 0
    ), "Count should still be 0 since no singleton instance should have been created yet."

    obj1 = DependencyInjection.GetSingleton(
        SingletonObject.__name__
    )  # only create singleton object at the first time is called

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


def test_add_dependency_between_singletons(setup: None) -> None:
    DependencyInjection.RegisterSingleton(SingletonObject, value=15)
    DependencyInjection.RegisterSingleton(DependentSingleton)

    dependent = DependencyInjection.GetSingleton(DependentSingleton.__name__)

    assert dependent is not None, "Dependent singleton should not be None."
    assert (
        DependentSingleton.numberOfCount == 1
    ), "Count should be 1 since one dependent singleton instance should have been created."

    assert (
        dependent.singleton is not None
    ), "The singleton dependency should have been injected and should not be None."

    assert (
        SingletonObject.numberOfCount == 1
    ), "Count should be 1 since only one singleton instance should have been created."


def test_get_non_existent_singleton(setup: None) -> None:
    with pytest.raises(ValueError):
        DependencyInjection.GetSingleton("NonExistentSingleton")


def test_register_singleton_using_object(setup: None) -> None:
    obj = SingletonObject(value=30)
    DependencyInjection.RegisterSingleton(obj, SingletonObject.__name__)

    assert (
        SingletonObject.numberOfCount == 1
    ), "Count should be 1 since one singleton instance should have been created."

    retrieved_obj = DependencyInjection.GetSingleton(SingletonObject.__name__)
    assert (
        retrieved_obj is obj
    ), "The retrieved singleton should be the same as the registered object."
    assert (
        retrieved_obj.value == 30
    ), "The retrieved singleton should have the value of 30."


def test_register_via_wrapper(setup: None) -> None:
    AsSingleton(SingletonObject, value=42)
    AsSingleton(DependentSingleton)

    assert (
        SingletonObject.numberOfCount == 0
    ), "Count should be 0 since no singleton instance should have been created yet."

    dependent = GetObject(DependentSingleton)
    assert dependent is not None, "Dependent singleton should not be None."
    assert (
        DependentSingleton.numberOfCount == 1
    ), "Count should be 1 since one dependent singleton instance should have been created."
    assert (
        SingletonObject.numberOfCount == 1
    ), "Count should be 1 since one singleton instance should have been created."
