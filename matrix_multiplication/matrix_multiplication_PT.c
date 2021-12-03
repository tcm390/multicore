#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>
int input_size;
int threadNumber;
int remain;
int *work_start;
int *work_end;
int **first_matrix;
int **second_matrix;
int **result_matrix;
void *thread_multi(void *arg)
{
    int id = *(int *)arg;
    int start = work_start[id];
    int end = work_end[id];
    for (int i = start; i < end; i++)
    {
        for (int j = 0; j < input_size; j++)
        {
            for (int k = 0; k < input_size; k++)
            {
                result_matrix[i][j] += first_matrix[i][k] * second_matrix[k][j];
            }
        }
    }
}
int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("invalid input\n");
        printf("./<programname> <number of threads> <size of matrix> \n");
        exit(1);
    }
    input_size = atoi(argv[2]);
    threadNumber = atoi(argv[1]);
    first_matrix = (int **)malloc(input_size * sizeof(int *));
    second_matrix = (int **)malloc(input_size * sizeof(int *));
    result_matrix = (int **)malloc(input_size * sizeof(int *));
    for (int i = 0; i < input_size; i++)
    {
        first_matrix[i] = (int *)malloc(input_size * sizeof(int));
        second_matrix[i] = (int *)malloc(input_size * sizeof(int));
        result_matrix[i] = (int *)malloc(input_size * sizeof(int));
    }
    for (int i = 0; i < input_size; i++)
        for (int j = 0; j < input_size; j++)
            first_matrix[i][j] = rand() % 50;
    for (int i = 0; i < input_size; i++)
        for (int j = 0; j < input_size; j++)
            second_matrix[i][j] = rand() % 50;
    for (int i = 0; i < input_size; i++)
        for (int j = 0; j < input_size; j++)
            result_matrix[i][j] = 0;
    pthread_t threads[threadNumber];
    remain = input_size % threadNumber;
    int current_work = 0;
    work_start = (int *)malloc(threadNumber * sizeof(int));
    work_end = (int *)malloc(threadNumber * sizeof(int));
    for (int i = 0; i < threadNumber; i++)
    {
        if (i < remain)
        {
            work_start[i] = current_work;
            work_end[i] = current_work + input_size / threadNumber + 1;
            current_work = current_work + input_size / threadNumber + 1;
        }
        else
        {
            work_start[i] = current_work;
            work_end[i] = current_work + input_size / threadNumber;
            current_work = current_work + input_size / threadNumber;
        }
    }
    for (int i = 0; i < threadNumber; i++)
    {
        int *_th_id = calloc(1, sizeof(int));
        *_th_id = i;
        int rc = pthread_create(&threads[i], NULL, thread_multi, (void *)_th_id);
        if (rc != 0)
        {
            printf("Error occurred, thread could not be created, errno = %d\n", rc);
            exit(0);
        }
    }
    for (int i = 0; i < threadNumber; i++)
    {
        pthread_join(threads[i], NULL);
    }
    free(first_matrix);
    free(second_matrix);
    free(result_matrix);
    return 0;
}
