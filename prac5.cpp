#include <iostream>
#include <fstream>
#include <sys/utsname.h> // For uname()
#include <unistd.h>      // For uname() (sometimes needed)
#include <string>

using namespace std;

// Function to display kernel information using uname()
void report_kernel_info() {
    struct utsname system_info;

    cout << "===========================================" << endl;
    cout << "         LINUX KERNEL INFORMATION" << endl;
    cout << "===========================================" << endl;

    if (uname(&system_info) == 0) {
        cout << "System Name (OS):   " << system_info.sysname << endl;
        // Requirement: Kernel Version
        cout << "Kernel Version:     " << system_info.release << endl;
        cout << "Kernel Build:       " << system_info.version << endl;
        // Requirement: CPU Type (Architecture)
        cout << "Architecture:       " << system_info.machine << endl;
        cout << "Hostname:           " << system_info.nodename << endl;
    } else {
        cerr << "Error: Could not retrieve kernel information using uname()." << endl;
    }
    cout << "===========================================" << endl;
}

// Function to display CPU information by reading /proc/cpuinfo
void report_cpu_info() {
    cout << "\n===========================================" << endl;
    cout << "             CPU INFORMATION" << endl;
    cout << "===========================================" << endl;

    // Requirement: CPU Information
    // On Linux, detailed CPU info is available in the /proc/cpuinfo virtual file
    ifstream cpuinfo("/proc/cpuinfo");
    string line;

    if (cpuinfo.is_open()) {
        while (getline(cpuinfo, line)) {
            // Display only key lines like 'model name', 'cpu family', and 'processor'
            if (line.find("processor") == 0 || 
                line.find("model name") == 0 ||
                line.find("cpu family") == 0) {
                
                cout << line << endl;
            }
        }
        cpuinfo.close();
    } else {
        cerr << "Error: Could not open /proc/cpuinfo. This file is required for detailed CPU data." << endl;
    }
    cout << "===========================================" << endl;
}

int main() {
    report_kernel_info();
    report_cpu_info();

    return 0;
}