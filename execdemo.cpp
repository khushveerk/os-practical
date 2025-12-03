/* if faculty asks about exec()): child can run another program: 
 file name - execdemo.cpp 

cmds -
  g++ execdemo.cpp -o execdemo
  ./execdemo
*/
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <iostream>
using namespace std;

int main() {
    pid_t pid = fork();

    if (pid == 0) {
        cout << "Child: executing ls using exec()\n";
        execl("/bin/ls", "ls", NULL);
        cout << "This prints only if exec fails\n";
    } else if (pid > 0) {
        wait(NULL);
        cout << "Parent: child finished exec\n";
    } else {
        cout << "Fork failed\n";
    }
}
