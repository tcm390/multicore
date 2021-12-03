#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <sys/time.h>
#include <unistd.h>

int for_number = 100000;
int delay_length = 10000;
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

    if (argc != 2)
    {
        printf("usage: ./lock_op thread_number\n");
        exit(1);
    }

    thread_number = atoi(argv[1]);

    gettimeofday(&start, NULL);

    int sum1 = 0;

#pragma omp parallel num_threads(thread_number)
    {
#pragma omp for
        for (int i = 0; i < for_number * thread_number; i++)
        {
#pragma omp atomic
            sum1++;
            delay(delay_length);
        }
    }

    gettimeofday(&end, NULL);

    double parallel_time = ((double)((double)(end.tv_usec - start.tv_usec) / 1000000 +
                                     (double)(end.tv_sec - start.tv_sec)));

    gettimeofday(&start, NULL);

    int sum2 = 0;
#pragma omp parallel num_threads(1)
    {
        for (int i = 0; i < for_number * thread_number; i++)
        {
            sum2++;
            delay(delay_length);
        }
    }

    gettimeofday(&end, NULL);

    double sequential_time = ((double)((double)(end.tv_usec - start.tv_usec) / 1000000 +
                                       (double)(end.tv_sec - start.tv_sec)));

    printf("\nsum1:%d\tsum2:%d\n", sum1, sum2);
    printf("Parallel time: %lf\n", parallel_time);

    printf("Sequential time: %lf\n", sequential_time);

    printf("average overhead time: %lf\n", parallel_time - sequential_time / thread_number);

    return 0;
}