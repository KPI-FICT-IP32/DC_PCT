#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <mpi.h>

/* Calculate function (integrand) value */
double function(double x) {
  /* x * cos(ln(x)) */
  return x * cos(log(x));
}

/* Check Runge's rule */
bool check_Runge(double I2, double I, double epsilon) {
  return ((double)fabs(I2 - I) / 3) < epsilon;
}

/* Integrate using right rectangles */
double integrate(double start, double finish, double epsilon) {
  int num_iterations = 1; /* Initial iteration number */
  double last_res = 0.;   /* Integration result on the previous step */
  double res = -1.;       /* Current integration result */
  double step = 0.;       /* Rectangles' width */
  while(!check_Runge(res, last_res, epsilon)) {
    num_iterations *= 2;
    last_res = res;
    res = 0.;
    step = (finish - start) / num_iterations;
    for(int i = 0; i < num_iterations; i++) {
      res += function(finish - i * step) * step;
    }
  }
  return res;
}

/* Write one double number to file filename */
void write_double_to_file(const char* filename, double data) {
  /* Open file for writing */
  FILE *fp = fopen(filename, "w");
  /* Check if the file is opened */
  if(fp == NULL) {
    printf("Failed to open the file\n");
    /* Terminate the program */
    MPI_Abort(MPI_COMM_WORLD, 1);
  }
  /* Write one double number to file */
  fprintf(fp, "%lg\n", data);
  /* Close file */
  fclose(fp);
}

int main(int argc, char* argv[]) {
  int np; /* total number of tasks */
  int rank; /* rank of the current task */

  MPI_Init(&argc, &argv);

  MPI_Comm_size(MPI_COMM_WORLD, &np);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  /* Read data from a file into an array.
   * Takes place in task 0.
   * input[0] -- lower limit of integration
   * input[1] -- upper limit of integration
   * input[2] -- admissible absolute error */
  double input[3];
  if(rank == 0) {
    /* Open file input.txt in read-only mode */
    FILE *fp = fopen("input.txt", "r");
    /* Check if the file is opened */
    if(fp == NULL) {
      printf("Failed to open the file\n");
      /* Terminate the program */
      MPI_Abort(MPI_COMM_WORLD, 1);
    }
    /* Read 3 double numbers */
    for(int i = 0; i < 3; i++)
      fscanf(fp, "%lg", &input[i]);

    /* Close file */
    fclose(fp);
  }

  /* Send input data from task 0 to all other tasks */
  MPI_Bcast(input, 3, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  double start = input[0];
  double finish = input[1];
  double epsilon = input[2];
  double step = (finish - start) / np;
  double res = integrate(
    start + rank * step,
    start + (rank + 1) * step,
    epsilon / np
  );
  double result = res;
  /* Send intermediate integration results to task 0 from all other tasks
   * and save intermediate results in the variable 'result' */
  MPI_Reduce (&res, &result, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

  /* Output result by task 0 to output file output.txt */
  if (rank == 0)
    write_double_to_file("output.txt", result);

  /* Finish work with MPI */
  MPI_Finalize();

  return 0;
}
