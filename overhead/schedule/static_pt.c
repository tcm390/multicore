#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>

int thread_number;
int chunk_size;
int *test_thread_array;
int for_number;
int delay_length = 100000;

void delay(int delaylength)
{
    int i;
    float a = 0.;
    for (i = 0; i < delaylength; i++)
        a += i;
    if (a < 0)
        printf("%f \n", a);
}
void *proceess_thread(void *arg)
{

    int id = *(int *)arg;

    int start = id;

    for (int i = id * chunk_size; i < for_number; i += thread_number * chunk_size)
    {
        for (int j = i; j < i + chunk_size; j++)
        {
            //test_thread_array[i] = id;
            delay(delay_length);
        }
    }
}
int main(int argc, char *argv[])
{
    struct timeval start, end;

    int static_number = 0;

    if (argc != 3)
    {
        printf("usage: <thread_number> <chunk_size>\n");
        exit(1);
    }

    thread_number = atoi(argv[1]);
    chunk_size = atoi(argv[2]);
    for_number = 10000 * thread_number;
    gettimeofday(&start, NULL);
    pthread_t threads[thread_number];

    //test_thread_array = (int *)malloc(for_number * sizeof(int));
    for (int i = 0; i < thread_number; i++)
    {
        int *_th_id = calloc(1, sizeof(int));
        *_th_id = i;
        int rc = pthread_create(&threads[i], NULL, proceess_thread, (void *)_th_id);
        if (rc != 0)
        {
            printf("Error occurred, thread could not be created, errno = %d\n", rc);
            exit(0);
        }
    }
    for (int i = 0; i < thread_number; i++)
    {
        pthread_join(threads[i], NULL);
    }

    gettimeofday(&end, NULL);

    double parallel_time = ((double)((double)(end.tv_usec - start.tv_usec) / 1000000 +
                                     (double)(end.tv_sec - start.tv_sec)));

    gettimeofday(&start, NULL);

    for (int i = 0; i < for_number; i++)
    {
        delay(delay_length);
    }

    gettimeofday(&end, NULL);

    double sequential_time = ((double)((double)(end.tv_usec - start.tv_usec) / 1000000 +
                                       (double)(end.tv_sec - start.tv_sec)));

    //##### test #######
    // for (int i = 0; i < 100 * thread_number; i++)
    //     printf("%d\t", test_thread_array[i]);
    // printf("\n");

    printf("Parallel time: %lf\n", parallel_time);

    printf("Sequential time: %lf\n", sequential_time);

    printf("average overhead time: %lf\n", parallel_time - sequential_time / thread_number);

    return 0;
}
