#include <iostream>
#include <vector>
#include <pthread.h>
#include <numeric>
#include <cmath>

using namespace std;

// Define the number of threads to use
#define NUM_THREADS 4

// Global data structure to hold thread-specific information
struct ThreadData {
    int thread_id;          // Identifier for the thread
    int start_index;        // Starting index in the array for this thread
    int end_index;          // Ending index (exclusive)
    const vector<int>* array; // Pointer to the shared array
    long long partial_sum;  // Thread's calculated sum
};

// Global array and total size
vector<int> numbers;
int array_size = 0;

// Function executed by each thread
void* calculate_partial_sum(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    long long sum = 0;

    for (int i = data->start_index; i < data->end_index; ++i) {
        sum += (*data->array)[i];
    }
    
    data->partial_sum = sum;
    pthread_exit(NULL);
}

int main() {
    array_size = 200000; // Example: Sum 200,000 numbers
    
    // Initialize the array with simple values (e.g., all 1s)
    numbers.resize(array_size);
    iota(numbers.begin(), numbers.end(), 1); // Fill with 1, 2, 3, ...

    pthread_t threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];
    int chunk_size = array_size / NUM_THREADS;
    long long final_total_sum = 0;

    cout << "Starting multi-threaded sum calculation for " << array_size << " numbers using " << NUM_THREADS << " threads." << endl;

    // 1. Create and Launch Threads
    for (int i = 0; i < NUM_THREADS; ++i) {
        thread_data[i].thread_id = i;
        thread_data[i].array = &numbers;
        thread_data[i].start_index = i * chunk_size;
        
        // Ensure the last thread handles any remainder
        if (i == NUM_THREADS - 1) {
            thread_data[i].end_index = array_size;
        } else {
            thread_data[i].end_index = (i + 1) * chunk_size;
        }

        cout << "  Thread " << i << " created: processing indices " 
             << thread_data[i].start_index << " to " 
             << thread_data[i].end_index - 1 << endl;

        // Create the thread and tell it to run calculate_partial_sum
        int rc = pthread_create(&threads[i], NULL, calculate_partial_sum, (void*)&thread_data[i]);
        if (rc) {
            cerr << "Error: unable to create thread, " << rc << endl;
            return 1;
        }
    }

    // 2. Wait for all threads to complete (Join)
    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }
    
    cout << "\nAll threads completed execution." << endl;

    // 3. Combine Partial Sums
    for (int i = 0; i < NUM_THREADS; ++i) {
        final_total_sum += thread_data[i].partial_sum;
    }

    // 4. Verification (Sum of 1 to N is N*(N+1)/2)
    long long verification_sum = (long long)array_size * ((long long)array_size + 1) / 2;

    cout << "------------------------------------------" << endl;
    cout << "Calculated Total Sum: " << final_total_sum << endl;
    cout << "Verification Sum:     " << verification_sum << endl;

    if (final_total_sum == verification_sum) {
        cout << "Result Verified: Success!" << endl;
    } else {
        cerr << "Result Error: Sums do not match." << endl;
    }

    return 0;
}