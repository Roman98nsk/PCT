#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <sys/time.h>
#include <inttypes.h>

/*
 * m, n - max = 25000
*/

int m = 25000;
int n = 25000;

double wtime()
{
   struct timeval t;
   gettimeofday(&t, NULL);
   return (double)t.tv_sec + (double)t.tv_usec * 1E-6;
}

void run_serial()
{
   double *a, *b, *c;

   a = malloc(sizeof(*a) * m * n);
   b = malloc(sizeof(*b) * n);
   c = malloc(sizeof(*c) * m);

   for (int i = 0; i < m; i++){
      for (int j = 0; j < n; j++)
         a[i * n + j] = i + j;
   }
   for (int j = 0; j < n; j++)
      b[j] = j; //инициализируем вектор b

   double t = wtime();
   matrix_vector_product(a, b, c, m, n);
   t = wtime() - t;

   printf("Elapsed time (serial): %.6f sec.\n", t);   //время работы функции
   free(a);
   free(b);
   free(c);
}

void run_parallel()
{
   double *a, *b, *c;

   a = malloc(sizeof(*a) * m * n);
   b = malloc(sizeof(*b) * n);
   c = malloc(sizeof(*c) * m);

   for (int i = 0; i < m; i++){
      for (int j = 0; j < n; j++)
         a[i * n + j] = i + j;
   }
   for (int j = 0; j < n; j++)
      b[j] = j; //инициализируем вектор b

   double t = wtime();
   matrix_vector_product_omp(a, b, c, m, n);
   t = wtime() - t;

   printf("Elapsed time (parallel): %.6f sec.\n", t);   //время работы функции
   free(a);
   free(b);
   free(c);
}

void matrix_vector_product(double *a, double *b, double *c, int m, int n)
{
      for (int i = 0; i < m; i++) {
         c[i] = 0.0;
         for (int j = 0; j < n; j++)
            c[i] += a[i * n + j] * b[j];
      }
   }

void matrix_vector_product_omp(double *a, double *b, double *c, int m, int n)
{
   //параллельный регион. Доступно 6 ядер
   #pragma omp parallel proc_bind(spread) num_threads(2)
   {
      int nthreads = omp_get_num_threads(); //"ntreads" - локальная переменная для потоков
      int threadid = omp_get_thread_num(); //номер потока
      int items_per_thread = m / nthreads; //эл. вектора С на поток
      int lb = threadid * items_per_thread;
      int ub = (threadid == nthreads - 1) ? (m - 1) : (lb + items_per_thread - 1);

      for (int i = lb; i <= ub; i++) {
         c[i] = 0.0;
         for (int j = 0; j < n; j++)
            c[i] += a[i * n + j] * b[j];
      }
   }
}

int main(int argc, char **argv)
{
   printf("MATRIX = %d\n", m * n);
   printf("Matrix-vector product (c[m] = a[m, n] * b[n]; m = %d, n = %d)\n", m, n);
   printf("Memory used: %" PRIu64 " MiB\n", ((m * n + m + n) * sizeof(double)) >> 20);

   run_serial();
   run_parallel();

   return 0;
}
