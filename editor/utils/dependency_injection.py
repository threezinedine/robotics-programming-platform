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

        instance = className(*args, **kwargs)

        if name in DependencyInjection._singletons:
            logger.warning(
                f"Singleton '{name}' is already registered. Overwriting existing instance."
            )

        DependencyInjection._singletons[name] = instance

    @staticmethod
    def GetSingleton(name: str) -> Any:
        """
        Retrieves the existed singleton object from the dependency injection
        container. If the singleton has not been registered, raises an error.
        """
        instance = DependencyInjection._singletons.get(name)
        if instance is None:
            logger.error(f"Singleton '{name}' is not registered.")
            raise ValueError(f"Singleton '{name}' is not registered.")
        return instance
