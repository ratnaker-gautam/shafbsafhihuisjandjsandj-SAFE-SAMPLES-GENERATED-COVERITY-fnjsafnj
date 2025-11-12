//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

enum Operation {
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_TRANSPOSE,
    OP_DETERMINANT
};

void print_matrix(int rows, int cols, int matrix[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%6d", matrix[i][j]);
        }
        printf("\n");
    }
}

int read_int(const char* prompt, int min, int max) {
    int value;
    char buffer[100];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Error reading input\n");
            exit(1);
        }
        
        if (sscanf(buffer, "%d", &value) == 1 && value >= min && value <= max) {
            return value;
        }
        printf("Invalid input. Please enter an integer between %d and %d\n", min, max);
    }
}

void read_matrix(int rows, int cols, int matrix[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            char prompt[50];
            snprintf(prompt, sizeof(prompt), "Enter element [%d][%d]: ", i, j);
            matrix[i][j] = read_int(prompt, -1000, 1000);
        }
    }
}

void matrix_add(int rows, int cols, int A[rows][cols], int B[rows][cols], int result[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[i][j] = A[i][j] + B[i][j];
        }
    }
}

void matrix_subtract(int rows, int cols, int A[rows][cols], int B[rows][cols], int result[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[i][j] = A[i][j] - B[i][j];
        }
    }
}

void matrix_multiply(int rowsA, int colsA, int colsB, int A[rowsA][colsA], int B[colsA][colsB], int result[rowsA][colsB]) {
    for (int i = 0; i < rowsA; i++) {
        for (int j = 0; j < colsB; j++) {
            result[i][j] = 0;
            for (int k = 0; k < colsA; k++) {
                result[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void matrix_transpose(int rows, int cols, int matrix[rows][cols], int result[cols][rows]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[j][i] = matrix[i][j];
        }
    }
}

int matrix_determinant_2x2(int matrix[2][2]) {
    return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
}

int matrix_determinant_3x3(int matrix[3][3]) {
    int det = 0;
    det += matrix[0][0] * (matrix[1][1] * matrix[2][2] - matrix[1][2] * matrix[2][1]);
    det -= matrix[0][1] * (matrix[1][0] * matrix[2][2] - matrix[1][2] * matrix[2][0]);
    det += matrix[0][2] * (matrix[1][0] * matrix[2][1] - matrix[1][1] * matrix[2][0]);
    return det;
}

int main() {
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    int operation = read_int("Select operation (0=Add, 1=Subtract, 2=Multiply, 3=Transpose, 4=Determinant): ", 0, 4);
    
    switch (operation) {
        case OP_ADD:
        case OP_SUBTRACT: {
            int rows = read_int("Enter number of rows: ", 1, 10);
            int cols = read_int("Enter number of columns: ", 1, 10);
            
            int A[rows][cols];
            int B[rows][cols];
            int result[rows][cols];
            
            printf("Enter matrix A:\n");
            read_matrix(rows, cols, A);
            printf("Enter matrix B:\n");
            read_matrix(rows, cols, B);
            
            if (operation == OP_ADD) {
                matrix_add(rows, cols, A, B, result);
                printf("Matrix A + B:\n");
            } else {
                matrix_subtract(rows, cols, A, B, result);
                printf("Matrix A - B:\n");
            }
            print_matrix(rows, cols, result