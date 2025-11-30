#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <numeric>
#include <limits>

using namespace std;

// Represents a memory block (partition)
struct Partition {
    int id;
    int size;
    bool is_occupied;
    int process_id;
};

// Represents a process requesting memory
struct Process {
    int id;
    int size;
    // We keep track of allocation state externally for this simulation to reuse the Process list
};

// --- Helper Function to print allocation status ---
void print_status(const vector<Partition>& partitions, const string& strategy) {
    cout << "\n===============================================" << endl;
    cout << "--- " << strategy << " Results ---" << endl;
    cout << "===============================================" << endl;
    cout << left << setw(10) << "Block ID" 
         << setw(13) << "Block Size" 
         << setw(10) << "Status" 
         << setw(10) << "Process ID" << endl;
    cout << setfill('-') << setw(53) << "" << setfill(' ') << endl;
    
    for (const auto& p : partitions) {
        cout << left << setw(10) << p.id 
             << setw(13) << to_string(p.size) + " KB" 
             << setw(10) << (p.is_occupied ? "USED" : "FREE") 
             << setw(10) << (p.is_occupied ? to_string(p.process_id) : "-") << endl;
    }
}

// --- 1. First-Fit Strategy ---
void first_fit(vector<Partition> partitions, const vector<Process>& processes) {
    int allocated_count = 0;

    for (const Process& proc : processes) {
        for (int i = 0; i < partitions.size(); ++i) {
            Partition& block = partitions[i];
            
            // Check 1: Is the block free?
            // Check 2: Is the block large enough?
            if (!block.is_occupied && block.size >= proc.size) {
                // Allocate the first suitable block found
                block.is_occupied = true;
                block.process_id = proc.id;
                allocated_count++;
                
                // Allocation is done for this process, move to the next process
                break; 
            }
        }
    }
    
    print_status(partitions, "First-Fit Allocation");
    cout << "Total Processes Allocated: " << allocated_count << " out of " << processes.size() << endl;
}

// --- 2. Best-Fit Strategy ---
void best_fit(vector<Partition> partitions, const vector<Process>& processes) {
    int allocated_count = 0;

    for (const Process& proc : processes) {
        int best_block_index = -1;
        // Start with the largest possible waste value
        int min_waste = numeric_limits<int>::max(); 

        for (int i = 0; i < partitions.size(); ++i) {
            Partition& block = partitions[i];
            
            // Check if the block is free AND big enough
            if (!block.is_occupied && block.size >= proc.size) {
                int waste = block.size - proc.size;
                
                // Find the block that leaves the smallest internal fragmentation (waste)
                if (waste < min_waste) {
                    min_waste = waste;
                    best_block_index = i;
                }
            }
        }

        if (best_block_index != -1) {
            // Allocate the block that is the "closest fit"
            partitions[best_block_index].is_occupied = true;
            partitions[best_block_index].process_id = proc.id;
            allocated_count++;
        }
    }
    
    print_status(partitions, "Best-Fit Allocation");
    cout << "Total Processes Allocated: " << allocated_count << " out of " << processes.size() << endl;
}

// --- 3. Worst-Fit Strategy ---
void worst_fit(vector<Partition> partitions, const vector<Process>& processes) {
    int allocated_count = 0;

    for (const Process& proc : processes) {
        int worst_block_index = -1;
        // Start with a small negative value
        int max_waste = -1; 

        for (int i = 0; i < partitions.size(); ++i) {
            Partition& block = partitions[i];
            
            // Check if the block is free AND big enough
            if (!block.is_occupied && block.size >= proc.size) {
                int waste = block.size - proc.size;
                
                // Find the block that leaves the largest internal fragmentation (waste)
                if (waste > max_waste) {
                    max_waste = waste;
                    worst_block_index = i;
                }
            }
        }

        if (worst_block_index != -1) {
            // Allocate the block that is the "worst fit" (largest block)
            partitions[worst_block_index].is_occupied = true;
            partitions[worst_block_index].process_id = proc.id;
            allocated_count++;
        }
    }
    
    print_status(partitions, "Worst-Fit Allocation");
    cout << "Total Processes Allocated: " << allocated_count << " out of " << processes.size() << endl;
}


int main() {
    // Initial memory blocks (partitions)
    vector<Partition> initial_partitions = {
        {1, 100, false, -1}, // Block 1: 100 KB
        {2, 500, false, -1}, // Block 2: 500 KB
        {3, 200, false, -1}, // Block 3: 200 KB
        {4, 300, false, -1}, // Block 4: 300 KB
        {5, 600, false, -1}  // Block 5: 600 KB
    };
    
    // Processes requesting memory (KB)
    vector<Process> processes_to_allocate = {
        {101, 212}, // P1: Needs 212 KB
        {102, 417}, // P2: Needs 417 KB
        {103, 112}, // P3: Needs 112 KB
        {104, 400}  // P4: Needs 400 KB
    };

    cout << "--- Memory Allocation Simulation Start ---" << endl;
    cout << "Initial Memory Blocks (KB): 100, 500, 200, 300, 600" << endl;
    cout << "Processes (KB): P1 (212), P2 (417), P3 (112), P4 (400)" << endl;
    

    // Run all three strategies sequentially, passing a fresh copy of partitions each time
    first_fit(initial_partitions, processes_to_allocate);
    best_fit(initial_partitions, processes_to_allocate);
    worst_fit(initial_partitions, processes_to_allocate);

    return 0;
}