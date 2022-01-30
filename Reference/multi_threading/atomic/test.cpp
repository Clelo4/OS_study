/**
 *  证明CPU乱序执行，并使用内存屏障消除乱序执行的影响
 *  rtoax 荣涛
 *  2021年5月26日
 */
#include <pthread.h>

#include <atomic>
#include <cstdio>

// #define HAVE_BARRIER 1
#ifdef HAVE_BARRIER
static void inline mrwbarrier() { asm volatile("mfence" ::: "memory"); }
static void inline mrbarrier() { asm volatile("lfence" ::: "memory"); }
static void inline mwbarrier() { asm volatile("sfence" ::: "memory"); }
#else
#define mrwbarrier()
#define mrbarrier()
#define mwbarrier()
#warning define HAVE_BARRIER to solve Out-of-Order Exec.
#endif

// volatile int x, y, a, b;
std::atomic<int> x;
std::atomic<int> y;
std::atomic<int> a;
std::atomic<int> b;

void *task1(void *arg) {
  // a = 1;
  a.store(1, std::memory_order_release);
  mrwbarrier();
  x.store(b.load(std::memory_order_acquire), std::memory_order_release);
  pthread_exit(NULL);
}

void *task2(void *arg) {
  // b = 1;
  b.store(1, std::memory_order_release);
  mrwbarrier();
  y.store(a.load(std::memory_order_acquire), std::memory_order_release);
  pthread_exit(NULL);
}

int main() {
  pthread_t tasks[2];

  int i, j;
  i = 100000000;

  while (--i >= 0) {
    x.store(0, std::memory_order_release);
    y.store(0, std::memory_order_release);
    a.store(0, std::memory_order_release);
    b.store(0, std::memory_order_release);
    // asm volatile("mfence" ::: "memory");

    pthread_create(&tasks[0], NULL, task1, NULL);
    pthread_create(&tasks[1], NULL, task2, NULL);

    pthread_join(tasks[0], NULL);
    pthread_join(tasks[1], NULL);

    if (x.load(std::memory_order_acquire) == 0 &&
        y.load(std::memory_order_acquire) == 0) {
      printf("CPU Out of Order Exec Happend.\n");
    }
  }
}
