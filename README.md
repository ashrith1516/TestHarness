# TestHarness
Mutli user test harness using C++ which accepts any test case in the form of a DLL. Executes and returns the log
Process Pool - Creates a new TestHarness process for each client connection and executes the test cases
TestHarnessCore - Core engine to load and execute test cases from each DLL in the queue
TestRequest - Example of creation of test request DLL
DLL Loader with hosted resource - Loading the DLLs from TestRequest and passing them to TestHarnessCore along with an instance of a shared Logger
Utilities - File and string operation utilities
