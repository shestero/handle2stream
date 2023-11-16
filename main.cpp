/**
 * Build:
 * g++ main.cpp -o caller.exe
 * Depends on:
 * main-serv.exe
 **/

#include "winiostream.hpp"  // obuf, ibuf
#include "createproc.hpp"   // CreateChildProcess

#include <windows.h>
#include <fcntl.h>
#include <fstream>
#include <iostream>

using namespace std;

int main()
{
    cout << "caller:\tHello!" << endl;

    TCHAR szCmdline[] = TEXT(".\\main-serv.exe");
    HANDLE hProcess = CreateChildProcess(szCmdline);
    if (hProcess == NULL) {
        cerr << "Error: cannot start main-serv !" << endl;
        return 1;
    }

    // Convet HADLE's to file descriptors and then to std:: input/output streams:
    obuf bo(g_hChildStd_IN_Wr);
    ostream os(&bo);
    ibuf bi(g_hChildStd_OUT_Rd);
    istream is(&bi);
    ibuf be(g_hChildStd_ERR_Rd);
    istream es(&be);

    // Read 'greeting' from stderr:
    string er;
    getline(es, er);
    cout << "Child process 'greating': " << er << endl;

    auto xx = { 1, 2, 4, 5, 9 };
    double y;
    for ( auto x : xx ) {
        os << x << endl;
        bo.flush();
        is >> y;
        cout << "x=" << x << "\t--> y=" << y << endl;
    }

    cout << "Task compleated." << endl;

    CloseHandle(hProcess);

    cout << "caller:\tBye!" << endl;
    return 0;
}
