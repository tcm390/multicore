#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <omp.h>
int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("invalid input\n");
        printf("./<programname> <number of threads> <size of matrix> \n");
        exit(1);
    }
    int input_size = atoi(argv[2]);
    int threadNumber = atoi(argv[1]);
    int **first_matrix = (int **)malloc(input_size * sizeof(int *));
    int **second_matrix = (int **)malloc(input_size * sizeof(int *));
    int **result_matrix = (int **)malloc(input_size * sizeof(int *));
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
    omp_set_dynamic(0);
    int i, j, k;
#pragma omp parallel num_threads(threadNumber) shared(first_matrix, second_matrix, result_matrix) private(i, j, k)
    {
#pragma omp for
        for (i = 0; i < input_size; i++)
        {
            for (j = 0; j < input_size; j++)
            {
                for (k = 0; k < input_size; k++)
                {
                    result_matrix[i][j] += first_matrix[i][k] * second_matrix[k][j];
                }
            }
        }
    }
    free(first_matrix);
    free(second_matrix);
    free(result_matrix);
    return 0;
}