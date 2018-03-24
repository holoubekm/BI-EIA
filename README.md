# BI-EIA Course
## CTU / ČVUT FIT

Warning - this repository contains lots of ugly code. Please refer here: https://www.youtube.com/watch?v=VcgRaf38uCQ

### Examples
Folder contains examples using the OpenMP syntax for plain C. Supported by all common compilers.

### Task 1
We got an algorithm (Gauss elimition method), our target was to cut down the runtime. We were told to use any appliable methods such as parallelisation (OpenMP), loop unrolling, loop merging, use of vector instructions, eliminate brach predictions, fix the cache corruption and else.

### Task 2
This time we got an algo solving an instance of Knapsack problem. We had to use task parallelism in OpenMP to solve is efficiently.

### Task 3
In the task 3 we had to implement custom `set` with an efficient version of `union` function. The runtime was as usually crucial. The solution involves item buckets (pages) and a lot of bitwise operations. 

### Semestral work
Semestral work was a combination of all above. Assignment was to find multiple pattern in a single file. The whole process shoud have been parallelised and done effiently. There is a lot of different versions so you can compare progress.
