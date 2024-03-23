

## Problem 1: Counting Prime Numbers
This program calculates the count of prime numbers within a given range `[x, y]` in parallel using MPI_Send and MPI_Receive.

### How to Run:
1. Compile the program using MPI:
   ```
   mpicc -o countPrime.out countPrime.c
   ```

2. Run the program with the desired number of processes:
   ```
   mpiexec -n <num_processes> ./countPrime.out
   ```

### Explanation:
- The master process calculates the subrange size `r = (y - x) / (p-1)`, where `p` is the number of processes.
- It broadcasts `x` and `r` to each slave process using MPI_Send.
- Slave processes receive `x` and `r` through MPI_Receive.
- Each slave process calculates the lower bound `a` and upper bound `b` according to its rank and counts primes in its subrange `[a, b]`.
- Slave processes send their partial count to the master process using MPI_Send.
- The master process receives sub-counts from each slave process using MPI_Receive.
- Finally, it prints the total count of primes between `x` and `y` along with the count of primes in each process.

## Problem 2: Caesar Cipher
This program implements Caesar Cipher Encryption/Decryption algorithm in parallel using MPI_Send and MPI_Receive.

### How to Run:
1. Compile the program using MPI:
   ```
   mpicc -o caesarCipher.out caesarCipher.c
   ```

2. Run the program with the desired number of processes:
   ```
   mpiexec -n <num_processes> ./caesarCipher.out
   ```

### Explanation:
- The master process reads a string from the user.
- The string is scattered among the processes.
- Each process converts the characters of its string portion to be encrypted/decrypted using the Caesar Cipher algorithm (shifting each character by 3).
- The encrypted/decrypted string portions are gathered at the master process.
- The master process prints the final encrypted/decrypted string.


Make sure to replace `<num_processes>` with appropriate values when running the programs.
