#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>

int for_number = 100000;
int delay_length = 10000;

int sum2 = 0;
int sum1 = 0;
int num_threads;
void delay(int delaylength)
{
    int i;
    float a = 0.;
    for (i = 0; i < delaylength; i++)
        a += i;
    if (a < 0)
        printf("%f \n", a);
}

//int count = 0;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *without_lock(void *td)
{

    for (int i = 0; i < for_number * num_threads; ++i)
    {
        sum2++;
        delay(10000);
    }
    pthread_exit(NULL);
}

void *with_lock(void *td)
{

    for (int i = 0; i < for_number; ++i)
    {
        pthread_mutex_lock(&lock);
        sum1++;
        pthread_mutex_unlock(&lock);
        delay(10000);
    }

    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    struct timeval start, end;
    if (argc != 2)
    {
        printf("usage: ./lock_pt num_of_threads \n");
        exit(1);
    }
    num_threads = atoi(argv[1]);

    gettimeofday(&start, NULL);
    pthread_t threads[num_threads];

    for (int i = 0; i < num_threads; ++i)
    {

        pthread_create(&(threads[i]), NULL, with_lock, NULL);
    }
    for (int i = 0; i < num_threads; ++i)
    {
        pthread_join(threads[i], NULL);
    }
    gettimeofday(&end, NULL);

    double time_with_lock = ((double)((double)(end.tv_usec - start.tv_usec) / 1000000 +
                                      (double)(end.tv_sec - start.tv_sec)));

    gettimeofday(&start, NULL);

    pthread_t single;

    pthread_create(&single, NULL, without_lock, NULL);
    pthread_join(single, NULL);

    gettimeofday(&end, NULL);

    double time_without_lock = ((double)((double)(end.tv_usec - start.tv_usec) / 1000000 +
                                         (double)(end.tv_sec - start.tv_sec)));

    //count = 0;

    printf("\nsum1:%d\tsum2:%d\n", sum1, sum2);
    printf("Parallel time: %lf\n", time_with_lock);

    printf("Sequential time: %lf\n", time_without_lock);

    printf("average overhead time: %lf\n", time_with_lock - time_without_lock / num_threads);
    return 0;
}
