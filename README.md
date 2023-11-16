# handle2stream
Demo code: "converting" Windows file HANDLE and C FILE* into C++ i/o stream. _(WinAPI !!)_

This code shows how to 
1. call a child MS Windows exe process to use it like "server" and intercept its Std{In/Out/Err};
2. "convert" WinAPI in/out/err HANDLE into C FILE* and then into C++ i/o streams.

### COMPILE AND RUN:
```
C:\Users\user\calltest>g++ main-serv.cpp -o main-serv.exe

C:\Users\user\calltest>g++ -std=c++1z main.cpp -o caller.exe

C:\Users\user\calltest>caller
caller: Hello!
Child process 'greating': Here I am!
x=1     --> y=1
x=2     --> y=4
x=4     --> y=16
x=5     --> y=25
x=9     --> y=81
Task compleated.
caller: Bye!

```
