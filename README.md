-------------------------------------------------------------------------------
Command Sequence Parser: v0.01 (March 12, 2023)
-------------------------------------------------------------------------------
This is a sequence parser that parses the data from the buffer filled by the test harness and counts the valid command sequence 0xA55A 

Environment:
The application is written and tested in the below environment
1.Ubuntu 22.04
2.gcc version 11.3.0
3.c++17 is default version used
4.libgtest-dev version 1.11.0-3 for google test framework
5.valgrind-3.18.1

Build and test:
1.In the CommandParserApp folder(top folder) run the following command to build the test application
 $ g++ TestApp.cpp -lgtest -lpthread -Wall -o testapp

2.Run the testapp binary to execute different tests from the google test framework
 $ ./testapp
