#include <stdio.h>
#include <unistd.h>
#include <mpi.h>

const int MY_TAG = 42;

/*
 * The program is designed exactly for 2 processes.
 * An array of 5 elements is passed from process 0 to process 1
 * using non-blocking transfer. Process 1 starts receiving data
 * earlier than process 0 starts its transmission.
 */
int main(int argc, char* argv[])
{
  int np;
  int rank;

  MPI_Init(&argc,&argv);
  MPI_Comm_size(MPI_COMM_WORLD, &np);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if(rank == 0)
  {
    //sleep(5); /* Delay.  Replace with calculations in a real program. */
    int data[] = { 1, 2, 3, 4, 5}; /* Data to transfer */
    MPI_Request send_req; /* Transfer (send) handle */

    printf("Task 0.  MPI_Isend()\n");
    MPI_Isend(&data, 5, MPI_INT, 1, MY_TAG, MPI_COMM_WORLD, &send_req);
    /* Now buffer 'data' is not available */
    printf("Task 0.  Sending data...\n");
    //sleep(5); /* Delay.  Replace with calculations in a real program. */
    printf("Task 0.  Computation finished.\n");

    /* Wait for transfer completion */
    MPI_Wait(&send_req, MPI_STATUS_IGNORE);
  }
  if(rank == 1)
  {
    int data[5]; /* Receive buffer */
    MPI_Request recv_req; /* Transfer (receive) handle */

    printf("Task 1.  MPI_Irecv()\n");
    MPI_Irecv(&data, 5, MPI_INT, 0, MY_TAG, MPI_COMM_WORLD, &recv_req);
    /* Now buffer 'data' is not available */
    printf("Task 1.  Recieving data...\n");
    //sleep(2); /* Delay.  Replace with calculations in a real program. */

    /* Wait for transfer completion */
    MPI_Wait(&recv_req, MPI_STATUS_IGNORE);

    printf("Task 1.  Recieved data:");
    /* Output data */
    for(int i = 0; i < 5; i++)
    {
      printf(" %d", data[i]);
    }
    printf("\n");
  }

  MPI_Finalize();
  return 0;
}
