/**
 * Build:
 * g++ main-serv.cpp -o main-serv.exe
 **/

#include <iostream>

using namespace std;

int main()
{
    cerr << "Here I am!" << endl;

    double d;
    while (cin >> d) {
        cout << d*d << endl;
    }

    return 0;
}
