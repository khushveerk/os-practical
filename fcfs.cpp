#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

// Simplified structure for the process
struct Job {
    int id;
    int arrival;
    int burst;
    int waiting_time;
    int turnaround_time;
};

void run_simple_fcfs(vector<Job>& jobs) {
    // FCFS rule: Sort by Arrival Time
    sort(jobs.begin(), jobs.end(), [](const Job& a, const Job& b) {
        return a.arrival < b.arrival;
    });

    int current_time = 0;
    double total_waiting = 0;
    double total_turnaround = 0;

    for (Job& j : jobs) {
        // 1. Calculate the time the job STARTS
        int start_time = max(current_time, j.arrival);

        // 2. Calculate Waiting Time
        // Waiting time is the time it was ready (start_time) minus when it arrived (j.arrival).
        j.waiting_time = start_time - j.arrival;
        total_waiting += j.waiting_time;

        // 3. Update Current Time (Completion Time)
        current_time = start_time + j.burst;

        // 4. Calculate Turnaround Time (Completion Time - Arrival Time)
        j.turnaround_time = current_time - j.arrival;
        total_turnaround += j.turnaround_time;
    }

    // Display Results
    cout << "--- Simple FCFS Results ---" << endl;
    cout << "ID | Arrival | Burst | Waiting | Turnaround" << endl;
    cout << "------------------------------------------" << endl;
    
    for (const auto& j : jobs) {
        cout << setw(2) << j.id << " |" 
             << setw(7) << j.arrival << " |" 
             << setw(5) << j.burst << " |"
             << setw(7) << j.waiting_time << " |" 
             << setw(10) << j.turnaround_time << endl;
    }

    // Display Averages
    int n = jobs.size();
    cout << "\nAverage Waiting Time:    " << fixed << setprecision(2) << total_waiting / n << endl;
    cout << "Average Turnaround Time: " << fixed << setprecision(2) << total_turnaround / n << endl;
}

int main() {
    // Example Jobs: (ID, Arrival Time, Burst Time)
    vector<Job> jobs = {
        {1, 0, 10},
        {2, 1, 5},
        {3, 2, 2}
    };
    
    run_simple_fcfs(jobs);
    
    return 0;
}