#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_ITEMS 10

typedef struct {
    int number;     
    int *factors;    
    int factor_count;    
} FactorData;

int buffer_in[MAX_ITEMS];
int in_count = 0;        
int in_prod_idx = 0;     
int in_con_idx = 0;     

pthread_mutex_t mutex_in = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_in_full = PTHREAD_COND_INITIALIZER; 
pthread_cond_t cond_in_empty = PTHREAD_COND_INITIALIZER; 

FactorData buffer_out[MAX_ITEMS];
int out_count = 0;     
int out_prod_idx = 0;    
int out_con_idx = 0;    

pthread_mutex_t mutex_out = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_out_full = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_out_empty = PTHREAD_COND_INITIALIZER;

void put_input(int item) {
    pthread_mutex_lock(&mutex_in);
    
    while (in_count == MAX_ITEMS) {
        pthread_cond_wait(&cond_in_full, &mutex_in);
    }
    
    buffer_in[in_prod_idx] = item;
    in_prod_idx = (in_prod_idx + 1) % MAX_ITEMS;
    in_count++;
    
    pthread_cond_signal(&cond_in_empty);
    pthread_mutex_unlock(&mutex_in);
}

int get_input() {
    pthread_mutex_lock(&mutex_in);
    
    while (in_count == 0) {
        pthread_cond_wait(&cond_in_empty, &mutex_in);
    }
    
    int item = buffer_in[in_con_idx];
    in_con_idx = (in_con_idx + 1) % MAX_ITEMS;
    in_count--;
    
    pthread_cond_signal(&cond_in_full);
    pthread_mutex_unlock(&mutex_in);
    
    return item;
}

void put_output(FactorData item) {
    pthread_mutex_lock(&mutex_out);
    
    while (out_count == MAX_ITEMS) {
        pthread_cond_wait(&cond_out_full, &mutex_out);
    }
    
    buffer_out[out_prod_idx] = item;
    out_prod_idx = (out_prod_idx + 1) % MAX_ITEMS;
    out_count++;
    
    pthread_cond_signal(&cond_out_empty);
    pthread_mutex_unlock(&mutex_out);
}

FactorData get_output() {
    pthread_mutex_lock(&mutex_out);
    
    while (out_count == 0) {
        pthread_cond_wait(&cond_out_empty, &mutex_out);
    }
    
    FactorData item = buffer_out[out_con_idx];
    out_con_idx = (out_con_idx + 1) % MAX_ITEMS;
    out_count--;
    
    pthread_cond_signal(&cond_out_full);
    pthread_mutex_unlock(&mutex_out);
    
    return item;
}

void *producer(void *arg) {
    while (1) {
        int num = get_input();

        if (num == 0) {
            FactorData sentinel;
            sentinel.number = 0;
            sentinel.factors = NULL;
            sentinel.factor_count = 0;
            put_output(sentinel);
            break; 
        }

        int *factors = malloc(sizeof(int) * num); 
        int count = 0;
        
        for (int i = 1; i <= num; i++) {
            if (num % i == 0) {
                factors[count++] = i;
            }
        }

        FactorData data;
        data.number = num;
        data.factors = factors;
        data.factor_count = count;
        
        put_output(data);
    }
    return NULL;
}

void *consumer(void *arg) {
    while (1) {
        FactorData data = get_output();

        if (data.number == 0) {
            break;
        }

        printf("%d: ", data.number);
        for (int i = 0; i < data.factor_count; i++) {
            printf("%d ", data.factors[i]);
        }
        printf("\n");

        free(data.factors);
    }
    return NULL;
}


// Main function starts here

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: ./p5 <number1> <number2> ...\n");
        return -1;
    }

    pthread_t prod_thread, con_thread;

    pthread_create(&prod_thread, NULL, producer, NULL);
    pthread_create(&con_thread, NULL, consumer, NULL);

    for (int i = 1; i < argc; i++) {
        int num = atoi(argv[i]); 
        put_input(num); 
    }

    put_input(0);

    pthread_join(prod_thread, NULL);
    pthread_join(con_thread, NULL);

    pthread_mutex_destroy(&mutex_in);
    pthread_cond_destroy(&cond_in_full);
    pthread_cond_destroy(&cond_in_empty);
    
    pthread_mutex_destroy(&mutex_out);
    pthread_cond_destroy(&cond_out_full);
    pthread_cond_destroy(&cond_out_empty);

    return 0;
}