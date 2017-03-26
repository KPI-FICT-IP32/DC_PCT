#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
  /* Initialize MPI environment */
  MPI_Init(&argc, &argv);

  /* Get the rank of the current task */
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  /* Get the total number of tasks */
  int np;
  MPI_Comm_size(MPI_COMM_WORLD, &np);

  /* This statement will be executed in every task */
  printf("I am task %d.  There are %d tasks total.\n", rank, np);

  // for (int i = 0; i < 1000000; ++i) {
  //   for (int j = 0; j < 1000000; ++j) {
  //     for (int k = 0; k < 1000000; ++k) {
  //       printf("[%d] Test output: i=%d; j=%d; k=%d\n", rank, i, j, k);
  //     }
  //   }
  // }

  if(rank == 1)
  {
    /* This statement will be executed only in task with rank 1 */
    printf("I am task 1 and I do things differently.\n");
  }

  /* De-initialize MPI environment and exit */
  MPI_Finalize();
  return 0;
}

