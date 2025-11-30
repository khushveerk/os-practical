#include <iostream>
#include <unistd.h>     // For fork(), pipe(), dup2()
#include <sys/wait.h>   // For wait()
#include <stdlib.h>     // For exit()
#include <assert.h>     // For assert()

using namespace std;

int main() {
    int pipefd[2]; // Array to hold the two ends of the pipe
    pid_t p1, p2;

    // Create the pipe
    if (pipe(pipefd) < 0) {
        perror("pipe failed");
        return 1;
    }
    // assert(pipe(pipefd) == 0); // Assert is an alternative error check from the PDF

    // --- First Child (Writer: ls) ---
    p1 = fork();
    if (p1 == 0) {
        cout << "Child 1: executing ls and writing to pipe..." << endl;
        
        close(pipefd[0]); // Close read end (Writer doesn't read)
        dup2(pipefd[1], STDOUT_FILENO); // Point stdout to pipe write end
        close(pipefd[1]); // Close the original pipe file descriptor
        
        // Transform into 'ls'
        execlp("ls", "ls", NULL); 
        
        // Only reached on failure
        cerr << "Child 1: Exec failed!" << endl;
        exit(1); 
    }

    // --- Second Child (Reader: wc) ---
    p2 = fork();
    if (p2 == 0) {
        cout << "Child 2: executing wc and reading from pipe..." << endl;
        
        close(pipefd[1]); // Close write end (Reader doesn't write)
        dup2(pipefd[0], STDIN_FILENO); // Point stdin to pipe read end
        close(pipefd[0]); // Close the original pipe file descriptor
        
        // Transform into 'wc -l' (count lines)
        execlp("wc", "wc", "-l", NULL);
        
        // Only reached on failure
        cerr << "Child 2: Exec failed!" << endl;
        exit(1);
    }

    // --- Parent ---
    // Parent must close both ends of the pipe to ensure the Reader (wc)
    // receives an End-Of-File (EOF) signal when the Writer (ls) is done.
    close(pipefd[0]);
    close(pipefd[1]);

    // Wait for both children to finish
    wait(NULL);
    wait(NULL);

    cout << "Parent: Pipeline finished." << endl;
    return 0;
}