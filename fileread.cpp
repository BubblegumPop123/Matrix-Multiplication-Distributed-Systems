#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

using namespace std;

// Function to generate a random value between 1 and 10
int generateRandomValue() {
    return rand() % 10 + 1;
}

// Function to initialize a matrix with random values
void initializeMatrix(int** matrix, int rows, int cols) {
    for (int i = 0; i < rows; ++i) {
        matrix[i] = new int[cols];
        for (int j = 0; j < cols; ++j) {
            matrix[i][j] = generateRandomValue();
        }
    }
}

// Function to print a matrix
void printMatrix(int** matrix, int rows, int cols) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

// Function to deallocate memory for a matrix
void deallocateMatrix(int** matrix, int rows) {
    for (int i = 0; i < rows; ++i) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

int main() {
    // Seed the random number generator
    srand(static_cast<unsigned>(time(0)));

    // File input stream
    ifstream inputFile("matrices.txt");

    // Check if the file is open
    if (!inputFile.is_open()) {
        cerr << "Error opening the file." << endl;
        return 1;
    }

    // Declare and initialize variables for matrix dimensions
    int rows[5], cols[5];

    // Dynamically allocate memory for matrices
    int** matrices[5];

    // Read dimensions and initialize matrices
    for (int i = 0; i < 5; ++i) {
        inputFile >> rows[i] >> cols[i];

        // Validate dimensions
        if (rows[i] < 20 || rows[i] > 50 || cols[i] < 20 || cols[i] > 50) {
            cerr << "Invalid dimensions for matrix " << i + 1 << "." << endl;
            return 1;
        }

        // Dynamically allocate memory for each matrix
        matrices[i] = new int* [rows[i]];

        // Initialize matrices
        initializeMatrix(matrices[i], rows[i], cols[i]);

        // Print each matrix
        cout << "Matrix " << i + 1 << ":" << endl;
        printMatrix(matrices[i], rows[i], cols[i]);
        cout << endl;
    }

    // Deallocate memory for matrices
    for (int i = 0; i < 5; ++i) {
        deallocateMatrix(matrices[i], rows[i]);
    }

    // Close the file
    inputFile.close();

    return 0;
}
