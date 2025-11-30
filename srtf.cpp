/*10. Write a program to implement SRTF scheduling algorithm*/

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct Process {
    int pid;
    int burst_time;
    int remaining_time;
    int arrival_time;
    int start_time;
    int completion_time;
    int waiting_time;
    int turnaround_time;

    Process(int id, int burst, int arrival)
        : pid(id),
          burst_time(burst),
          remaining_time(burst),
          arrival_time(arrival),
          start_time(-1),
          completion_time(0),
          waiting_time(0),
          turnaround_time(0) {}
};

void SRTFScheduling(vector<Process> &p) {
    int n = p.size();
    int current_time = 0;
    int completed = 0;

    // sort by arrival time
    sort(p.begin(), p.end(),
         [](const Process &a, const Process &b) {
             return a.arrival_time < b.arrival_time;
         });

    vector<bool> done(n, false);

    while (completed < n) {
        int idx = -1;
        int min_rem = 1e9;

        // pick arrived process with smallest remaining time
        for (int i = 0; i < n; ++i) {
            if (!done[i] &&
                p[i].arrival_time <= current_time &&
                p[i].remaining_time < min_rem &&
                p[i].remaining_time > 0) {
                min_rem = p[i].remaining_time;
                idx = i;
            }
        }

        if (idx == -1) {
            // no process ready, CPU idle
            current_time++;
            continue;
        }

        // start time for first execution
        if (p[idx].start_time == -1)
            p[idx].start_time = current_time;

        // execute 1 time unit (preemptive)
        p[idx].remaining_time--;
        current_time++;

        // if finished
        if (p[idx].remaining_time == 0) {
            done[idx] = true;
            completed++;

            p[idx].completion_time = current_time;
            p[idx].turnaround_time =
                p[idx].completion_time - p[idx].arrival_time;
            p[idx].waiting_time =
                p[idx].turnaround_time - p[idx].burst_time;
        }
    }

    // print table
    cout << "PID\tBurst\tArrival\tCompletion\tWaiting\tTurnaround\n";
    int total_wt = 0, total_tat = 0;
    for (const auto &pr : p) {
        cout << pr.pid << "\t"
             << pr.burst_time << "\t"
             << pr.arrival_time << "\t"
             << pr.completion_time << "\t\t"
             << pr.waiting_time << "\t"
             << pr.turnaround_time << "\n";
        total_wt += pr.waiting_time;
        total_tat += pr.turnaround_time;
    }
    cout << "Average Waiting Time: "
         << (float)total_wt / n << "\n";
    cout << "Average Turnaround Time: "
         << (float)total_tat / n << "\n";
}

int main() {
    // example set (PID, Burst, Arrival)
    vector<Process> procs = {
        Process(1, 6, 0),
        Process(2, 8, 1),
        Process(3, 7, 2),
        Process(4, 3, 3)
    };

    SRTFScheduling(procs);
    return 0;
}
