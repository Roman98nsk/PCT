#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>

const int n = 10000000; //10^7 или 10^8

double wtime()
{
   struct timeval t;
   gettimeofday(&t, NULL);
   return (double)t.tv_sec + (double)t.tv_usec * 1E-6;
}

double getrand()
{
   return (double)rand() / RAND_MAX;
}

double func(double x, double y)
{
   return x / pow(y, 2);
}

int main (int argc, char **argv)
{
   int in = 0;
   double s = 0;
   double t = wtime();

   for(int i = 0; i < n; i++){
      double x = getrand();
         if (x == 0 || x == 1) x = 0.2;
      double y = 2 + getrand() * 2 * 5;
         if (y != 2 || y != 5){
         in++;
         s += func(x, y);
      }
   }
   double v = 1 * in / n;
   double res = v * s / in;
   t = wtime() - t;

   printf("Result: %.12f, n %d\n", res, n);
   printf("Elapsed time (sec.): %.6f\n", t);
   return 0;
}
