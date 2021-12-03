#include <stdio.h>
#include "pthread.h"
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>

struct node {
    int data;
    node *next;
};

struct thread_data {
    int tid;
    node *start;
    int step;
};

node *head = NULL;

void *process(void *thread_args) {
    thread_data *curr = (thread_data *) thread_args;
    int step = curr->step;
    node *p = curr->start;
    for (int i = 0; i < step; ++i) {
        for (int j = 0; j < 1000; ++j) {
            cos(p->data);
        }
        p = p->next;
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if(argc != 3){
        printf("usage: ./linked_list num_nodes num_threads\n");
        printf("num_nodes: the number of nodes\n");
        printf("num_threads: the number of threads\n");
        exit(1);
    }
    int n = strtol(argv[1], NULL, 10);
    int num_threads = strtol(argv[2], NULL, 10);
    head = new node();
    node *p = head;
    for (int i = 1; i <= n; ++i) {
        p->data = i;
        if (i < n) {
            p->next = new node();
        }
        p = p->next;
    }
    timeval start, end;
    gettimeofday(&start, NULL);
    pthread_t threads[num_threads];
    thread_data td[num_threads];
    int step = n / num_threads;
    p = head;
    for (int i = 0; i < num_threads; ++i) {
        td[i].tid = i;
        td[i].step = i == num_threads - 1 ? n - step * (num_threads - 1) : step;
        td[i].start = p;
        for (int j = 0; j < td[i].step; ++j) {
            p = p->next;
        }
    }
    for (int i = 0; i < num_threads; ++i) {
        int rc = pthread_create(&threads[i], NULL, process, (void *)&td[i]);
        if (rc) {
            printf("Error:unable to create thread, %d\n", rc);
            exit(-1);
        }
    }
    for (int i = 0; i < num_threads; ++i) {
        pthread_join(threads[i], NULL);
    }
    gettimeofday(&end, NULL);
    double time_spent = ((double)((double)(end.tv_usec - start.tv_usec) / 1000000 +
                           (double)(end.tv_sec - start.tv_sec)));
    printf("Time taken for execution: %f seconds\n", time_spent);
    return 0;
}
