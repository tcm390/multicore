#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <sys/time.h>
#include <unistd.h>

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

int main(int argc, char *argv[])
{
    struct timeval start, end;
    int thread_number = 0;
    int chunk_size = 0;

    if (argc != 3)
    {
        printf("usage: <thread_number> <chunk_size>\n");
        exit(1);
    }

    thread_number = atoi(argv[1]);
    chunk_size = atoi(argv[2]);

    gettimeofday(&start, NULL);

    for_number = 10000 * thread_number;
    //##### test #######
    //int test_thread_array[forNumber];

    int i;
#pragma omp parallel num_threads(thread_number)
    {

#pragma omp for schedule(static, chunk_size)
        for (i = 0; i < for_number; i++)
        {
            //##### test #######
            //test_thread_array[i] = omp_get_thread_num();
            delay(delay_length);
        }
    }

    gettimeofday(&end, NULL);

    double parallel_time = ((double)((double)(end.tv_usec - start.tv_usec) / 1000000 +
                                     (double)(end.tv_sec - start.tv_sec)));

    gettimeofday(&start, NULL);

    for (i = 0; i < for_number; i++)
    {
        delay(delay_length);
    }

    gettimeofday(&end, NULL);

    double sequential_time = ((double)((double)(end.tv_usec - start.tv_usec) / 1000000 +
                                       (double)(end.tv_sec - start.tv_sec)));

    //##### test #######
    // for (i = 0; i < 100 * thread_number; i++)
    //     printf("%d\t", test_thread_array[i]);
    // printf("\n");

    printf("Parallel time: %lf\n", parallel_time);

    printf("Sequential time: %lf\n", sequential_time);

    printf("average overhead time: %lf\n", parallel_time - sequential_time / thread_number);

    return 0;
}
