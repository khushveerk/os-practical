// practical 3 -  iii. Before terminating, the parent waits for the child to finish its task.
#include <iostream>
#include <unistd.h>     // fork, sleep, getpid
#include <sys/types.h>  // pid_t
#include <sys/wait.h>   // wait

using namespace std;

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        cout << "Fork failed" << endl;
    } 
    else if (pid == 0) {
        cout << "Child running... PID: " << getpid() << endl;
        sleep(2);   // simulate some work
        cout << "Child finished\n";
    } 
    else {
        wait(NULL); // parent waits for child to finish
        cout << "Parent waited for child. Parent PID: " << getpid() << endl;
    }

    return 0;
}
