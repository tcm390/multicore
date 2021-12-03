#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <sys/time.h>
#define TASK_SIZE 100
void Merge(int a[], int sizea, int b[], int sizeb, int ans[], int sizeans)
{
    int aptr = 0;
    int bptr = 0;
    for (int i = 0; i < sizeans; i++)
    {
        if (aptr >= sizea)
        {
            ans[i] = b[bptr];
            bptr++;
        }
        else if (bptr >= sizeb)
        {
            ans[i] = a[aptr];
            aptr++;
        }
        else if (aptr < sizea && bptr < sizeb)
        {
            if (a[aptr] <= b[bptr])
            {
                ans[i] = a[aptr];
                aptr++;
            }
            else
            {
                ans[i] = b[bptr];
                bptr++;
            }
        }
    }
}
void Mergesort(int a[], int size)
{
    if (size <= 1)
        return;
    int *left;
    left = (int *)malloc(size / 2 * sizeof(int));
    for (int i = 0; i < size / 2; i++)
        left[i] = a[i];
#pragma omp task shared(a) if (size > TASK_SIZE)
    Mergesort(left, size / 2);
    int size2 = size;
    if (size % 2 != 0)
        size2 += 1;
    int *right;
    right = (int *)malloc(size2 / 2 * sizeof(int));
    int j = 0;
    for (int i = size / 2; i < size; i++)
        right[j++] = a[i];
#pragma omp task shared(a) if (size > TASK_SIZE)
    Mergesort(right, size2 / 2);
#pragma omp taskwait
    Merge(left, size / 2, right, size2 / 2, a, size);
}
int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("invalid input\n");
        printf("./<programname> <number of threads> <number of elements> \n");
        exit(1);
    }
    int threadNumber = atoi(argv[1]);
    int input_size = atoi(argv[2]);
    int *input_array = malloc(input_size * sizeof(int));
    for (int i = 0; i < input_size; i++)
    {
        input_array[i] = rand() % 50;
    }
    omp_set_dynamic(0);
#pragma omp parallel num_threads(threadNumber)
    {
#pragma omp single
        Mergesort(input_array, input_size);
    }
    free(input_array);
    return 0;
}
