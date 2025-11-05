from packages import *  # this import will be deleted in the core, this line must be at the top of the file
from constants import *


def test_open_project():
    TestUtils.LeftClick(EDITOR_MENUBAR_FILE)
    TestUtils.MoveToItem(EDITOR_MENUBAR_OPEN)
    TestUtils.MoveToItem(EDITOR_MENUBAR_OPEN_PROJECT)
