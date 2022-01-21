1. 现代CPU的乱序执行技术（Out-of-order execution）：https://en.wikipedia.org/wiki/Out-of-order_execution、https://www.wenhui.space/docs/08-ic-design/cpu/meltdown-and-spectre/#%E4%B9%B1%E5%BA%8F%E6%89%A7%E8%A1%8C

2. 内存屏障（Memory barrier、memory fence） https://en.wikipedia.org/wiki/Memory_barrier
*  a. 为什么？大多数现代计算机为了提高性能而采取乱序执行，这使得内存屏障成为必须。
    * This reordering of memory operations (loads and stores) normally goes unnoticed within a single thread of execution, but can cause unpredictable behaviour in concurrent programs and device drivers unless carefully controlled.
*  b. 是什么？语义上，内存屏障之前的所有写操作都要写入内存；内存屏障之后的读操作都可以获得同步屏障之前的写操作的结果。因此，对于敏感的程序块，写操作之后、读操作之前可以插入内存屏障。
*  c. 举例1：
  在单线程中，即使发生指令重排，也不会影响最终的结果，最终结果与program order是一致的。
  但在多线程环境中，out-of-order可能会导致未定义的行为。
  Initially, memory locations x and f both hold the value 0. The software thread running on processor #1 loops while the value of f is zero, then it prints the value of x. The software thread running on processor #2 stores the value 42 into x and then stores the value 1 into f. Pseudo-code for the two program fragments is shown below.
  The steps of the program correspond to individual processor instructions.
  > Thread #1 Core #1:
  ```
  while (f == 0);
  // Memory fence required here
  print x;
  ```
  > Thread #2 Core #2:
  ```
  x = 42;
  // Memory fence required here
  f = 1;
  ```
  > One might expect the print statement to always print the number "42"; however, if thread #2's store operations are executed out-of-order, it is possible for f to be updated before x, and the print statement might therefore print "0". Similarly, thread #1's load operations may be executed out-of-order and it is possible for x to be read before f is checked, and again the print statement might therefore print an unexpected value. For most programs neither of these situations is acceptable.
  > A memory barrier must be inserted before thread #2's assignment to f to ensure that the new value of x is visible to other processors at or prior to the change in the value of f.
  > Another important point is a memory barrier must also be inserted before thread #1's access to x to ensure the value of x is not read prior to seeing the change in the value of f
* d. 举例2:
  当驱动程序执行下列动作时，如果处理器的写入指令 out-of-order，使得资料还没有写入存储器，硬件模块就被触发开始动作，就会产生错误的行为。
  ```
  寫資料到記憶體, 稍後硬體模塊會存取這一筆資料
  // 此處需要內存屏障
  觸發硬體模塊開始處理資料>
  ```
* e. 其他例子： double-checked locking.

3. Out-of-order execution versus compiler reordering optimizations
3.1 compiler reordering optimizations
Compilers may also reorder instructions as part of the program optimization process. 

多线程编程与内存可见性：内存可见性问题，主要是高速缓存与内存的一致性问题。一个处理器上的线程修改了某数据，而在另一处理器上的线程可能仍然使用着该数据在专用cache中的老值，这就是可见性出了问题。解决办法是令该数据为volatile属性，或者读该数据之前执行内存屏障。【有待确认：volatile会执行】
* 编译器内存屏障：编译器会对生成的可执行代码做一定优化，造成乱序执行甚至省略（不执行）
```
gcc编译器在遇到内嵌汇编语句：
> asm volatile("" ::: "memory");
将以此作为一条内存屏障，重排序内存操作。即此语句之前的各种编译优化将不会持续到此语句之后。也可用内建的__sync_synchronize

Microsoft Visual C++的编译器内存屏障为：
> _ReadWriteBarrier() MemoryBarrier()

Intel C++编译器的内存屏障为：
> __memory_barrier()
```

参考：https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md、https://www.kernel.org/doc/html/latest/process/volatile-considered-harmful.html
