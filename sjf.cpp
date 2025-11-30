#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <limits> // Used for numeric_limits<int>::max()

using namespace std;

// Simplified structure for the process
struct Job {
    int id;
    int arrival;
    int burst;
    int waiting_time;
    int turnaround_time;
    bool is_completed = false; // Flag to track completion
};

void run_simple_sjf(vector<Job>& jobs) {
    int n = jobs.size();
    int current_time = 0;
    int completed_count = 0;
    double total_waiting = 0;
    double total_turnaround = 0;

    // We process jobs based on arrival AND burst time
    while (completed_count < n) {
        int shortest_burst = numeric_limits<int>::max();
        int shortest_job_index = -1;

        // 1. Scan for the best job: Find the shortest job that has arrived
        for (int i = 0; i < n; ++i) {
            if (jobs[i].arrival <= current_time && !jobs[i].is_completed) {
                if (jobs[i].burst < shortest_burst) {
                    shortest_burst = jobs[i].burst;
                    shortest_job_index = i;
                }
            }
        }

        if (shortest_job_index != -1) {
            // A job is ready to run (Shortest Job First)
            Job& j = jobs[shortest_job_index];
            
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
    cout << "--- Non-Preemptive SJF Results ---" << endl;
    cout << "ID | Arrival | Burst | Waiting | Turnaround" << endl;
    cout << "------------------------------------------" << endl;
    
    for (const auto& j : jobs) {
        cout << setw(2) << j.id << " |" 
             << setw(7) << j.arrival << " |" 
             << setw(5) << j.burst << " |"
             << setw(7) << j.waiting_time << " |" 
             << setw(10) << j.turnaround_time << endl;
    }

    cout << "\nAverage Waiting Time:    " << fixed << setprecision(2) << total_waiting / n << endl;
    cout << "Average Turnaround Time: " << fixed << setprecision(2) << total_turnaround / n << endl;
}

int main() {
    // Example Jobs: (ID, Arrival Time, Burst Time)
    // The scheduler should run P1, then P2, then P3, then P4.
    vector<Job> jobs = {
        {1, 0, 7},
        {2, 2, 4},
        {3, 4, 1}, // This will run second because of the shortest burst time
        {4, 5, 4}
    };
    
    run_simple_sjf(jobs);
    
    return 0;
}