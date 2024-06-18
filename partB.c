//part B


#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

#define MAX_DIM 50
#define MAX_MATRICES 7

MPI_Status status;

void printMatrix(double *matrix, int rows, int cols, const char *label) {
    printf("%s:\n", label);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%.0f\t", matrix[i * cols + j]);
        }
        printf("\n");
    }
    printf("\n");
}

int main(int argc, char **argv) {
    int processCount, processId, slaveTaskCount, source, dest, rows, offset;
    int matrixCount = 0;
    double start_time, end_time;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &processId);
    MPI_Comm_size(MPI_COMM_WORLD, &processCount);

    slaveTaskCount = processCount - 1;

    double matrices[MAX_MATRICES][MAX_DIM][MAX_DIM];
    double result[MAX_DIM][MAX_DIM];
    double localResult[MAX_DIM][MAX_DIM];

      // Start time measurement
    if (processId == 0) {
        start_time = MPI_Wtime();
    }

    if (processId == 0) {
        FILE *file = fopen("matrices.txt", "r");
        if (file == NULL) {
            printf("Error opening file.\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        // Read matrix dimensions from the file
        while (matrixCount < MAX_MATRICES) {
            int rows, cols;
            if (fscanf(file, "%d X %d", &rows, &cols) != 2) {
                break;
            }

            // Check for uniqueness of dimensions
            for (int i = 0; i < matrixCount; i++) {
                if (matrices[i][0][0] == rows && matrices[i][0][1] == cols) {
                    printf("Error: Duplicate matrix dimensions.\n");
                    MPI_Abort(MPI_COMM_WORLD, 1);
                }
            }

            // Initialize the matrix with random values
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                    matrices[matrixCount][i][j] = rand() % 10;
                }
            }

            matrixCount++;
        }
        fclose(file);

        printf("\n\t\tMatrix - Matrix Multiplication using MPI\n");

        for (int m = 0; m < matrixCount; m++) {
            printMatrix(&matrices[m][0][0], MAX_DIM, MAX_DIM, "Matrix");
        }

        rows = MAX_DIM / slaveTaskCount;
        offset = 0;

        for (dest = 1; dest <= slaveTaskCount; dest++) {
            MPI_Send(&offset, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
            MPI_Send(&rows, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
            MPI_Send(&matrixCount, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);  // Send matrixCount to slaves
            for (int m = 0; m < matrixCount; m++) {
                MPI_Send(&matrices[m][0][0], MAX_DIM * MAX_DIM, MPI_DOUBLE, dest, 1, MPI_COMM_WORLD);
            }
            offset = offset + rows;
        }

        for (int i = 1; i <= slaveTaskCount; i++) {
            source = i;
            MPI_Recv(&offset, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
            MPI_Recv(&rows, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
            MPI_Recv(&localResult[offset][0], rows * MAX_DIM, MPI_DOUBLE, source, 2, MPI_COMM_WORLD, &status);
        }

        // Print the result matrix
        printMatrix(&localResult[0][0], MAX_DIM, MAX_DIM, "Result Matrix C");
    }

    if (processId > 0) {
        source = 0;
        MPI_Recv(&offset, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&rows, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&matrixCount, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);  // Receive matrixCount from root

        // Matrix multiplication
        for (int m = 0; m < matrixCount; m++) {
            MPI_Recv(&matrices[m][0][0], MAX_DIM * MAX_DIM, MPI_DOUBLE, source, 1, MPI_COMM_WORLD, &status);
        }

        for (int k = 0; k < MAX_DIM; k++) {
            for (int i = 0; i < rows; i++) {
                localResult[i][k] = 0.0;
                for (int j = 0; j < MAX_DIM; j++) {
                    for (int m = 0; m < matrixCount; m++) {
                        localResult[i][k] += matrices[m][i + offset][j] * matrices[m][j][k];
                    }
                }
            }
        }

        MPI_Send(&offset, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
        MPI_Send(&rows, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
        MPI_Send(&localResult[0][0], rows * MAX_DIM, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD);
    }

    if (processId == 0) {
        end_time = MPI_Wtime();
        printf("Execution Time: %f seconds\n", end_time - start_time);
        printf("Processors: %d", processCount);
    }


    MPI_Finalize();
    return 0;
}
