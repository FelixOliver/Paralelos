#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include <assert.h>

//leer vector mpi scatter
void Read_vector(float * local_a ,int local_n, int n,char vec_name[], int my_rank,MPI_Comm comm ) {
  float ∗ a ;
  int i;

  if (my_rank == 0) {
    a = (float *)malloc(sizeof(float)*n);

    printf("Enter the vector %s\n", vec_name);
    for (i = 0; i < n; i++)
        scanf("%lf", &a[i]);
    MPI_Scatter(a, local_n, MPI_FLOAT, local_a, local_n, MPI_FLOAT, 0, comm);
    free(a);
  } 
  else {
    MPI_Scatter(a, local_n, MPI_FLOAT, local_a, local_n, MPI_FLOAT, 0, comm);

    }

}


// gather
/*
void Print vector( float local_b[],
                    int local_n,
                    int n, 
                    char title[], 
                    int my_rank, 
                     MPI_Comm comm
                  ) {
  float∗ b = NULL;
  int i;

  if (my rank == 0) {
      b = malloc(n∗sizeof double));
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
*/

// allgather

/*
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


*/
int main(int argc, char** argv)
{

  MPI_Init(NULL, NULL);

  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  int n = 8; // size total del vector
  int local_n = n/world_size; // size para cada proceso

  float * local_a;

  local_a = (float*)malloc(sizeof(float)*local_n);

  Read_vector(local_a, local_n, n, "myvector", world_rank,MPI_COMM_WORLD);



  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Finalize();

}