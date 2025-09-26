import pytest  # type: ignore
from unittest.mock import Mock
from utils.signal import Signal


def test_emit_signal():
    functionCallback = Mock()

    signal = Signal()

    signal.Attach(functionCallback)

    signal.Emit(None)

    functionCallback.assert_called_once_with(None)


def test_emit_multiple_times():
    functionCallback = Mock()

    signal = Signal()

    signal.Attach(functionCallback)

    signal.Emit(1)
    signal.Emit(2)
    signal.Emit(3)

    assert functionCallback.call_count == 3
    functionCallback.assert_any_call(1)
    functionCallback.assert_any_call(2)
    functionCallback.assert_any_call(3)


def test_attach_multiple_times():
    functionCallback1 = Mock()
    functionCallback2 = Mock()

    signal = Signal()

    signal.Attach(functionCallback1)
    signal.Attach(functionCallback2)

    signal.Emit(1)

    functionCallback1.assert_called_once_with(1)
    functionCallback2.assert_called_once_with(1)


def test_attach_with_another_signal():
    functionCallback = Mock()

    signal1 = Signal()
    signal2 = Signal()

    signal1.Attach(functionCallback)
    signal2.Attach(signal1)

    signal2.Emit(1)

    functionCallback.assert_called_once_with(1)
