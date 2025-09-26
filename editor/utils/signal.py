from typing import Any, Callable


class Signal:
    """
    The mechanism for notify changes in the data.
    """

    def __init__(self) -> None:
        self._callbacks: list[Callable[[Any], None]] = []
        self._signals: list[Signal] = []

    def Attach(self, callback: Callable[[Any], None] | "Signal") -> None:
        if isinstance(callback, Signal):
            self._signals.append(callback)
        else:
            self._callbacks.append(callback)

    def Emit(self, data: Any, emittedSignals: set["Signal"] = set()) -> None:
        emittedSignals.add(self)

        for signal in self._signals:
            if signal in emittedSignals:
                continue
            signal.Emit(data, emittedSignals | {signal})

        for callback in self._callbacks:
            callback(data)
