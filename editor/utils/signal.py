from typing import Any, Callable


class Signal:
    """
    The mechanism for notify changes in the data. It is similar to the signal/slot
    mechanism in Qt framework.
    """

    def __init__(self) -> None:
        self._callbacks: list[Callable[[Any], None]] = []
        self._signals: list[Signal] = []

    def Attach(self, callback: Callable[[Any], None] | "Signal") -> None:
        """
        Link a function callback or another signal to this signal. When this signal
        is emitted, all the linked callbacks and signals will be triggered as well.
        Note that if a signal is linked to itself, it will cause infinite recursion.
        Also note that if two signals are linked to each other, it will also cause
        infinite recursion.

        Parameters
        ----------
        callback : Callable[[Any], None] | Signal
            The function callback or another signal to link to this signal.
        """
        if isinstance(callback, Signal):
            self._signals.append(callback)
        else:
            self._callbacks.append(callback)

    def Emit(self, data: Any, emittedSignals: set["Signal"] | None = None) -> None:
        """
        Emit the signal with the given data. All the linked callbacks and signals
        will be triggered as well.

        Parameters
        ----------
        data : Any
            The data to pass to the callbacks and signals.
        """
        if emittedSignals is None:
            emittedSignals = set()

        emittedSignals.add(self)

        for signal in self._signals:
            if signal in emittedSignals:
                continue
            signal.Emit(data, emittedSignals | {signal})

        for callback in self._callbacks:
            callback(data)
