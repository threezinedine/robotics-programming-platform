from typing import Any, Callable, TypeVar
from .logger import logger

T = TypeVar("T")
U = TypeVar("U")


class DependencyInjection:
    """
    The center hub which manages all object lifetimes and dependencies. The main
    purpose of this class is to allow for easy dependency injection and lifetime
    management of objects. This class should be used as the only way to create
    objects which have dependencies on other objects.
    """

    _singletons: dict[str, Any] = {}
    """
    Storage of all singleton objects. The key is the name of the singleton
    and the value is the singleton instance.
    """

    _singletonFactories: dict[str, Callable[[list[Any]], Any]] = {}
    """
    Used for lazy initialization of singleton objects, suitable for dependency
    injection between singletons.
    """

    _dependencies: dict[str, list[Any]] = {}
    """
    Storing all dependent objects from the class name to the list of dependencies.
    """

    @staticmethod
    def RegisterSingleton(
        className: Callable[..., T],
        name: str | None = None,
        *args: Any,
        **kwargs: Any,
    ) -> None:
        """
        Registers a singleton object with the dependency injection container. The
        object will be created the first time it is requested and will be reused
        for all subsequent requests.

        Parameters
        ----------
        cls : Callable[..., T]
            The class to register as a singleton.
        name : str | None
            The name to register the singleton under. If None, the class name will
            be used.
        *args : Any
            The arguments to pass to the class constructor.
        **kwargs : Any
            The keyword arguments to pass to the class constructor.
        """

        if name is None:
            name = className.__name__

        if (
            name in DependencyInjection._singletons
            or name in DependencyInjection._singletonFactories
        ):
            logger.warning(
                f"Singleton '{name}' is already registered. Overwriting existing instance."
            )

        DependencyInjection._singletonFactories[name] = lambda dependencies: className(
            *dependencies, *args, **kwargs
        )

    @staticmethod
    def GetSingleton(name: str) -> Any:
        """
        Retrieves the existed singleton object from the dependency injection
        container. If the singleton has not been registered, raises an error.
        """
        if name in DependencyInjection._singletonFactories:
            factory = DependencyInjection._singletonFactories.pop(name)

            dependencies = (
                []
                if name not in DependencyInjection._dependencies
                else [
                    DependencyInjection.GetSingleton(dep.__name__)
                    for dep in DependencyInjection._dependencies[name]
                ]
            )

            instance = factory(dependencies)
            DependencyInjection._singletons[name] = instance
            return instance

        instance = DependencyInjection._singletons.get(name)
        if instance is None:
            logger.error(f"Singleton '{name}' is not registered.")
            raise ValueError(f"Singleton '{name}' is not registered.")
        return instance


def Depend(*dependencies: Any) -> Any:
    """
    Decorator method for marking that the passed class depends on the passed
    dependencies. The dependencies will be automatically injected into the
    class constructor.
    """

    def wrapper(className: T) -> T:
        """
        Just registering the dependencies for the class initialization.
        """
        DependencyInjection._dependencies[className.__name__] = list(dependencies)  # type: ignore

        return className

    return wrapper
