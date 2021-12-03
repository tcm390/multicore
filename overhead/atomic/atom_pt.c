#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdatomic.h>
#include <sys/time.h>

atomic_int acnt;
int cnt, num_threads;
int for_number = 100000;
int delay_length = 10000;
struct thread_data
{
    int tid;
};

void delay(int delaylength)
{
    int i;
    float a = 0.;
    for (i = 0; i < delaylength; i++)
        a += i;
    if (a < 0)
        printf("%f \n", a);
}

void *incr_non_atomic()
{
    for (int i = 0; i < for_number * num_threads; ++i)
    {
        ++cnt;
        delay(delay_length);
    }
    pthread_exit(NULL);
}

void *incr_atomic(void *td)
{
    //struct thread_data *curr = (struct thread_data *) td;
    for (int i = 0; i < for_number; ++i)
    {
        ++acnt;
        delay(delay_length);
    }
    //printf("acnt is: %d, thread %d\n", acnt, curr->tid);
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    struct timeval start, end;
    if (argc != 2)
    {
        printf("usage: ./atom_pt num_of_threads \n");
        exit(1);
    }
    num_threads = atoi(argv[1]);
    pthread_t threads[num_threads];
    struct thread_data td[num_threads];
    pthread_t single;

    gettimeofday(&start, NULL);

    for (int i = 0; i < num_threads; ++i)
    {
        td[i].tid = i;
        pthread_create(&(threads[i]), NULL, incr_atomic, &(td[i]));
    }
    for (int i = 0; i < num_threads; ++i)
    {
        pthread_join(threads[i], NULL);
    }

    gettimeofday(&end, NULL);
    double time_with_atomic = ((double)((double)(end.tv_usec - start.tv_usec) / 1000000 +
                                        (double)(end.tv_sec - start.tv_sec)));

    gettimeofday(&start, NULL);

    pthread_create(&single, NULL, incr_non_atomic, NULL);

    pthread_join(single, NULL);

    gettimeofday(&end, NULL);
    double seq_time = ((double)((double)(end.tv_usec - start.tv_usec) / 1000000 +
                                (double)(end.tv_sec - start.tv_sec)));

    //printf("The atomic counter is %u\n", acnt);
    //printf("The non-atomic counter is %u\n", cnt);
    printf("time without atomic (sequential) is %lf seconds\n", seq_time);
    printf("time with atomic is %lf seconds\n", time_with_atomic);
    printf("atomic overhead is: %lf seconds per thread\n", time_with_atomic - seq_time / num_threads);
    return 0;
}