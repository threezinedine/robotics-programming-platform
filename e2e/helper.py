from packages import *
from constants import *


def open_file_block():
    TestUtils.MoveToItem(EDITOR_FILES)
    TestUtils.LeftClick(EDITOR_FILES)


def assert_current_editor_window_title(expectedTitle: str):
    TestUtils.Assert(
        TestUtils.FindRendererIdByName(expectedTitle) != INVALID_ID,
        f"Window name is not '{expectedTitle}'",
    )


def assert_function_selected(functionName: str):
    TestUtils.Assert(
        TestUtils.IsItemFound(
            EDITOR_MAIN_FUNCTION_SELECTED_FORMAT.format(functionName)
        ),
        f"Function '{functionName}' is not selected",
    )


def assert_function_not_selected(functionName: str):
    TestUtils.Assert(
        not TestUtils.IsItemFound(
            EDITOR_MAIN_FUNCTION_SELECTED_FORMAT.format(functionName)
        ),
        f"Function '{functionName}' is selected",
    )
