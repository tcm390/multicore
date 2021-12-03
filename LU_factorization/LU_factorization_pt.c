#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
int num_of_thread;
int matrix_size;
double **matrix;
double **lower;
struct thread_arg
{
    int start;
    int end;
    int thread_id;
    int k;
};
void *calculate(void *arg)
{
    struct thread_arg *t_arg = (struct thread_arg *)arg;
    int start = t_arg->start;
    int end = t_arg->end;
    int k = t_arg->k;
    for (int j = start; j < end; j++) {
        lower[j][k] = matrix[j][k] / matrix[k][k];
        for(int i = k; i < matrix_size; i++) {
            matrix[j][i] = matrix[j][i] - lower[j][k] * matrix[k][i];
        }
    }
    pthread_exit(NULL);
}
int main(int argc, char *argv[])
{
    num_of_thread = 0;
    matrix_size = 0;
    FILE *fp;
    if (argc != 3)
    {
        printf("usage: ./genfile number_of_threads filename\n");
        exit(1);
    }
    num_of_thread = atoi(argv[1]);
    if ((fp = fopen(argv[2], "r")) == NULL)
    {
        printf("Cannot read the file %s\n", argv[3]);
        exit(1);
    }
    fscanf(fp, "%d", &matrix_size);
    matrix = (double **)malloc(sizeof(double *) * matrix_size);
    lower = (double **)malloc(sizeof(double *) * matrix_size);
    for (int i = 0; i < matrix_size; i++)
    {
        matrix[i] = (double *)malloc(sizeof(double) * matrix_size);
        lower[i] = (double *)malloc(sizeof(double) * matrix_size);
    }
    for (int i = 0; i < matrix_size; i++)
    {
        for (int j = 0; j < matrix_size; j++)
        {
            fscanf(fp, "%lf", &matrix[i][j]);
            if(i == j)
                lower[i][j] = 1;
            else
                lower[i][j] = 0;
        }
    }
    for (int k = 0; k < matrix_size; k++)
    {
        pthread_t cal_threads[num_of_thread];
        int step = (matrix_size - k - 1) / num_of_thread;
        int remain = (matrix_size - k - 1) % num_of_thread;
        int current_work = k+1;
        for (int i = 0; i < num_of_thread; i++)
        {
            struct thread_arg *t_args = (struct thread_arg *)malloc(sizeof(struct thread_arg));
            t_args->start = current_work;
            if(i < remain) {
                current_work =current_work + step + 1; 
            } else {
                current_work =current_work + step; 
            }
            t_args->end = current_work;
            t_args->k = k;
            pthread_create(&cal_threads[i], NULL, calculate, (void *)t_args);
        }
        for (int i = 0; i < num_of_thread; i++)
        {
            pthread_join(cal_threads[i], NULL);
        }
    }
    return 0;
}