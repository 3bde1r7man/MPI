#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mpi.h"

// Encrypt/Decrypt the string takes the string, shift value, start and end index of the string
void caesarCipher(char *str, int shift, int start, int end) {
    for (int i = start; i <= end; i++) {
        if (str[i] >= 'A' && str[i] <= 'Z'){
            str[i] = (str[i] - 'A' + shift) % 26 + 'A';
        }
        else if (str[i] >= 'a' && str[i] <= 'z'){
            str[i] = (str[i] - 'a' + shift) % 26 + 'a';
        }
    }
}

int main(int argc, char *argv[]) {
    int rank, size, source, dest, tag = 0;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if(rank == 0) {
        int choice;
        printf("Enter 1 to encode or 2 to decode: ");
        scanf("%d", &choice);
        if(choice != 1 && choice != 2) {
            printf("Invalid choice\n");
            return 0;
        }
        
        int mode;
        printf("Enter 1 to read from (console) or 2 to read from (file): ");
        scanf("%d", &mode);
        
        char str[100];
        if(mode ==1){
            printf("Enter the string: ");
            scanf("%s", str);
        }
        else if(mode==2){
            char filename[100];

            printf("Enter the filename: ");
            scanf("%s", filename); 

            FILE* file;
            file = fopen(filename, "r");

            if (file == NULL) {
                printf("The file could not be opened. The program will exit.\n");
                exit(1); 
            } else {
                fgets(str, 100, file);
            }
        }
        else{
            printf("Invalid choice\n");
            return 0;
        }
        
        
        int len = strlen(str);
        int range = len / (size-1);
        for(int i = 1; i < size; i++) {
            int start = (i - 1) * range;
            int end = start + range - 1;
            // If it is the last process, then the end index will be the length of the string
            if(i == size - 1) {
                end = len - 1;
            }
            // Sending the start, end, choice, length of the string and the string to the processes
            MPI_Send(&start, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&end, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&choice, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&len, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(str, strlen(str) + 1, MPI_CHAR, i, 0, MPI_COMM_WORLD);
        }
        for(int i = 1; i < size; i++) {
            char temp[len];
            int start = (i - 1) * range;
            int end = start + range - 1;
            if(i == size - 1) {
                end = len - 1;
            }
            // Receiving the encrypted/decrypted string from the processes
            MPI_Recv(temp, 100, MPI_CHAR, i, 0, MPI_COMM_WORLD, &status);
            // Copying the encrypted/decrypted string to the original string
            for(int j = start; j <= end; j++) {
                str[j] = temp[j];
            }
        }
        printf("Encrypted/Decrypted string: %s\n", str);
    } else {
        int start, end, choice, len;
        // Receiving the start, end, choice, length of the string and the string from the process 0
        MPI_Recv(&start, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&end, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&choice, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&len, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        char str[len];
        MPI_Recv(str, 100, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
        // Encrypting/Decrypting the string
        if(choice == 1) {
            // Encrypting the string with shift value 3
            caesarCipher(str, 3, start, end);
        } else {
            // Decrypting the string with shift value 23
            caesarCipher(str, 23, start, end);
        }
        // Sending the encrypted/decrypted string to the process 0
        MPI_Send(str, strlen(str) + 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    }
    MPI_Finalize();
}
