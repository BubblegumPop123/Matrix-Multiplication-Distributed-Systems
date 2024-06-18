//part C


#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

#define MAX_DIMENSION 50

// Function declarations
void generateRandomMatrix(double *matrix, int rows, int columns);
void addMatrices(double *A, double *B, double *C, int rows, int columns);
void subtractMatrices(double *A, double *B, double *C, int rows, int columns);
void multiplyMatrices(double *A, double *B, double *C, int rows, int columns);
void strassenMultiply(double *A, double *B, double *C, int rows, int columns);
void strassenMultiplyRecursive(double *A, double *B, double *C, int n);


int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int my_rank, p;
    double start_time, end_time;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    // Start time measurement
    if (my_rank == 0) {
        start_time = MPI_Wtime();
    }

    // Read matrix dimensions from the file
    FILE *file = fopen("matrices.txt", "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening the file.\n");
        MPI_Finalize();
        exit(1);
    }

    int num_matrices = 0;
    char line[256];
    while (fgets(line, sizeof(line), file) != NULL) {
        num_matrices++;
    }
    rewind(file);

    int *matrix_dimensions = (int *)malloc(sizeof(int) * num_matrices * 2);
    if (matrix_dimensions == NULL) {
        fprintf(stderr, "Memory allocation error for matrix_dimensions.\n");
        MPI_Finalize();
        exit(1);
    }

    for (int i = 0; i < num_matrices; i++) {
        fscanf(file, "%d X %d", &matrix_dimensions[i * 2], &matrix_dimensions[i * 2 + 1]);
        fgetc(file); // To read the newline character
    }
    fclose(file);

    // Process each matrix
    for (int i = 0; i < num_matrices; i++) {
        int rows = matrix_dimensions[i * 2];
        int columns = matrix_dimensions[i * 2 + 1];

        // Allocate memory for matrices
        double *matrix_A = (double *)malloc(sizeof(double) * rows * MAX_DIMENSION);
        double *matrix_B = (double *)malloc(sizeof(double) * MAX_DIMENSION * columns);
        double *matrix_C = (double *)malloc(sizeof(double) * rows * columns);

        if (matrix_A == NULL || matrix_B == NULL || matrix_C == NULL) {
            fprintf(stderr, "Memory allocation error.\n");
            MPI_Finalize();
            exit(1);
        }

        // Initialize matrices with random values
        generateRandomMatrix(matrix_A, rows, MAX_DIMENSION);
        generateRandomMatrix(matrix_B, MAX_DIMENSION, columns);

        // Perform Strassen's multiplication
        strassenMultiply(matrix_A, matrix_B, matrix_C, rows, columns);

        // Output the result matrix
        if (my_rank == 0) {
            printf("Result Matrix %d:\n", i + 1);
            for (int r = 0; r < rows; r++) {
                for (int c = 0; c < columns; c++) {
                    printf("%.2f\t", matrix_C[r * columns + c]);
                }
                printf("\n");
            }
            printf("\n");
        }

        // Free allocated memory
        free(matrix_A);
        free(matrix_B);
        free(matrix_C);
    }

    free(matrix_dimensions);


    if (my_rank == 0) {
        end_time = MPI_Wtime();
        printf("Execution Time: %f seconds\n", end_time - start_time);
        printf("Processors: %d", p);
    }

    MPI_Finalize();
    return 0;
}

void generateRandomMatrix(double *matrix, int rows, int columns) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            matrix[i * columns + j] = ((double)rand() / RAND_MAX) * 2.0 - 1.0;
        }
    }
}
void addMatrices(double *A, double *B, double *C, int rows, int columns) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            C[i * columns + j] = A[i * columns + j] + B[i * columns + j];
        }
    }
}
void subtractMatrices(double *A, double *B, double *C, int rows, int columns) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            C[i * columns + j] = A[i * columns + j] - B[i * columns + j];
        }
    }
}
void multiplyMatrices(double *A, double *B, double *C, int rows, int columns) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            double sum = 0.0;
            for (int k = 0; k < columns; k++) {
                sum += A[i * columns + k] * B[k * columns + j];
            }
            C[i * columns + j] = sum;
        }
    }
}
void strassenMultiply(double *A, double *B, double *C, int rows, int columns) {
    // Determine the size of the new square matrices
    int maxSize = (rows > columns) ? rows : columns;
    if (maxSize % 2 != 0) {
        maxSize++;  // Make it even
    }

    // Allocate memory for square matrices
    double *A_square = (double *)calloc(maxSize * maxSize, sizeof(double));
    double *B_square = (double *)calloc(maxSize * maxSize, sizeof(double));
    double *C_square = (double *)calloc(maxSize * maxSize, sizeof(double));

    // Copy the original matrices into the top-left corner of the square matrices
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            A_square[i * maxSize + j] = A[i * columns + j];
            B_square[i * maxSize + j] = B[i * columns + j];
        }
    }

    // Perform Strassen's multiplication on the square matrices
    // This part would involve the usual steps of Strassen's algorithm
    // but applied to A_square, B_square, and C_square
    // ...
    strassenMultiplyRecursive(A_square, B_square, C_square, maxSize);

    // Copy the relevant part of the result back into C
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            C[i * columns + j] = C_square[i * maxSize + j];
        }
    }

    // Free allocated memory for square matrices
    free(A_square);
    free(B_square);
    free(C_square);
}

void strassenMultiplyRecursive(double *A, double *B, double *C, int n) {
    if (n <= 2) {
        // Base case: Use standard multiplication for small matrices
        multiplyMatrices(A, B, C, n, n);
        return;
    }

    // Calculate the size of the submatrices
    int newSize = n / 2;
    int size = newSize * newSize;

    // Allocate memory for submatrices and intermediate matrices
    double *A11 = (double *)malloc(size * sizeof(double));
    double *A12 = (double *)malloc(size * sizeof(double));
    double *A21 = (double *)malloc(size * sizeof(double));
    double *A22 = (double *)malloc(size * sizeof(double));
    double *B11 = (double *)malloc(size * sizeof(double));
    double *B12 = (double *)malloc(size * sizeof(double));
    double *B21 = (double *)malloc(size * sizeof(double));
    double *B22 = (double *)malloc(size * sizeof(double));
    double *M1 = (double *)malloc(size * sizeof(double));
    double *M2 = (double *)malloc(size * sizeof(double));
    double *M3 = (double *)malloc(size * sizeof(double));
    double *M4 = (double *)malloc(size * sizeof(double));
    double *M5 = (double *)malloc(size * sizeof(double));
    double *M6 = (double *)malloc(size * sizeof(double));
    double *M7 = (double *)malloc(size * sizeof(double));
    double *tempA = (double *)malloc(size * sizeof(double));
    double *tempB = (double *)malloc(size * sizeof(double));

    // Partition A and B matrices into 4 submatrices each
    for (int i = 0; i < newSize; i++) {
        for (int j = 0; j < newSize; j++) {
            A11[i * newSize + j] = A[i * n + j];
            A12[i * newSize + j] = A[i * n + (j + newSize)];
            A21[i * newSize + j] = A[(i + newSize) * n + j];
            A22[i * newSize + j] = A[(i + newSize) * n + (j + newSize)];
            B11[i * newSize + j] = B[i * n + j];
            B12[i * newSize + j] = B[i * n + (j + newSize)];
            B21[i * newSize + j] = B[(i + newSize) * n + j];
            B22[i * newSize + j] = B[(i + newSize) * n + (j + newSize)];
        }
    }

    // M1 = (A11 + A22) * (B11 + B22)
    addMatrices(A11, A22, tempA, newSize, newSize);
    addMatrices(B11, B22, tempB, newSize, newSize);
    strassenMultiplyRecursive(tempA, tempB, M1, newSize);

    // M2 = (A21 + A22) * B11
    addMatrices(A21, A22, tempA, newSize, newSize);
    strassenMultiplyRecursive(tempA, B11, M2, newSize);

    // M3 = A11 * (B12 - B22)
    subtractMatrices(B12, B22, tempB, newSize, newSize);
    strassenMultiplyRecursive(A11, tempB, M3, newSize);

    // M4 = A22 * (B21 - B11)
    subtractMatrices(B21, B11, tempB, newSize, newSize);
    strassenMultiplyRecursive(A22, tempB, M4, newSize);

    // M5 = (A11 + A12) * B22
    addMatrices(A11, A12, tempA, newSize, newSize);
    strassenMultiplyRecursive(tempA, B22, M5, newSize);

    // M6 = (A21 - A11) * (B11 + B12)
    subtractMatrices(A21, A11, tempA, newSize, newSize);
    addMatrices(B11, B12, tempB, newSize, newSize);
    strassenMultiplyRecursive(tempA, tempB, M6, newSize);

    // M7 = (A12 - A22) * (B21 + B22)
    subtractMatrices(A12, A22, tempA, newSize, newSize);
    addMatrices(B21, B22, tempB, newSize, newSize);
    strassenMultiplyRecursive(tempA, tempB, M7, newSize);

    // Construct the final matrix C from the intermediate matrices
    for (int i = 0; i < newSize; i++) {
        for (int j = 0; j < newSize; j++) {
            C[i * n + j] = M1[i * newSize + j] + M4[i * newSize + j] - M5[i * newSize + j] + M7[i * newSize + j];
            C[i * n + (j + newSize)] = M3[i * newSize + j] + M5[i * newSize + j];
            C[(i + newSize) * n + j] = M2[i * newSize + j] + M4[i * newSize + j];
            C[(i + newSize) * n + (j + newSize)] = M1[i * newSize + j] - M2[i * newSize + j] + M3[i * newSize + j] + M6[i * newSize + j];
        }
    }

    // Free all allocated memory
    free(A11);
    free(A12);
    free(A21);
    free(A22);
    free(B11);
    free(B12);
    free(B21);
    free(B22);
    free(M1);
    free(M2);
    free(M3);
    free(M4);
    free(M5);
    free(M6);
    free(M7);
    free(tempA);
    free(tempB);
}

