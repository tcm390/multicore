#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>
#include <math.h>
void delay(int delaylength) {
	 int i; 
	 float a = 0.;
	 for (i = 0; i < delaylength; i++) 
	 	a += i; 
	 if (a < 0) printf("%f \n", a);
}
int input_size;
int threadNumber;
int *a;
int *b;
int *c;
int remain;
int *work_start;
int *work_end;
void *process_array(void *arg)
{
    int id = *(int *)arg;
    int start = work_start[id];
    int end = work_end[id];
    for (int i = start; i < end; i++)
    {
    	delay(10000);
        a[i] = b[i] * c[i];
    }
}
int main(int argc, char **argv)
{
    if (argc != 3)
    {
        printf("invalid input\n");
        printf("./<programname> <number of threads> <size of matrix> \n");
        exit(1);
    }
    input_size = atoi(argv[2]);
    threadNumber = atoi(argv[1]);
    a = (int *)malloc(input_size * sizeof(int));
    b = (int *)malloc(input_size * sizeof(int));
    c = (int *)malloc(input_size * sizeof(int));
    for (int i = 0; i < input_size; i++)
        b[i] = i % 100;
    for (int i = 0; i < input_size; i++)
        c[i] = i % 100;
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
    pthread_t threads[threadNumber];
    for (int i = 0; i < threadNumber; i++)
    {
        int *_th_id = (int *)calloc(1, sizeof(int));
        *_th_id = i;
        int rc = pthread_create(&threads[i], NULL, process_array, (void *)_th_id);
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
    free(a);
    free(b);
    free(c);
    return 0;
}
