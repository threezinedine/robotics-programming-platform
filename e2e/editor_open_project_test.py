from packages import *  # this import will be deleted in the core, this line must be at the top of the file
from constants import *


def test_open_project():
    TestUtils.LeftClick(EDITOR_MENUBAR_FILE)
    TestUtils.MoveToItem(EDITOR_MENUBAR_OPEN)
    TestUtils.LeftClick(EDITOR_MENUBAR_OPEN_PROJECT)

    TestSystem.Wait(10)

    TestUtils.Assert(
        TestUtils.IsItemFound(EDITOR_OPEN_PROJECT_MODAL),
        "Open Project modal not found after clicking File->Open->Project",
    )
