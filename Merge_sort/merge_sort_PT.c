#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

int *input_array;
int threadNumber;
int input_size;
int OFFSET;

struct data
{
    int *array;
    int id;
    int size;
};

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

    Mergesort(left, size / 2);
    int size2 = size;
    if (size % 2 != 0)
        size2 += 1;
    int *right;
    right = (int *)malloc(size2 / 2 * sizeof(int));
    int j = 0;
    for (int i = size / 2; i < size; i++)
        right[j++] = a[i];

    Mergesort(right, size2 / 2);
    Merge(left, size / 2, right, size2 / 2, a, size);
}
void *assign_thread_work(void *arg)
{
    struct data *data = (struct data *)arg;
    int thread_id = data->id;
    int start = thread_id * (input_size / threadNumber);
    int end = (thread_id + 1) * (input_size / threadNumber) - 1;
    if (thread_id == threadNumber - 1)
    {
        end += OFFSET;
    }
    int size = end - start + 1;
    data->size = size;
    if (size <= 1)
        return NULL;
    int *left;
    left = (int *)malloc(size / 2 * sizeof(int));
    int k = 0;
    for (int i = start; i < start + size / 2; i++)
        left[k++] = input_array[i];
    Mergesort(left, size / 2);
    int size2 = size;
    if (size % 2 != 0)
        size2 += 1;
    int *right;
    right = (int *)malloc(size2 / 2 * sizeof(int));
    int j = 0;
    for (int i = start + size / 2; i < start + size; i++)
        right[j++] = input_array[i];
    Mergesort(right, size2 / 2);
    data->array = (int *)malloc(size * sizeof(int));
    Merge(left, size / 2, right, size2 / 2, data->array, size);
    return NULL;
}
int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("invalid input\n");
        printf("./<programname> <number of threads> <number of elements> \n");
        exit(1);
    }
    threadNumber = atoi(argv[1]);
    input_size = atoi(argv[2]);
    printf("\nthread number: %d\n\n", threadNumber);
    input_array = (int *)malloc(input_size * sizeof(int));
    OFFSET = input_size % threadNumber;
    for (int i = 0; i < input_size; i++)
    {
        input_array[i] = rand() % 50;
    }
    struct data thread_data[threadNumber];
    pthread_t threads[threadNumber];

    for (int i = 0; i < threadNumber; i++)
    {
        thread_data[i].id = i;
        int rc = pthread_create(&threads[i], NULL, assign_thread_work, &thread_data[i]);
        if (rc != 0)
        {
            printf("Error occurred, thread could not be created, errno = %d\n", rc);
            exit(-1);
        }
    }
    for (int i = 0; i < threadNumber; i++)
    {
        pthread_join(threads[i], NULL);
    }

    int index_of_local[threadNumber][1];
    for (int i = 0; i < threadNumber; i++)
        index_of_local[i][0] = 1;

    while (1)
    {
        int count = 0;
        for (int i = 0; i < threadNumber; i++)
        {
            if (index_of_local[i][0] == 1)
            {
                for (int j = i + 1; j < threadNumber; j++)
                {
                    if (index_of_local[j][0] == 1)
                    {
                        count++;
                        index_of_local[j][0] = 0;
                        int *temp = (int *)malloc(thread_data[i].size * sizeof(int));
                        int temp_size = thread_data[i].size;
                        thread_data[i].size = temp_size + thread_data[j].size;

                        for (int k = 0; k < temp_size; k++)
                        {
                            temp[k] = thread_data[i].array[k];
                        }
                        thread_data[i].array = (int *)malloc(thread_data[i].size * sizeof(int));
                        Merge(temp, temp_size, thread_data[j].array, thread_data[j].size, thread_data[i].array, thread_data[i].size);
                        free(temp);
                        break;
                    }
                }
            }
        }
        if (count == 0)
        {
            break;
        }
    }
    free(input_array);
    return 0;
}
