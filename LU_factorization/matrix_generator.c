#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

int main(int argc, char *argv[])
{
    int matrix_size = 0;
    FILE *fp;
    if (argc != 3)
    {
        printf("usage: ./genfile matrix_size filename\n");
        exit(1);
    }
    if ((fp = fopen(argv[2], "w")) == NULL)
    {
        printf("Cannot write the file %s\n", argv[3]);
        exit(1);
    }

    matrix_size = atoi(argv[1]);

    fprintf(fp,"%d ", matrix_size);

    for(int i = 0; i < matrix_size; i++) {
        for(int j = 0; j < matrix_size; j++) {
            fprintf(fp,"%d ", rand()%1000);
        }
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
    fclose(fp);
}