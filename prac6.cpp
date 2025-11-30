#include <iostream>
#include <fstream>
#include <sys/utsname.h> // For uname()
#include <unistd.h>      // For uname()
#include <string>
#include <sstream>       // For stringstream (to parse lines)

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
        cerr << "Error: Could not open /proc/cpuinfo. File not found." << endl;
    }
    cout << "===========================================" << endl;
}

// NEW FUNCTION: Function to display configured memory, free, and used memory.
void report_memory_info() {
    cout << "\n===========================================" << endl;
    cout << "           MEMORY (RAM) INFORMATION" << endl;
    cout << "===========================================" << endl;

    // Memory information is available in the /proc/meminfo virtual file
    ifstream meminfo("/proc/meminfo");
    string line;

    long total_mem = 0;
    long free_mem = 0;
    long available_mem = 0;
    
    // Read the file line by line to find the required metrics
    if (meminfo.is_open()) {
        while (getline(meminfo, line)) {
            stringstream ss(line);
            string key;
            long value;
            string unit;

            ss >> key >> value >> unit;

            if (key == "MemTotal:") {
                total_mem = value;
            } else if (key == "MemFree:") {
                free_mem = value;
            } else if (key == "MemAvailable:") {
                available_mem = value;
            }

            // Optimization: Stop reading once all three values are found
            if (total_mem != 0 && free_mem != 0 && available_mem != 0) {
                break;
            }
        }
        meminfo.close();

        if (total_mem > 0) {
            // Memory values are typically in Kilobytes (KB)
            cout << "Total Configured Memory: " << total_mem << " KB" << endl;
            
            // Calculate Used Memory (Simple approximation)
            long used_mem = total_mem - free_mem;
            cout << "Used Memory (approx.):   " << used_mem << " KB" << endl;
            
            // Report Free and Available Memory
            cout << "Free Memory:             " << free_mem << " KB" << endl;
            cout << "Available Memory:        " << available_mem << " KB" << endl;
        } else {
            cerr << "Error: Could not find MemTotal in /proc/meminfo." << endl;
        }

    } else {
        cerr << "Error: Could not open /proc/meminfo. This file is required for memory statistics." << endl;
    }
    cout << "===========================================" << endl;
}


int main() {
    report_kernel_info();
    report_cpu_info();
    report_memory_info(); // Added the new function call

    return 0;
}