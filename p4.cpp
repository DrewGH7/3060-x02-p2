#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>

using namespace std;

struct Process {
    int id;
    int arrival;
    int burst;
    int remaining;
    int first_start;
    int finish;
};

void reset_processes(Process p[], int n) {
    for (int i = 0; i < n; i++) {
        p[i].remaining = p[i].burst;
        p[i].first_start = -1;
        p[i].finish = 0;
    }
}

void print_stats(Process p[], int n) {
    double total_resp = 0, total_ta = 0, total_wait = 0;
    for (int i = 0; i < n; i++) {
        int ta = p[i].finish - p[i].arrival;
        int wait = ta - p[i].burst;
        int resp = p[i].first_start - p[i].arrival;
        total_ta += ta;
        total_wait += wait;
        total_resp += resp;
    }
    cout << fixed << setprecision(2);
    cout << "Avg. Resp.:" << total_resp / n << ", Avg. T.A.:" << total_ta / n << ", Avg. Wait:" << total_wait / n << endl << endl;
}

int main(int argc, char* argv[]) {
    int quantum = 100;
    if (argc > 1) {
        quantum = atoi(argv[1]);
    }

    Process p[100];
    int n = 0;
    int a, b;
    while (cin >> a >> b) {
        p[n].id = n;
        p[n].arrival = a;
        p[n].burst = b;
        n++;
    }

    cout << "First Come, First Served" << endl;
    reset_processes(p, n);
    int clock = 0;
    for (int i = 0; i < n; i++) {
        if (clock < p[i].arrival) clock = p[i].arrival;
        p[i].first_start = clock;
        clock += p[i].burst;
        p[i].finish = clock;
    }
    print_stats(p, n);

    cout << "Shortest Job First" << endl;
    reset_processes(p, n);
    clock = 0;
    int completed = 0;
    bool done[100] = {false};
    while (completed < n) {
        int idx = -1;
        int min_burst = 999999;
        for (int i = 0; i < n; i++) {
            if (p[i].arrival <= clock && !done[i]) {
                if (p[i].burst < min_burst) {
                    min_burst = p[i].burst;
                    idx = i;
                }
            }
        }
        if (idx != -1) {
            p[idx].first_start = clock;
            clock += p[idx].burst;
            p[idx].finish = clock;
            done[idx] = true;
            completed++;
        } else {
            clock++;
        }
    }
    print_stats(p, n);

    cout << "Shortest Remaining Time First" << endl;
    reset_processes(p, n);
    clock = 0;
    completed = 0;
    int last_idx = -1;
    while (completed < n) {
        int idx = -1;
        int min_rem = 999999;
        for (int i = 0; i < n; i++) {
            if (p[i].arrival <= clock && p[i].remaining > 0) {
                if (p[i].remaining < min_rem) {
                    min_rem = p[i].remaining;
                    idx = i;
                }
            }
        }
        if (idx != -1) {
            if (p[idx].first_start == -1) p[idx].first_start = clock;
            p[idx].remaining--;
            clock++;
            if (p[idx].remaining == 0) {
                p[idx].finish = clock;
                completed++;
            }
        } else {
            clock++;
        }
    }
    print_stats(p, n);

    cout << "Round Robin with Time Quantum of " << quantum << endl;
    reset_processes(p, n);
    clock = 0;
    completed = 0;
    int queue[1000];
    int head = 0, tail = 0;
    bool in_queue[100] = {false};
    
    int added = 0;
    while (completed < n) {
        for(int i=0; i<n; i++) {
            if (p[i].arrival <= clock && !in_queue[i] && p[i].remaining > 0) {
                queue[tail++] = i;
                in_queue[i] = true;
                added++;
            }
        }

        if (head != tail) {
            int idx = queue[head++];
            if (p[idx].first_start == -1) p[idx].first_start = clock;
            
            int run_time = (p[idx].remaining < quantum) ? p[idx].remaining : quantum;
            
            for(int t=0; t<run_time; t++) {
                clock++;
                for(int i=0; i<n; i++) {
                    if (p[i].arrival == clock && !in_queue[i]) {
                        queue[tail++] = i;
                        in_queue[i] = true;
                    }
                }
            }
            
            p[idx].remaining -= run_time;
            if (p[idx].remaining == 0) {
                p[idx].finish = clock;
                completed++;
            } else {
                queue[tail++] = idx;
            }
        } else {
            clock++;
        }
    }
    print_stats(p, n);

    return 0;
}