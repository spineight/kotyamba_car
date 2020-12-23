## c++ alignment align by cache line
https://stackoverflow.com/questions/8469427/how-and-when-to-align-to-cache-line-size

You may need to align to a cache line boundary, which is typically 64 bytes per cache line

https://stackoverflow.com/questions/7281699/aligning-to-cache-line-and-knowing-the-cache-line-size/7284876

https://vorbrodt.blog/2019/04/06/data-alignment-the-c-way/

## atomics
https://stackoverflow.com/questions/12346487/what-do-each-memory-order-mean

## set thread affinity

https://stackoverflow.com/questions/24645880/set-cpu-affinity-when-create-a-thread
https://eli.thegreenplace.net/2016/c11-threads-affinity-and-hyperthreading/


Yes, there are way to make it. I came across this method on this blog link

And, all other answers to this question have no example. Here is how to write the code

  std::vector<std::thread> threads(num_threads);
  for (unsigned i = 0; i < num_threads; ++i) {
    threads[i] = std::thread([i] {
    std::this_thread::sleep_for(std::chrono::milliseconds(900));
      }
    });

    // Create a cpu_set_t object representing a set of CPUs. Clear it and mark
    // only CPU i as set.
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(i, &cpuset);
    int rc = pthread_setaffinity_np(threads[i].native_handle(),
                                    sizeof(cpu_set_t), &cpuset);
    if (rc != 0) {
      std::cerr << "Error calling pthread_setaffinity_np: " << rc << "\n";
    }
  }

the whole credit should be given to the blog author Eli endersky, and the link was pasted above.
