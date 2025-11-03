from packages import *  # this import will be deleted in the core, this line must be at the top of the file

print("Start")
TestSystem.Wait(1000)
ImGuiTestUtils.LeftClick("Click Me")
TestSystem.Wait(1000)

ImGuiTestUtils.LeftClick("Test Button")

TestSystem.Wait(2000)

print(ImGuiTestUtils.IsItemFound("Test Button"))
print(ImGuiTestUtils.IsItemFound("Nonexistent Item"))

print("End")
