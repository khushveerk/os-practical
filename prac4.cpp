#include <iostream>
#include <unistd.h>     // For fork() and sleep()
#include <sys/wait.h>   // For wait()

using namespace std;

// This function is the "same code" that both processes can run.
void common_action(const string& process_name, int delay) {
    cout << "  [" << process_name << "] Running shared task. (My PID is: " << getpid() << ")" << endl;
    sleep(delay); // Wait for 'delay' seconds
}

int main() {
    cout << "--- Starting Program ---" << endl;
    cout << "The Original Process is running. PID: " << getpid() << endl;

    // 1. FORK: Create a clone.
    pid_t pid = fork();

    if (pid < 0) {
        // Error handling if fork fails
        cerr << "FATAL ERROR: Could not create a child process." << endl;
        return 1;
    } 
    else if (pid == 0) {
        // --- CHILD Process Code (pid is 0 here) ---
        
        cout << "\nCHILD: *** I am the clone! ***" << endl;

        // Requirement (ii): Same program, different code (This is the child's unique task)
        cout << "CHILD: My unique job is to count to three slowly." << endl;
        for (int i = 1; i <= 3; ++i) {
            common_action("Child (Counting)", 1);
        }
        
        // Requirement (i): Same program, same code (Running the shared function)
        common_action("Child (Shared Task)", 1);

        cout << "CHILD: I am done and exiting now." << endl;
        exit(0); 
    } 
    else {
        // --- PARENT Process Code (pid is the child's ID here) ---
        
        cout << "PARENT: Child has been created with PID: " << pid << endl;
        
        // Requirement (i): Same program, same code (Running the shared function)
        common_action("Parent (Shared Task)", 1); 

        // Requirement (iii): Parent waits for the child to finish.
        cout << "\nPARENT: *** Now I wait for the Child to finish... ***" << endl;
        
        // wait(NULL) is the simplest form of waiting [source: 93]
        wait(NULL); 

        cout << "PARENT: *** The Child has finished and returned. ***" << endl;

        // Requirement (ii): Same program, different code (Parent's unique task after waiting)
        cout << "PARENT: Now I can run my final unique code." << endl;
        common_action("Parent (Final Task)", 1);
    }

    cout << "\n--- Program Finished. PID: " << getpid() << " ---" << endl;
    return 0;
}