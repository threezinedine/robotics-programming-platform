from packages import *  # this import will be deleted in the core, this line must be at the top of the file


def test_open_close_new_project_modal():
    ImGuiTestUtils.LeftClick("Editor/MenuBar/File")
    ImGuiTestUtils.MoveToItem("Editor/MenuBar/File/New")
    ImGuiTestUtils.LeftClick("Editor/MenuBar/File/New/Project")

    TestSystem.Wait(10)

    assert ImGuiTestUtils.IsItemFound(
        "Editor/NewProjectModal"
    ), "New Project modal not opened!"

    ImGuiTestUtils.LeftClick("Editor/NewProjectModal/Cancel")

    TestSystem.Wait(10)

    assert not ImGuiTestUtils.IsItemFound(
        "Editor/NewProjectModal"
    ), "New Project modal not closed!"


def test_click_new_file():
    ImGuiTestUtils.LeftClick("Editor/MenuBar/File")
    ImGuiTestUtils.MoveToItem("Editor/MenuBar/File/New")
    ImGuiTestUtils.LeftClick("Editor/MenuBar/File/New/File")
