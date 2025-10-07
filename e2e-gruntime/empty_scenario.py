from TestSystem import *  # type: ignore

# TestLog("Hello from empty_scenario.py")
print("Hello from empty_scenario.py")

Wait(1000)  # Wait for 1 second

Log("This is a log message from empty_scenario.py")

Wait(2000)

print("Exiting empty_scenario.py")
