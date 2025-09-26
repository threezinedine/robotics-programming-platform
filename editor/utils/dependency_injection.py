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

    _transientsFactories: dict[str, Callable[[list[Any]], Any]] = {}
    """
    Used for creating transient objects, suitable for dependency injection
    between transients or between transients and singletons.
    """

    _dependencies: dict[str, list[Any]] = {}
    """
    Storing all dependent objects from the class name to the list of dependencies.
    """

    @staticmethod
    def RegisterSingleton(
        className: type[T] | T,
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

        finalName = ""

        if name is not None:
            finalName = name
        elif isinstance(className, type):
            finalName = className.__name__
        else:
            raise ValueError("name must be provided if className is not a class type.")

        if (
            finalName in DependencyInjection._singletons
            or finalName in DependencyInjection._singletonFactories
        ):
            logger.warning(
                f"Singleton '{finalName}' is already registered. Overwriting existing instance."
            )

        if finalName in DependencyInjection._transientsFactories:
            raise ValueError(
                f"Cannot register singleton '{finalName}' because a transient with the same name already exists."
            )

        if not isinstance(className, type):
            DependencyInjection._singletons[finalName] = (
                className  # directly register the instance
            )
        else:
            DependencyInjection._singletonFactories[finalName] = (
                lambda dependencies: className(*dependencies, *args, **kwargs)
            )

    @staticmethod
    def RegisterTransient(
        className: type[T],
        name: str | None = None,
        *args: Any,
        **kwargs: Any,
    ) -> None:
        """
        Registers a transient object with the dependency injection container. The
        object will be created every time it is requested.

        Parameters
        ----------
        cls : Callable[..., T]
            The class to register
        """
        name = className.__name__ if name is None else name

        if name in DependencyInjection._transientsFactories:
            logger.warning(
                f"Transient '{name}' is already registered. Overwriting existing instance."
            )

        if (
            name in DependencyInjection._singletons
            or name in DependencyInjection._singletonFactories
        ):
            raise ValueError(
                f"Cannot register transient '{name}' because a singleton with the same name already exists."
            )

        if name in DependencyInjection._transientsFactories:
            logger.warning(
                f"Transient '{name}' is already registered. Overwriting existing instance."
            )

        DependencyInjection._transientsFactories[name] = className

    @staticmethod
    def GetObject(name: str, *args: Any, **kwargs: Any) -> Any:
        """
        Retrieves the existed singleton object from the dependency injection
        container. If the singleton has not been registered, raises an error.

        Parameters
        ----------
        name : str
            The name of the singleton to retrieve.

        *args : Any
            The arguments to pass to the class constructor (for transient objects only).
            The arguments will be ignored for singleton objects.

        **kwargs : Any
            The keyword arguments to pass to the class constructor (for transient objects only).
            The keyword arguments will be ignored for singleton objects.
        """
        if name in DependencyInjection._singletonFactories:
            factory = DependencyInjection._singletonFactories.pop(name)

            dependencies = (
                []
                if name not in DependencyInjection._dependencies
                else [
                    DependencyInjection.GetObject(dep.__name__)
                    for dep in DependencyInjection._dependencies[name]
                ]
            )

            instance = factory(dependencies)
            DependencyInjection._singletons[name] = instance
            return instance

        if name in DependencyInjection._transientsFactories:
            factory = DependencyInjection._transientsFactories[name]

            dependencies = (
                []
                if name not in DependencyInjection._dependencies
                else [
                    DependencyInjection.GetObject(dep.__name__)
                    for dep in DependencyInjection._dependencies[name]
                ]
            )

            instance = factory(*dependencies, *args, **kwargs)
            return instance

        instance = DependencyInjection._singletons.get(name)
        if instance is None:
            logger.error(f"Singleton '{name}' is not registered.")
            raise ValueError(f"Singleton '{name}' is not registered.")
        return instance


def Depend(*dependencies: Any) -> Callable[[T], T]:
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


def AsSingleton(
    classType: type[U] | U,
    interfaceType: type[T] | None = None,
    *args: Any,
    **kwargs: Any,
) -> None:
    """
    Simple wrapper for easily registering a class as a singleton without any
    dependencies. Used this method manually at the beginning of the program to
    register all singletons.
    """

    registerName = ""

    if interfaceType is not None:
        registerName = interfaceType.__name__
    elif isinstance(classType, type):
        registerName = classType.__name__
    else:
        raise ValueError(
            "classType must be a class type or interface type must be provided."
        )

    DependencyInjection.RegisterSingleton(classType, registerName, *args, **kwargs)


def AsTransient(
    classType: type[U],
    interfaceType: type[T] | None = None,
    *args: Any,
    **kwargs: Any,
) -> None:
    """
    Simple wrapper for easily registering a class as a transient object without
    any dependencies. Used this method manually at the beginning of the program
    to register all transient objects.
    """
    registerName = (
        classType.__name__ if interfaceType is None else interfaceType.__name__
    )

    DependencyInjection.RegisterTransient(classType, registerName, *args, **kwargs)


def GetObject(interfaceType: type[T], *args: Any, **kwargs: Any) -> T:
    """
    Simple wrapper for easily retrieving a singleton object by its interface type.
    """
    return DependencyInjection.GetObject(interfaceType.__name__, *args, **kwargs)  # type: ignore
