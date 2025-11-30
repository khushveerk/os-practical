#include <iostream>
#include <unistd.h>     // For fork() and execvp()
#include <sys/wait.h>   // For wait()

using namespace std;

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        cerr << "Fork failed" << endl;
        return 1;
    }

    if (pid == 0) {
        // Child Process
        cout << "Child: I am going to become 'ls' now..." << endl;
        
        // Arguments for the command (must be C-style char* array, ending in NULL)
        char *args[] = {"ls", "-l", NULL}; 
        
        // Execute the command. If successful, this function NEVER returns.
        execvp("ls", args);
        
        // We only reach here if execvp fails
        cerr << "Child: Exec failed! Could not run ls." << endl;
        // It's important to exit in the child on failure to avoid running parent code
        exit(1);
    } 
    else {
        // Parent Process
        cout << "Parent: I will wait for my child (PID " << pid << ") to finish." << endl;
        wait(NULL); 
        cout << "Parent: Child finished. I can quit now." << endl;
    }

    return 0;
}