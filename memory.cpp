/*5. Write a program to report behaviour of Linux kernel including
configured memory, amount of free and used memory. (Memory
information)*/

#include <fstream>
#include <string>
#include <iostream>
using namespace std;

int main() {
    ifstream file("/proc/meminfo");
    string line;

    if (!file) {
        cout << "Error opening /proc/meminfo" << endl;
        return 1;
    }

    cout << "----memory information----" << endl;

    int count = 0;
    while (getline(file, line) && count < 5) {
        cout << line << endl;
        count++;
    }

    file.close();
    return 0;
}
