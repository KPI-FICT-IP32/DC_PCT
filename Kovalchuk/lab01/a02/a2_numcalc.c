#include <mpi.h>

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "a2_funcs.c"

const double EPSILON = 1E-6;                // Desired precision
const int VALUE_TAG = 1;                    // Tag for the power of e
const int TERM_NUMBER_TAG = 2;              // Tag for the current term number
const int TERM_TAG = 3;                     // Tag for the current term value
const int BREAK_TAG = 4;                    // Tag for the signal to stop calculation

const char *input_file_name = "in.txt";     // Input data file name

// Output file name
#ifdef OUTFNAME
const char *output_file_name = OUTFNAME;
#else
const char *output_file_name = "out.txt";   
#endif


/* The main function (the program to calculate sqrt(1 + x) ) */
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

  /* Value of x to calculate sqrt(1 + x) */
  double x;

  /* Read data from file in task 0 */
  if(rank == 0)
  {
    FILE *input_file = fopen(input_file_name, "r");
    /* Terminate all task if can not open input file */
    if(!input_file)
    {
      fprintf(stderr, "Can't open input file!\n\n");
      MPI_Abort(MPI_COMM_WORLD, 1);
      return 1;
    }
    /* Read data from file */
    fscanf(input_file, "%lf", &x);
    fclose(input_file);
  }

  /* Send x from the task 0 to all other tasks */
  if(rank == 0)
  {
    /* Sequentially send x to every task 1..np */
    for(int i = 1; i < np; i++)
    {
      MPI_Send(&x, 1, MPI_DOUBLE, i, VALUE_TAG, MPI_COMM_WORLD);
    }
  }
  else
  {
    /* Receive x from task 0 */
    MPI_Recv(&x, 1, MPI_DOUBLE, 0, VALUE_TAG, MPI_COMM_WORLD,
        MPI_STATUS_IGNORE);
  }

  /* The number of last calculated term in series */
  int last_term_number = 0;
  /* The sum of terms in series */
  double sum = .0;

  /* The main iteration cycle */
  for(int step = 0; step < 1000; step++)
  {
    /* Number of term that is calculated in the current step in this task */
    int term_number;

    /* Send numbers of terms to be calculated in the current step
     * from task 0 to all other tasks */
    if(rank == 0)
    {
      term_number = last_term_number++;
      int current_term_number = last_term_number;
      for(int i = 1; i < np; i++)
      {
        MPI_Send(&current_term_number, 1, MPI_INT, i, TERM_NUMBER_TAG,
            MPI_COMM_WORLD);
        current_term_number++;
      }
      last_term_number = current_term_number;
    }
    else
    {
      MPI_Recv(&term_number, 1, MPI_INT, 0, TERM_NUMBER_TAG,
          MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    /* Calculate current term in series */
    double term = calc_series_term(term_number, x);
    // double term = sqrt_series_term(term_number, x);

    /* Flag indicating the completion of operation */
    int need_break = false;

    /* Calculate sum of terms in series */
    if(rank == 0)
    {
      double current_term = term;
      /* Add term calculated in task 0 to total sum */
      sum += current_term;
      if(fabs(current_term) < EPSILON)
      {
        need_break = true;
      }


      for(int i = 1; i < np; i++)
      {
        /* Receive term from task i, add it to total sum */
        MPI_Recv(&current_term, 1, MPI_DOUBLE, i, TERM_TAG,
            MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        sum += current_term;

        /* Test stopping criteria */
        if(fabs(current_term) < EPSILON)
        {
          need_break = true;
          break;
        }
      }

      /* Send signal of iteration completion from task 0
       * to every other task */
      for(int i = 1; i < np; i++)
      {
        MPI_Send(&need_break, 1, MPI_INT, i, BREAK_TAG, MPI_COMM_WORLD);
      }
    }
    else
    {
      /* Send the calculated term in series to task 0 */
      MPI_Send(&term, 1, MPI_DOUBLE, 0, TERM_TAG, MPI_COMM_WORLD);

      /* Receive from task 0 the signal of iteration completion */
      MPI_Recv(&need_break, 1, MPI_INT, 0, BREAK_TAG, MPI_COMM_WORLD,
          MPI_STATUS_IGNORE);
    }

    /* End iteration if the needed accuracy is achieved */
    if(need_break)
    {
      break;
    }
  }

  /* Output the result in task 0 */
  if(rank == 0)
  {
    FILE *output_file = fopen(output_file_name, "w");
    /* Terminate if can not open output file */
    if(!output_file)
    {
      fprintf(stderr, "Can't open output file!\n\n");
      MPI_Abort(MPI_COMM_WORLD, 2);
      return 2;
    }
    fprintf(output_file, "%.15lf\n", sum);
  }

  /* De-initialize MPI environment and exit */
  MPI_Finalize();
  return 0;
}

