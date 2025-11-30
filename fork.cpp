// for practical 2 - process control cmd

#include <iostream>
#include <unistd.h>     // For fork()
#include <sys/types.h>  // For pid_t

using namespace std;

int main() {
    cout << "I am the single parent. About to fork..." << endl;

    pid_t pid = fork();

    if (pid < 0) {
        // Error occurred
        cerr << "Fork failed" << endl;
        return 1;
    } 
    else if (pid == 0) {
        // Child process
        cout << "Child: Hello from the C++ CHILD process! (PID: " << getpid() << ")" << endl;
    } 
    else {
        // Parent process
        cout << "Parent: Hello from the C++ PARENT process! (Child's PID: " << pid << ")" << endl;
    }

    return 0;
}
