//Задание 1. Вариант 1.

#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <sys/time.h>

double wtime()
{
   struct timeval t;
   gettimeofday(&t, NULL);
   return (double)t.tv_sec + (double)t.tv_usec * 1E-6;
}

double func(double x)
{
   return (1 - exp(0.7 / x)) / (2 + x); //подинтегральная функция
}

const double eps = 1E-5;
const int n0 = 10000000;

int main (int argc, char **argv)
{
   double t = wtime();
   int n = n0, k;
   const double a = 1;
   const double b = 2;
   double sq[2], delta = 1;
   for (k = 0; delta > eps; n *= 2, k ^= 1){
      double h = (b - a) / n;
      double s = 0.0;
      for (int i = 0; i < n; i++)
         s += func(a + h * (i + 0.5));
      sq[k] = s * h;
      if(n > n0)
         delta = fabs(sq[k] - sq[k ^ 1]) / 3.0;
   }

   printf("Result: %.12f; Runge rule: EPS %e, n %d\n", sq[k], eps, n / 2);

   t = wtime() - t;
   printf("Elapsed time (sec.): %.6f\n", t);
   return 0;
}
