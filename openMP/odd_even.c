#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

//gcc -o odd_even -fopenmp odd_even.c
//./odd_even 4



//lenado de vec
void llenar_vector(int *a, int n)
{
	srand(time(NULL));
	
	for(int i=0;i<n;i++) {
		a[i] = rand()%50;		
	}
}
//imprime vec
void print_vector(int *a, int n){
	
	for(int i=0;i<n;i++){
		printf("%d ",a[i]);
	}
	printf("-----------------\n");
}

// forma con pragma for parallel para cada for
void odd_even_1(int* a, int n,int num_thd)
{
  int phase, i, temp;
  for(phase=0;phase<n;++phase)
  {
  	if(phase%2==0) 
    {
    	#pragma omp parallel for num_threads(num_thd) default(none) shared(a,n) private(i,temp)
      	for(i=1;i<n;i+=2)
			if(a[i-1] > a[i])
			{
	  			temp = a[i];
	  			a[i] = a[i-1];
	  			a[i-1] = temp;
			}
    }
    else 
    {
		#pragma omp parallel for num_threads(num_thd) default(none) shared(a,n) private(i,temp)
      	for(i=1;i<n-1;i+=2)
			if(a[i] > a[i+1])
			{
      	  		temp = a[i];
	  			a[i] = a[i+1];
	  			a[i+1] = temp;
			}
    }
  }
}

void odd_even_2(int* a, int n,int num_thd)
{
  int phase, i, temp;
  #pragma omp parallel num_threads(num_thd) default(none) shared(a,n) private(i,temp,phase)
  for(phase=0;phase<n;++phase)
  {
    if(phase%2==0) 
    {
		#pragma omp for
      	for(i=1;i<n;i+=2)
			if(a[i-1] > a[i])
			{
	  			temp = a[i];
	  			a[i] = a[i-1];
	  			a[i-1] = temp;
			}
    }
    else 
    {
		#pragma omp for
      	for(i=1;i<n-1;i+=2)
			if(a[i] > a[i+1])
			{
      	  		temp = a[i];
	  			a[i] = a[i+1];
	  			a[i+1] = temp;
			}
    }
  }
}

int main(int argc,char* argv[])
{
	int thread_count = strtol(argv[1],NULL,10);
	printf("numero de threads: %d\n",thread_count);

	double ini_1, fin_1, rpt1;
 	double ini_2, fin_2, rpt2;

	int num_data = 10;
	int size = num_data*sizeof(int);

	int* vec_entrada = (int*) malloc(size);
	llenar_vector(vec_entrada,num_data);

	int* vec1 ,*vec2;
	vec1 = (int*) malloc(size);
	vec2 = (int*) malloc(size);

	memcpy(vec1,vec_entrada,size);
	memcpy(vec2,vec_entrada,size);
	print_vector(vec1,num_data);
	print_vector(vec2,num_data);

	ini_1 = omp_get_wtime();
  	odd_even_1(vec1,num_data,thread_count);
  	fin_1 = omp_get_wtime();
  	rpt1 = fin_1 - ini_1;

  	ini_2 = omp_get_wtime();
  	odd_even_2(vec2,num_data,thread_count);
  	fin_2 = omp_get_wtime();
  	rpt2 = fin_2 - ini_2;

	print_vector(vec1,num_data);
	print_vector(vec2,num_data);

	printf("first form , elapsed time: %f\n",rpt1);
  	printf("second form , elapsed time: %f\n",rpt2);


	free(vec_entrada);
	free(vec1);
	free(vec2);
	return 0;
}
