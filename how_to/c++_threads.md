```bash
cat /proc/cpuinfo
```

A summary output can be obtained from lscpu:
```bash
lscpu
```

Observations:
Car::driving_loop thread was migrated from CPUs (1,2,3)
(based on this article: https://eli.thegreenplace.net/2016/c11-threads-affinity-and-hyperthreading/)
```C++
std::cout << "car driving loop on CPU" << sched_getcpu() << '\n';
```

sched_getcpu function (glibc specific - other platforms will have other APIs with similar functionality)

Some observations: the threads are sometimes scheduled onto the same CPU, and sometimes onto different CPUs.
Also, there's quite a bit of migration going on.
Eventually, the scheduler managed to place each thread onto a different CPU, and keep it there.
Different constraints (such as system load) could result in a different scheduling, of course.

Now let's rerun the same sample,
but this time using taskset to restrict the affinity of the process to only two CPUs - 2 and 3:
```bash
taskset -c 2,3
```


set the affinities of specific threads from within the program
```C++
namespace kotyamba {
  void Car::driving_loop() {
    std::cout << "#### Setting driving_loop thread`s affinity programatically #####\n";
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    constexpr int CPU_ID = 2;
    CPU_SET(CPU_ID, &cpuset);
    const int rc = pthread_setaffinity_np(pthread_self(),
                                    sizeof(cpu_set_t), &cpuset);
    if (rc != 0) {
      std::cerr << "Error calling pthread_setaffinity_np: " << rc << "\n";
    }

```


I can use the excellent lstopo tool to display the topology of my processor
```bash
lstopo
```

An alternative non-graphical way to see which threads share the same core
is to look at a special system file that exists per logical CPU. For example, for CPU 0:
```bash
$ cat /sys/devices/system/cpu/cpu0/topology/thread_siblings_list
0,4
```

