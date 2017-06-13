// Author: Wes Kendall
// Copyright 2012 www.mpitutorial.com
// This code is provided freely with the tutorials on mpitutorial.com. Feel
// free to modify it for your own use. Any distribution of the code must
// either provide a link to www.mpitutorial.com or keep this header intact.
//
// Program that computes the average of an array of elements in parallel using
// MPI_Scatter and MPI_Gather
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include <assert.h>

// Creates an array of random numbers. Each number has a value from 0 - 1
float *create_rand_nums(int num_elements) {
  float *rand_nums = (float *)malloc(sizeof(float) * num_elements);
  assert(rand_nums != NULL);
  int i;
  for (i = 0; i < num_elements; i++) {
    rand_nums[i] = (rand() / (float)RAND_MAX);
  }
  return rand_nums;
}

// Computes the average of an array of numbers
float compute_avg(float *array, int num_elements) {
  float sum = 0.f;
  int i;
  for (i = 0; i < num_elements; i++) {
    sum += array[i];
  }
  return sum / num_elements;
}

//leer vector mpi scatter
void Read vector(double local a[] ,int local n, int n,char vec name[], int my rank,MPI Comm comm ) {
  double∗ a = NULL;
  int i;

  if (my rank == 0) {
    a = malloc(n∗sizeof(double));
    printf("Enter the vector %s\n", vec name);
    for (i = 0; i < n; i++)
        scanf("%lf", &a[i]);
    MPI Scatter(a, local n, MPI DOUBLE, local a, local n,
    MPI DOUBLE, 0, comm);
  free(a);
  } 
  else {
    MPI Scatter(a, local n, MPI DOUBLE, local a, local n,
    MPI DOUBLE, 0, comm);

    }

}

// gather

void Print vector( double local_b[],
                    int local_n,
                    int n, 
                    char title[], 
                    int my_rank, 
                    MPI_Comm comm
                  ) {
  double∗ b = NULL;
  int i;

  if (my rank == 0) {
      b = malloc(n∗sizeof(double));
      MPI Gather(local b, local n, MPI DOUBLE, b, local n,
      MPI DOUBLE, 0, comm);
      printf("%s\n", title);
      for (i = 0; i < n; i++)
          printf("%f ", b[i]);
      printf("\n");
      free(b);
  } 
  else {
      MPI Gather(local b, local n, MPI DOUBLE, b, local n,
      MPI DOUBLE, 0, comm);
  }

}

// allgather

void Mat vect mult(   double local_A[], 
                      double local_x[], 
                      double local_y[], 
                      int local_m, 
                      int n, 
                      int local_n, 
                      MPI_Comm comm)
{ 
    
    double *x;
    int local_i, j;
    int local_ok = 1;

    x = malloc(n∗sizeof(double));
    MPI Allgather(local_x, local_n, MPI_DOUBLE,
    x, local_n, MPI_DOUBLE, comm);
    
    for (local i = 0; local i < local m; local i++) {
        local y[local i] = 0.0;
        for (j = 0; j < n; j++)
              local y[local i] += local A[local i∗n+j]∗x[j];
    }
    free(x);
}

int main(int argc, char** argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: avg num_elements_per_proc\n");
    exit(1);
  }

  int num_elements_per_proc = atoi(argv[1]);
  // Seed the random number generator to get different results each time
  srand(time(NULL));

  MPI_Init(NULL, NULL);

  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  // Create a random array of elements on the root process. Its total
  // size will be the number of elements per process times the number
  // of processes
  float *rand_nums = NULL;
  if (world_rank == 0) {
    rand_nums = create_rand_nums(num_elements_per_proc * world_size);
  }

  // For each process, create a buffer that will hold a subset of the entire
  // array
  float *sub_rand_nums = (float *)malloc(sizeof(float) * num_elements_per_proc);
  assert(sub_rand_nums != NULL);

  // Scatter the random numbers from the root process to all processes in
  // the MPI world
  MPI_Scatter(rand_nums, num_elements_per_proc, MPI_FLOAT, sub_rand_nums,
              num_elements_per_proc, MPI_FLOAT, 0, MPI_COMM_WORLD);

  // Compute the average of your subset
  float sub_avg = compute_avg(sub_rand_nums, num_elements_per_proc);

  // Gather all partial averages down to the root process
  float *sub_avgs = NULL;
  if (world_rank == 0) {
    sub_avgs = (float *)malloc(sizeof(float) * world_size);
    assert(sub_avgs != NULL);
  }
  MPI_Gather(&sub_avg, 1, MPI_FLOAT, sub_avgs, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);

  // Now that we have all of the partial averages on the root, compute the
  // total average of all numbers. Since we are assuming each process computed
  // an average across an equal amount of elements, this computation will
  // produce the correct answer.
  if (world_rank == 0) {
    float avg = compute_avg(sub_avgs, world_size);
    printf("Avg of all elements is %f\n", avg);
    // Compute the average across the original data for comparison
    float original_data_avg =
      compute_avg(rand_nums, num_elements_per_proc * world_size);
    printf("Avg computed across original data is %f\n", original_data_avg);
  }

  // Clean up
  if (world_rank == 0) {
    free(rand_nums);
    free(sub_avgs);
  }
  free(sub_rand_nums);

  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Finalize();
}
