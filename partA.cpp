//part A

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <climits>
#include <chrono>

using namespace std;
using namespace std::chrono;

int generateRandomValue() {
    return rand() % 10 + 1;
}

void initializeMatrix(int** &matrix, int rows, int cols) {
    matrix = new int*[rows];
    for (int i = 0; i < rows; ++i) {
        matrix[i] = new int[cols];
        for (int j = 0; j < cols; ++j) {
            matrix[i][j] = generateRandomValue();
        }
    }
}

void printMatrix(int** matrix, int rows, int cols) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

void deallocateMatrix(int** &matrix, int rows) {
    for (int i = 0; i < rows; ++i) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

int optimalMatrixMultiplicationOrder(int rows[], int cols[], int n) {
    int** dp = new int*[n];
    for (int i = 0; i < n; ++i) {
        dp[i] = new int[n]();
    }

    for (int len = 2; len < n; ++len) {
        for (int i = 1; i < n - len + 1; ++i) {
            int j = i + len - 1;
            dp[i][j] = INT_MAX;
            for (int k = i; k <= j - 1; ++k) {
                int cost = dp[i][k] + dp[k + 1][j] + rows[i - 1] * cols[k] * cols[j];
                if (cost < dp[i][j]) {
                    dp[i][j] = cost;
                }
            }
        }
    }

    int result = dp[1][n - 1];

    for (int i = 0; i < n; ++i) {
        delete[] dp[i];
    }
    delete[] dp;

    return result;
}

int main() {
    srand(static_cast<unsigned>(time(0)));

    auto start = high_resolution_clock::now();

    ifstream inputFile("matrices.txt");
    if (!inputFile.is_open()) {
        cerr << "Error opening the file." << endl;
        return 1;
    }

    int rows[5], cols[5];
    int** matrices[5];
    char x; // For reading the 'X' character in the input

    for (int i = 0; i < 5; ++i) {
        inputFile >> rows[i] >> x >> cols[i];
        if (rows[i] < 20 || rows[i] > 50 || cols[i] < 20 || cols[i] > 50 || rows[i] == cols[i]) {
            cerr << "Invalid dimensions for matrix " << i + 1 << "." << endl;
            return 1;
        }
        initializeMatrix(matrices[i], rows[i], cols[i]);
        cout << "Matrix " << i + 1 << ":" << endl;
        printMatrix(matrices[i], rows[i], cols[i]);
        cout << endl;
    }

    int optimalScalarMultiplications = optimalMatrixMultiplicationOrder(rows, cols, 5);
    cout << "Optimal Scalar Multiplications: " << optimalScalarMultiplications << endl;

    for (int i = 0; i < 5; ++i) {
        deallocateMatrix(matrices[i], rows[i]);
    }

    inputFile.close();

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Execution time: " << duration.count() / 1000000.0 << " seconds" << endl;

    return 0;
}
