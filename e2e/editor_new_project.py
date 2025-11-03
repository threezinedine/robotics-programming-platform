from packages import *  # this import will be deleted in the core, this line must be at the top of the file

ImGuiTestUtils.LeftClick("Editor/MenuBar/File")
ImGuiTestUtils.MoveToItem("Editor/MenuBar/File/New")
ImGuiTestUtils.LeftClick("Editor/MenuBar/File/New/Project")
