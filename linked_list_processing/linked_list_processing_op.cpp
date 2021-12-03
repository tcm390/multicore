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
struct thread_data
{
    int tid;
    node *start;
    int step;
};

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("usage: ./linked_list num_nodes num_threads\n");
        printf("num_nodes: the number of nodes\n");
        printf("num_threads: the number of threads\n");
        exit(1);
    }
    int n = strtol(argv[1], NULL, 10);
    int num_threads = strtol(argv[2], NULL, 10);
    node *head = new node();
    node *p = head;
    for (int i = 1; i <= n; ++i)
    {
        p->data = i;
        if (i < n)
        {
            p->next = new node();
        }
        p = p->next;
    }
    timeval start, end;
    gettimeofday(&start, NULL);

    thread_data td[num_threads];
    int step = n / num_threads;
    p = head;
    for (int i = 0; i < num_threads; ++i)
    {
        td[i].tid = i;
        td[i].step = i < n % num_threads ? step + 1 : step;
        td[i].start = p;
        for (int j = 0; j < td[i].step; ++j)
        {
            p = p->next;
        }
    }
#pragma omp parallel for num_threads(num_threads)
    for (int j = 0; j < num_threads; j++)
    {
        node *ptr = td[j].start;

        for (int i = 0; i < td[j].step; i++)
        {
            for (int k = 0; k < 1000; ++k)
            {
                cos(ptr->data);
            }
            //printf("processing node(%d)--thread(%d)\n", ptr->data, omp_get_thread_num());
            ptr = ptr->next;
        }
    }
    gettimeofday(&end, NULL);
    double time_spent = ((double)((double)(end.tv_usec - start.tv_usec) / 1000000 +
                                  (double)(end.tv_sec - start.tv_sec)));
    printf("Time taken for execution: %f seconds\n", time_spent);
    return 0;
}