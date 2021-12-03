#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <sys/time.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    struct timeval start, end;

    int t_count = 0;

    if (argc != 2)
    {
        printf("usage: ./genfile number_of_threads \n");
        exit(1);
    }
    t_count = atoi(argv[1]);

    gettimeofday(&start, NULL);

#pragma omp parallel num_threads(t_count)
    {
    }

    gettimeofday(&end, NULL);

    double parallel_time = ((double)((double)(end.tv_usec - start.tv_usec) / 1000000 +
                                     (double)(end.tv_sec - start.tv_sec)));

    printf("%d threads overhead time: %lf\n", t_count, parallel_time);

    return 0;
}
