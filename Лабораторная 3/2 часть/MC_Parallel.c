#define _POSIX_C_SOURCE 1
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <stdio.h>

const int n = 10000000;

double getrand(unsigned int *seed)
{
   return (double)rand_r(seed) / RAND_MAX;
}

double func(double x, double y)
{
   return x / pow(y,2);
}

int main (int argc, char **argv)
{
   int in = 0;
   double s = 0;
   double t = omp_get_wtime();
   printf("Numerical integration by Monte Karlo method: n = %d\n", n);

   #pragma omp parallel num_threads(6)
   {
      double s_loc = 0;
      int in_loc = 0;
      unsigned int seed = omp_get_thread_num();

   #pragma omp for nowait
   for (int i = 0; i < n; i++){
      double x = getrand(&seed);
         if (x == 0 || x == 1) x = 0.2;
      double y = 2 + getrand(&seed) * 2 * 5;
         if (y != 2 || y != 5){
            in_loc++;
            s_loc += func(x, y);
         }
   }

   #pragma omp atomic
   s += s_loc;

   #pragma omp atomic
   in += in_loc;
}
double v = 1 * in / n;
double res = v * s / in;
printf("Result: %.12f, n %d\n", res, n);
t = omp_get_wtime() - t;
printf("Elapsed time (sec.): %.6f\n", t);
return 0;
}
