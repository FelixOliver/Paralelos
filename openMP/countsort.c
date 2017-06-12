#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void serial_count_sort(int* a,int n)
{
	int i,j,count;
	int* tmp = malloc(n*sizeof(int));
	for(i=0;i<n;i++)
	{
		count = 0;
		for(j=0;j<n;j++)
		{
			if(a[j] < a[i])
				count++;
			else if(a[j] == a[i] && j<i)
				count++;
		}
		tmp[count] = a[i];
	}

	memcpy(a,tmp,n*sizeof(int));
	free(tmp);
}

void parallel_count_sort(int* a,int n,int thread_count)
{
	int i,j,count;
	int* tmp = malloc(n*sizeof(int));
	
	#pragma omp parallel for num_threads(thread_count) \
	default(none) private(i, j, count) shared(a, n, tmp, thread_count)

	for (i = 0; i < n; i++) 
	{
        count = 0;
        for (j = 0; j < n; j++)
        {
            if (a[j] < a[i])
               count++;
            else if (a[j] == a[i] && j < i)
               count++;
       	}
        tmp[count] = a[i];
     }

	memcpy(a,tmp,n*sizeof(int));
	free(tmp);
}

void llenar_vector(int* a,int n)
{
	srand(time(NULL));
	
	for(int i=0;i<n;i++)
	{
		a[i] = rand()%20+1;
	}

}

void print_vector(int* a,int n)
{
	
	for(int i=0;i<n;i++)
	{
		printf("%d ",a[i]);
	}
	printf("\n");

}


int main(int argc,char* argv[])
{

	int thread_count = strtol(argv[1],NULL,10);
	printf("Numero de threads: %d\n\n",thread_count);

	int num_data = 10;
	int size = num_data*sizeof(int);
	int* vec1 = (int*) malloc(size);
	int* vec2 = (int*) malloc(size);
	
	llenar_vector(vec1,num_data);
	memcpy(vec2,vec1,size);
	
	printf("Serial count sort :\n");
	serial_count_sort(vec1,num_data);
	print_vector(vec1,num_data);
	printf("\n");

	printf("Parallel count sort :\n");
	parallel_count_sort(vec2,num_data,thread_count);
	print_vector(vec2,num_data);

	free(vec1);
	free(vec2);
	return 0;
}