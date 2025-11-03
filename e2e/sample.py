from packages import *  # this import will be deleted in the core, this line must be at the top of the file

print("Start")
TestSystem.Wait(1000)
ImGuiTestUtils.MoveToItem("Click Me")
TestSystem.Wait(1000)

ImGuiTestUtils.MoveToItem("Test Button")
TestSystem.Wait(1000)

InputSystem.ClickMouse(0)

TestSystem.Wait(2000)
print("End")
