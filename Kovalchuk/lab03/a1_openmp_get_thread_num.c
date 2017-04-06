#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main (int argc, char *argv[]) {
  double start_time = omp_get_wtime();
  if (argc < 2) {
    fprintf(stderr, "\e[31mUsage: a1 <ordered threads>\e]39m\n");
    return 1;
  }


  int ordered_threads = atoi(argv[1]);

#pragma omp parallel num_threads(ordered_threads)
  {
    int task_id = omp_get_thread_num();
    if(task_id == 0) {
      printf("I am Master!\n");
    } else {
      printf("I am Slave number %d\n", task_id);
    }
  }

  double end_time = omp_get_wtime();  
  printf("Time: %f\n", end_time-start_time);

  return 0;
}

