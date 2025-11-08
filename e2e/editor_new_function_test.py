from packages import *  # this import will be deleted in the core, this line must be at the top of the file
from setup import setup_recent_projects #type: ignore
from constants import *


def test_create_new_function(setup_recent_projects: None):
    TestUtils.LeftClick(EDITOR_MAIN_TOOLBAR_NEW)

    TestUtils.Assert(
        TestUtils.IsItemFound(EDITOR_MAIN_TOOLBAR_NEW_POPUP),
        "New popup not found after clicking Main Toolbar New button",
    )

    TestUtils.LeftClick(EDITOR_MAIN_TOOLBAR_NEW_POPUP_FUNCTION)