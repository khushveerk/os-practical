/* 4. Write a program to report behaviour of Linux kernel including kernel
version, CPU type and model. (CPU information)*/

#include <iostream>
#include stdlib>
using namespace std;

int main() {
    cout << "--- Kernel information ---" << endl;
    system("uname -s");
    system("uname -r");
    system("uname -v");
    system("uname -m");

    cout << "\n--- CPU information ---" << endl;
    system("lscpu");

    return 0;
}
