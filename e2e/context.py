import inspect
from typing import Generator, Callable, Any

functionNames: dict[str, Callable[..., Any]] = {}
functionSignatures: dict[str, inspect.Signature] = {}
setupCalls: dict[str, Any] = (
    {}
)  # store the output of the setup calls which will be passed to the dependent functions
setupCallsOrder: list[str] = []  # store the order of the setup calls


def context(func: Callable[..., Any]) -> Callable[..., Any]:
    """
    The decorator which mark the function for context tracking.
    """

    # check if function not generator function
    if not inspect.isgeneratorfunction(func):

        def generatorWrapper(*args: Any, **kwargs: Any) -> Generator[Any, None, None]:
            yield func(*args, **kwargs)

        functionNames[func.__name__] = generatorWrapper

    else:
        functionNames[func.__name__] = func

    functionSignatures[func.__name__] = inspect.signature(func)

    return func


def setup_context(testFunc: Callable[..., Any]) -> None:
    """
    Used for running the function with context tracking, all the args will be automatically passed if it's possible,
        else the error will be raised.

    Example
    -------
    ```python
    @context
    def setup_database():
        ...

    def test_database_connection(setup_database):
        ...

    run_context(test_database_connection) // the setup_database will be automatically called before test_database_connection
    ```
    """
    print("Setup context for:", testFunc.__name__)
    functionNames[testFunc.__name__] = testFunc
    functionSignatures[testFunc.__name__] = inspect.signature(testFunc)

    _run_setup_internal(testFunc.__name__, called=False)


def run_context(testFunc: Callable[..., Any]) -> Any:
    """
    Used for running the function with context tracking, all the args will be automatically passed if it's possible,
        else the error will be raised.
    """
    args: list[Any] = []
    funcSignature = functionSignatures[testFunc.__name__]
    for argName, _ in funcSignature.parameters.items():
        if argName not in functionNames:
            raise ValueError(
                f"Cannot find the context function '{argName}' required by '{testFunc.__name__}'"
            )

        _run_setup_internal(argName, True)

        args.append(setupCalls[argName])

    return testFunc(*args)


def teardown_context() -> None:
    """
    Used for tearing down the context, clearing all the stored setup calls.
    """
    for setupName in reversed(setupCallsOrder):
        func = functionNames[setupName]
        func()

    setupCalls.clear()
    setupCallsOrder.clear()


def _run_setup_internal(functionName: str, called: bool = True) -> None:
    if functionName in setupCalls:
        return

    func = functionNames[functionName]

    assert func is not None, f"Cannot find the context function '{functionName}'"

    funcSignature = functionSignatures[functionName]

    if len(funcSignature.parameters) == 0:
        if called:
            print("Run setup for:", functionName)
            result = next(func())
            setupCalls[functionName] = result
            setupCallsOrder.append(functionName)
        return

    args: list[Any] = []

    for argName, _ in funcSignature.parameters.items():
        if argName not in functionNames:
            raise ValueError(
                f"Cannot find the context function '{argName}' required by '{functionName}'"
            )

        _run_setup_internal(argName, True)

        args.append(setupCalls[argName])

    if called:
        print("Run setup for:", functionName)
        result = next(func(*args))
        setupCalls[functionName] = result
        setupCallsOrder.append(functionName)
