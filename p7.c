#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define MAX_REQUESTS 100

void sort_array(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}
int calculate_fcfs(int reqs[], int count) {
    int total_seek = 0;
    for (int i = 1; i < count; i++) {
        total_seek += abs(reqs[i] - reqs[i - 1]);
    }
    return total_seek;
}

int calculate_sstf(int reqs[], int count) {
    int total_seek = 0;
    int current_pos = reqs[0];
    int visited[MAX_REQUESTS] = {0};
    visited[0] = 1;

    for (int i = 1; i < count; i++) {
        int min_distance = INT_MAX;
        int next_index = -1;

        for (int j = 1; j < count; j++) {
            if (visited[j] == 0) {
                int distance = abs(current_pos - reqs[j]);
                if (distance < min_distance) {
                    min_distance = distance;
                    next_index = j;
                }
            }
        }

        visited[next_index] = 1;
        total_seek += min_distance;
        current_pos = reqs[next_index];
    }
    return total_seek;
}

int calculate_look(int reqs[], int count) {
    int total_seek = 0;
    int start_pos = reqs[0];
    int sorted_reqs[MAX_REQUESTS];
    int num_reqs = count - 1;

    for (int i = 0; i < num_reqs; i++) {
        sorted_reqs[i] = reqs[i + 1];
    }

    sort_array(sorted_reqs, num_reqs);

    int split_index = 0;
    while (split_index < num_reqs && sorted_reqs[split_index] < start_pos) {
        split_index++;
    }

    int current_pos = start_pos;
    
    for (int i = split_index; i < num_reqs; i++) {
        total_seek += abs(sorted_reqs[i] - current_pos);
        current_pos = sorted_reqs[i];
    }
    
    for (int i = split_index - 1; i >= 0; i--) {
        total_seek += abs(sorted_reqs[i] - current_pos);
        current_pos = sorted_reqs[i];
    }
    
    return total_seek;
}

int calculate_clook(int reqs[], int count) {
    int total_seek = 0;
    int start_pos = reqs[0];
    int sorted_reqs[MAX_REQUESTS];
    int num_reqs = count - 1;

    for (int i = 0; i < num_reqs; i++) {
        sorted_reqs[i] = reqs[i + 1];
    }

    sort_array(sorted_reqs, num_reqs);

    int split_index = 0;
    while (split_index < num_reqs && sorted_reqs[split_index] < start_pos) {
        split_index++;
    }

    int current_pos = start_pos;
    
    for (int i = split_index; i < num_reqs; i++) {
        total_seek += abs(sorted_reqs[i] - current_pos);
        current_pos = sorted_reqs[i];
    }
    
    if (split_index > 0) {
        for (int i = 0; i < split_index; i++) {
            total_seek += abs(sorted_reqs[i] - current_pos);
            current_pos = sorted_reqs[i];
        }
    }
    
    return total_seek;
}

int main() {
    int requests[MAX_REQUESTS];
    int count = 0;
    int temp;

    while (count < MAX_REQUESTS && scanf("%d", &temp) == 1) {
        requests[count] = temp;
        count++;
    }

    if (count == 0) {
        printf("Error: No data read.\n");
        return -1;
    }

    printf("Assignment 7: Block Access Algorithm\n");
    printf("By: Drew Howard\n\n");
    printf("FCFS Total Seek: %d\n", calculate_fcfs(requests, count));
    printf("SSTF Total Seek: %d\n", calculate_sstf(requests, count));
    printf("LOOK Total Seek: %d\n", calculate_look(requests, count));
    printf("C-LOOK Total Seek: %d\n", calculate_clook(requests, count));

    return 0;
}