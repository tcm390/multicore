#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
int main(int argc, char *argv[])
{
    int matrix_size = 0;
    FILE *fp;
    if (argc != 2)
    {
        printf("usage: ./genfile filename\n");
        exit(1);
    }
    if ((fp = fopen(argv[1], "r")) == NULL)
    {
        printf("Cannot read the file %s\n", argv[3]);
        exit(1);
    }
    fscanf(fp, "%d", &matrix_size);
    double **matrix = (double **)malloc(sizeof(double *) * matrix_size);
    double **lower = (double **)malloc(sizeof(double *) * matrix_size);
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
        }
    }
    for (int k = 0; k < matrix_size; k++)
    {
        lower[k][k] = 1;
        for (int j = k + 1; j < matrix_size; j++)
        {
            lower[j][k] = matrix[j][k] / matrix[k][k];
            for(int i = k; i < matrix_size; i++) {
                matrix[j][i] = matrix[j][i] - lower[j][k] * matrix[k][i];
            }
        }
    }
    return 0;
}