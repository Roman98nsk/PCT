//Задание 1. Вариант 1.

#include <stdio.h>
#include <math.h>
#include <omp.h>
#include <stdint.h>
#include <sys/time.h>

double omp_get_wtime()
{
   struct timeval t;
   gettimeofday(&t, NULL);
   return (double)t.tv_sec + (double)t.tv_usec * 1E-6;
}


double func(double x)
{
   return (1 - exp(0.7 / x)) / (2 + x); //подинтегральная функция
}

int main (int argc, char **argv)
{
   double t = omp_get_wtime();
   const double eps = 1E-5; //точность
   const double a = 1; //нижняя граница интеграла
   const double b = 2; //верхняя граница интеграла
   const int n0 = 10000000; //количество прямоугольников
   printf("Numerical integration: [%f, %f], n0 = %d, EPS = %f\n", a, b, n0, eps);

   double sq[2];

   #pragma omp parallel num_threads(6)
   {
      int n = n0, k;
      double delta = 1;
      for (k = 0; delta > eps; n *= 2, k ^= 1){
         double h = (b - a) / n;
         double s = 0.0;
         sq[k] = 0;

         #pragma omp barrier

         #pragma omp for nowait
         for (int i = 0; i < n; i++)
            s += func(a + h * (i + 0.5));

         #pragma omp atomic
         sq[k] += s * h;

         #pragma omp barrier
         if(n > n0)
            delta = fabs(sq[k] - sq[k ^ 1]) / 3.0;
         #if 0
         printf("n=%d i=%d sq=%.12f delta=%.12f\n", n, k, sq[k], delta);
         #endif
      }

      #pragma omp master
      printf("Result: %.12f; Runge rule: EPS %e, n %d\n", sq[k], eps, n / 2);
   }
   t = omp_get_wtime() - t;
   printf("Elapsed time (sec.): %.6f\n", t);
   return 0;
}
