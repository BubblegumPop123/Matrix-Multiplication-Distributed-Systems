### Project Description and Guide

This project consists of three main parts: matrix operations, parallel processing using MPI, and Strassen's matrix multiplication. Additionally, there is a file for reading matrix data and a benchmarking report. Below is a detailed description of each file, along with instructions on how to compile and run the codes.

### File Descriptions

1. **fileread.cpp**
   - This file reads the contents of `matrices.txt` and prints each line to the console.

2. **matrices.txt**
   - This file contains matrix data used by the other parts of the project. It includes dimensions and values for matrices used in the computations.

3. **partA.cpp**
   - This file performs matrix multiplication and calculates the optimal order for scalar multiplications. It initializes matrices with random values, prints the matrices, and computes the optimal multiplication order using dynamic programming.

4. **partB.c**
   - This file implements matrix multiplication using MPI (Message Passing Interface). It reads matrix dimensions from a file, distributes the computation across multiple processes, and prints the result matrix. It also includes time measurement for performance benchmarking.

5. **partC.c**
   - This file performs Strassen's matrix multiplication using MPI. It reads matrix dimensions, generates random matrices, performs matrix multiplication using Strassen's algorithm, and prints the result matrix. The execution time and number of processors are also reported.

6. **Report.pdf**
   - The report contains detailed benchmarking results for the parallel codes executed on multiple virtual machines and processes. It provides insights into the performance and efficiency of the implemented algorithms.

### Compilation and Execution Guide

#### Prerequisites

- A C/C++ compiler (e.g., `g++`, `gcc`)
- MPI library installed (e.g., OpenMPI)
- POSIX-compliant system (Linux, macOS, or Windows with Cygwin)

#### Steps to Compile and Run Each Part

1. **fileread.cpp**

   To compile and run `fileread.cpp`, use the following commands:

   ```sh
   g++ -o fileread fileread.cpp
   ./fileread
   ```

2. **partA.cpp**

   To compile and run `partA.cpp`, use the following commands:

   ```sh
   g++ -o partA partA.cpp
   ./partA
   ```

3. **partB.c**

   To compile and run `partB.c` with MPI, use the following commands:

   ```sh
   mpicc -o partB partB.c
   mpirun -np <number_of_processes> ./partB
   ```

4. **partC.c**

   To compile and run `partC.c` with MPI, use the following commands:

   ```sh
   mpicc -o partC partC.c
   mpirun -np <number_of_processes> ./partC
   ```

### Benchmarking

The benchmarking results, including performance comparisons and efficiency analysis of the parallel codes executed on multiple virtual machines and processes, are detailed in `Report.pdf`. This report provides valuable insights into the scalability and optimization of the implemented algorithms.

This guide should help users understand, compile, run, and benchmark the different parts of the project effectively. For any issues or further details, please refer to the `Report.pdf` for comprehensive benchmarking information.
