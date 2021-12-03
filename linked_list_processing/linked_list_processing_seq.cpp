#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>

struct node
{
    int data;
    node *next;
};

void process(node *p) {
    if (!p) {
        return;
    }
    for (int i = 0; i < 1000; ++i) {
        cos(p->data);
    }
}

int main(int argc, char *argv[]) {
    if(argc != 2){
        printf("usage: ./linked_list num_nodes \n");
        printf("num_nodes: the number of nodes\n");
        exit(1);
    }
    int n = strtol(argv[1], NULL, 10);
    node *head = new node();
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
    
    p = head;
    while (p) {
        process(p);
        p = p->next;
    }
    
    gettimeofday(&end, NULL);
    double time_spent = ((double)((double)(end.tv_usec - start.tv_usec) / 1000000 +
                           (double)(end.tv_sec - start.tv_sec)));
    printf("Time taken for execution: %f seconds\n", time_spent);
    return 0;
}