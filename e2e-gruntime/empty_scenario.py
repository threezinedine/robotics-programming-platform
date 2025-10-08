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

    newFile = FileSystem.OpenFile("output.txt", 1)  # 1 = write mode
    FileSystem.Write(newFile, "This is a test write operation.\n")
    FileSystem.CloseFile(newFile)

    readFile = FileSystem.OpenFile("output.txt", 0)  # 0 = read mode
    if FileSystem.IsFileOpen(readFile):
        readContent = FileSystem.Read(readFile)
        TestSystem.Log(f"Read from output.txt: {readContent}")
        FileSystem.CloseFile(readFile)

TestSystem.Wait(1000)  # Wait for 1 second

TestSystem.Log("This is a log message from empty_scenario.py")

TestSystem.Wait(2000)

print("Exiting empty_scenario.py")
