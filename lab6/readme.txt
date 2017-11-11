Christopher Aziz
304806012

Assignment 6. Multithreaded performance
---------------------------------------

1. Implementation, Issues, and Conclusion

To parallelize the ray tracer program, I created the input number of pthreads
and divided the columns of pixels to trace between them. Instead of printing the
pixels as they were traced, I instead created a global 3D array scaled_color to
hold all the pixels until after the threads were joined. I had to trace the
image in a way that would guarantee that each thread was designated unique
pixels to trace. I did this by starting each thread on its thread number and
iterate by the number of threads. Technically, this would mean that when the
number of threads exceeds the number of columns in the image to be traced, the
extra threads would serve no purpose. This is okay, though, because the overhead
of creating that many threads would likely defeat the purpose of parallelism in
the first place.

I also had to make the number of threads nthreads and the scene global variables
because each thread needed access to them. Since the threads only read from
these variables and never wrote to them, the program is still thread-safe.

I created local arrays that kept track of the pthreads and their associated
thread numbers. These were used to join the threads after the tracing was done
and assign each thread its associated pixel columns to trace.

After the threads were joined, I had to print the pixels in the correct
order. This was done, as described above, by moving the printing from directly
after the tracing to after the joining of threads. The pixels were read out of
the 3D scaled_color array.

2. Performance Improvements

There was a clear improvement of performance of the program due to my addition
of multithreading support. By parallizing the threading of the program as
described above, the real time taken by the srt program shows a direct
correlation with the the number of pthreads used. As the number of threads
increases, the real time decreases.

time ./srt 1-test.ppm >1-test.ppm.tmp

real	0m44.959s
user	0m44.954s
sys	0m0.001s

time ./srt 2-test.ppm >2-test.ppm.tmp

real	0m23.036s
user	0m45.690s
sys	0m0.001s

time ./srt 4-test.ppm >4-test.ppm.tmp

real	0m11.641s
user	0m46.252s
sys	0m0.003s

time ./srt 8-test.ppm >8-test.ppm.tmp

real	0m5.974s
user	0m46.276s
sys	0m0.002s

Above, the leading integer of the input file to srt indicates the number of
threads used by the program. Between all of the tests, we see that the user and
system time stays relatively constant. This indicates that the total time used
by all the threads is constant, which makes sens. By using many threads,
however, we see a drastic decrease in real time. Using 8 threads, for example, we see
that the real time decreases 45 seconds to just 6 seconds in the parallized version.
