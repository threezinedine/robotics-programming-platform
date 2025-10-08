from packages import *  # this import will be deleted in the core, this line must be at the top of the file

print("Hello from empty_scenario.py")

file = FileSystem.OpenFile("test.txt", 0)  # 0 = read mode
if FileSystem.IsFileOpen(file):
    TestSystem.Log("File opened successfully.")
    content = FileSystem.Read(file)
    TestSystem.Log(f"File content: {content}")
    FileSystem.CloseFile(file)
else:
    TestSystem.Log("Failed to open file.")

TestSystem.Wait(1000)  # Wait for 1 second

TestSystem.Log("This is a log message from empty_scenario.py")

TestSystem.Wait(2000)

print("Exiting empty_scenario.py")
