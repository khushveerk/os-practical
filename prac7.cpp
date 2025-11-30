#include <iostream>
#include <fstream>
#include <unistd.h>     // For read(), write(), close()
#include <fcntl.h>      // For open() and file flags (O_RDONLY, O_WRONLY, etc.)
#include <sys/stat.h>   // For file permissions (S_IRUSR, S_IWUSR, etc.)
#include <errno.h>      // For the errno variable
#include <cstring>      // For strerror() function declaration

using namespace std;

// Define a reasonable buffer size for efficient copying (e.g., 4KB)
#define BUFFER_SIZE 4096 

int main(int argc, char *argv[]) {
    // Check for correct command-line arguments
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <source_file> <destination_file>" << endl;
        return 1;
    }

    const char* source_path = argv[1];
    const char* dest_path = argv[2];

    // --- 1. Open Source File for Reading ---
    // O_RDONLY: Read-only access
    int input_fd = open(source_path, O_RDONLY);
    
    if (input_fd < 0) {
        // If open fails, report error and exit
        cerr << "Error opening source file (" << source_path << "): " << strerror(errno) << endl;
        return 2;
    }

    // --- 2. Open/Create Destination File for Writing ---
    // O_WRONLY: Write-only access
    // O_CREAT: Create file if it doesn't exist
    // O_TRUNC: Truncate file to zero length if it exists
    // S_IRUSR | S_IWUSR: File permissions (User Read/Write)
    int output_fd = open(dest_path, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

    if (output_fd < 0) {
        // If open fails, report error and close the source file before exiting
        cerr << "Error opening/creating destination file (" << dest_path << "): " << strerror(errno) << endl;
        close(input_fd);
        return 3;
    }

    // --- 3. Copy Data in Chunks ---
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    ssize_t bytes_written;

    cout << "Starting copy from " << source_path << " to " << dest_path << "..." << endl;

    // Loop until the end of the input file is reached (read() returns 0)
    while ((bytes_read = read(input_fd, buffer, BUFFER_SIZE)) > 0) {
        // Write the chunk of data we just read to the output file
        bytes_written = write(output_fd, buffer, bytes_read);

        if (bytes_written != bytes_read) {
            // This handles cases where only a partial write occurred or an error happened
            cerr << "Error during writing. Only wrote " << bytes_written << " of " << bytes_read << " bytes." << endl;
            close(input_fd);
            close(output_fd);
            return 4;
        }
    }

    // Check if the loop terminated because of an error during reading
    if (bytes_read < 0) {
        cerr << "Error during reading source file: " << strerror(errno) << endl;
        close(input_fd);
        close(output_fd);
        return 5;
    }

    // --- 4. Close File Descriptors ---
    close(input_fd);
    close(output_fd);

    // The logic below was incorrect, as input_fd is a file descriptor number, not the count.
    // We cannot easily track total bytes copied using only system calls without a counter variable.
    // For simplicity, we just confirm success.
    cout << "Copy successful!" << endl; 

    return 0;
}