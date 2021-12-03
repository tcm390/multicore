#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
void delay(int delaylength) {
	 int i; 
	 float a = 0.;
	 for (i = 0; i < delaylength; i++) 
	 	a += i; 
	 if (a < 0) printf("%f \n", a);
}
int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("invalid input\n");
        printf("./<programname> <number of threads> <size of matrix> \n");
        exit(1);
    }
    int input_size = atoi(argv[1]);
    int *a = (int *)malloc(input_size * sizeof(int));
    int *b = (int *)malloc(input_size * sizeof(int));
    int *c = (int *)malloc(input_size * sizeof(int));
    for (int i = 0; i < input_size; i++)
        b[i] = i % 100;
    for (int i = 0; i < input_size; i++)
        c[i] = i % 100;
    for (int i = 0; i < input_size; i++)
    {
    	delay(10000);
        a[i] = b[i] * c[i];
    }
    free(a);
    free(b);
    free(c);
    return 0;
}
