from packages import *
from constants import *
from helper import open_file_block, assert_title_exist
from setup import setup_project_with_functions  # type: ignore


def test_open_function(setup_project_with_functions: None):
    open_file_block()
    assert_title_exist("Editor - ok")

    TestUtils.DoubleClick(EDITOR_MAIN_FUNCTION_FORMAT.format("ExistingFunction"))
    TestSystem.Wait(10)

    assert_title_exist("ExistingFunction")
