/*9. Write a program to implement non-primitive priority-based
scheduling algorithm.*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <limits> // Used for numeric_limits<int>::max()

using namespace std;

// Structure includes priority field
struct Job {
    int id;
    int arrival;
    int burst;
    int priority;    // Lower number = Higher Priority (Common convention)
    int waiting_time;
    int turnaround_time;
    bool is_completed = false; // Flag to track completion
};

void run_simple_priority(vector<Job>& jobs) {
    int n = jobs.size();
    int current_time = 0;
    int completed_count = 0;
    double total_waiting = 0;
    double total_turnaround = 0;

    // We process jobs based on arrival AND priority
    while (completed_count < n) {
        int highest_priority = numeric_limits<int>::max();
        int highest_priority_index = -1;

        // 1. Scan for the best job: Find the job with the highest priority that has arrived
        for (int i = 0; i < n; ++i) {
            if (jobs[i].arrival <= current_time && !jobs[i].is_completed) {
                // Find the process with the smallest priority number
                if (jobs[i].priority < highest_priority) {
                    highest_priority = jobs[i].priority;
                    highest_priority_index = i;
                }
            }
        }

        if (highest_priority_index != -1) {
            // A job is ready to run (Highest Priority First)
            Job& j = jobs[highest_priority_index];
            
            // 2. Calculate Start Time (it starts now)
            int start_time = current_time; 
            
            // 3. Update Current Time (Completion Time)
            current_time = start_time + j.burst;
            j.is_completed = true;
            completed_count++;

            // 4. Calculate Metrics
            j.turnaround_time = current_time - j.arrival;
            j.waiting_time = j.turnaround_time - j.burst;
            
            total_turnaround += j.turnaround_time;
            total_waiting += j.waiting_time;

        } else {
            // If no process is ready (CPU is idle), advance time
            current_time++;
        }
    }

    // Sort the results back by ID for clean display
    sort(jobs.begin(), jobs.end(), [](const Job& a, const Job& b) {
        return a.id < b.id;
    });

    // Display Results
    cout << "--- Non-Preemptive Priority Results (Low Number = High Priority) ---" << endl;
    cout << "ID | Arrival | Burst | Pri | Waiting | Turnaround" << endl;
    cout << "-------------------------------------------------" << endl;
    
    for (const auto& j : jobs) {
        cout << setw(2) << j.id << " |" 
             << setw(7) << j.arrival << " |" 
             << setw(5) << j.burst << " |"
             << setw(3) << j.priority << " |"
             << setw(7) << j.waiting_time << " |" 
             << setw(10) << j.turnaround_time << endl;
    }

    cout << "\nAverage Waiting Time:    " << fixed << setprecision(2) << total_waiting / n << endl;
    cout << "Average Turnaround Time: " << fixed << setprecision(2) << total_turnaround / n << endl;
}

int main() {
    // Example Jobs: (ID, Arrival Time, Burst Time, Priority)
    // P2 has the highest priority (1) and should run first after P1, regardless of its burst time.
    vector<Job> jobs = {
        {1, 0, 10, 3}, // Priority 3
        {2, 1, 5, 1},  // Priority 1 (Highest)
        {3, 2, 2, 4},  // Priority 4 (Lowest)
        {4, 3, 3, 2}   // Priority 2
    };
    
    run_simple_priority(jobs);
    
    return 0;
}
