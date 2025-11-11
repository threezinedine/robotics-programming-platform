from packages import *


def assert_current_editor_window_title(expectedTitle: str):
    TestUtils.Assert(
        TestUtils.FindRendererIdByName(expectedTitle) != INVALID_ID,
        f"Window name is not '{expectedTitle}'",
    )
