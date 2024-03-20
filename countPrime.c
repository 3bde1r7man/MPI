#include <stdio.h>
#include <mpi.h>

int isPrime(int num) {
    if (num <= 1)
        return 0;
    for (int i = 2; i <= num / 2; i++) {
        if (num % i == 0)
            return 0;
    }
    return 1;
}

int main(int argc, char *argv[]) {
    int my_rank;
    int p;
    int source;
    int dest;
    int tag = 0;
    int x;
    int y;
    int r;
    int my_count = 0;

    int total_count = 0;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    if (my_rank == 0) {
        printf("Enter the lower bound: ");
        scanf("%d", &x);
        printf("Enter the upper bound: ");
        scanf("%d", &y);

        r = (y - x) / (p - 1);

        for (dest = 1; dest < p; dest++) {
            MPI_Send(&x, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
            MPI_Send(&r, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
            x += r;
        }

        MPI_Send(&y, 1, MPI_INT, p - 1, tag, MPI_COMM_WORLD);

        int recv_counts[p]; 
        for (source = 1; source < p; source++) {
            MPI_Recv(&recv_counts[source], 1, MPI_INT, source, tag, MPI_COMM_WORLD, &status);
            total_count += recv_counts[source];
        }

        printf("\nTotal number of prime numbers is: %d \n", total_count);
        for (int i = 1; i < p; i++) 
        {
            printf("Total number of prime numbers in p%d: %d\n", i, recv_counts[i]);
        }
    } else {
        MPI_Recv(&x, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
        MPI_Recv(&r, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);

        if (my_rank == p - 1) {
            MPI_Recv(&y, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
        }

        int a = x;
        int b = x + r;

        if (my_rank == (p - 1)) {
            if (b < y) {
                b = y + 1;
            }
        }

        for (; a < b; a++) {
            if (isPrime(a)) {
                my_count++;
            }
        }

        MPI_Send(&my_count, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
