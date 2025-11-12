//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 10

int read_int(const char* prompt, int min, int max) {
    char buffer[32];
    int value;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return min;
        }
        
        char* endptr;
        long temp = strtol(buffer, &endptr, 10);
        
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (temp < min || temp > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        
        value = (int)temp;
        break;
    }
    
    return value;
}

void read_matrix(int rows, int cols, int matrix[MAX_DIM][MAX_DIM]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            char prompt[64];
            snprintf(prompt, sizeof(prompt), "Enter element [%d][%d]: ", i, j);
            matrix[i][j] = read_int(prompt, INT_MIN, INT_MAX);
        }
    }
}

void print_matrix(int rows, int cols, int matrix[MAX_DIM][MAX_DIM]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%6d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void add_matrices(int rows, int cols, int A[MAX_DIM][MAX_DIM], int B[MAX_DIM][MAX_DIM], int result[MAX_DIM][MAX_DIM]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if ((A[i][j] > 0 && B[i][j] > INT_MAX - A[i][j]) || 
                (A[i][j] < 0 && B[i][j] < INT_MIN - A[i][j])) {
                result[i][j] = (A[i][j] > 0) ? INT_MAX : INT_MIN;
            } else {
                result[i][j] = A[i][j] + B[i][j];
            }
        }
    }
}

void multiply_matrices(int rowsA, int colsA, int A[MAX_DIM][MAX_DIM], int rowsB, int colsB, int B[MAX_DIM][MAX_DIM], int result[MAX_DIM][MAX_DIM]) {
    for (int i = 0; i < rowsA; i++) {
        for (int j = 0; j < colsB; j++) {
            long sum = 0;
            for (int k = 0; k < colsA; k++) {
                long product = (long)A[i][k] * (long)B[k][j];
                if (product > INT_MAX - sum || product < INT_MIN - sum) {
                    sum = (product > 0) ? INT_MAX : INT_MIN;
                    break;
                }
                sum += product;
            }
            result[i][j] = (sum > INT_MAX) ? INT_MAX : (sum < INT_MIN) ? INT_MIN : (int)sum;
        }
    }
}

void transpose_matrix(int rows, int cols, int matrix[MAX_DIM][MAX_DIM], int result[MAX_DIM][MAX_DIM]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[j][i] = matrix[i][j];
        }
    }
}

int main() {
    int choice;
    int rowsA, colsA, rowsB, colsB;
    int matrixA[MAX_DIM][MAX_DIM];
    int matrixB[MAX_DIM][MAX_DIM];
    int result[MAX_DIM][MAX_DIM];
    
    printf("Matrix Operations Program\n");
    printf("1. Matrix Addition\n");
    printf("2. Matrix Multiplication\n");
    printf("3. Matrix Transpose\n");
    
    choice = read_int("Enter your choice (1-3): ", 1, 3);
    
    switch (choice) {
        case 1:
            rowsA = read_int("Enter number of rows for matrix A: ", 1, MAX_DIM);
            colsA = read_int("Enter number of columns for matrix A: ", 1, MAX_DIM);
            rowsB = read_int("Enter number of rows for matrix B: ", 1, MAX_DIM);
            colsB = read_int("Enter number of columns for matrix B: ", 1, MAX_DIM);
            
            if (rowsA != rowsB || colsA != colsB) {
                printf("Error: Matrices must have the same dimensions for addition.\n");
                return 1;
            }
            
            printf("Enter elements for matrix A:\n");
            read_matrix(rowsA, colsA, matrixA);
            printf("Enter elements for matrix B:\n");
            read_matrix(rowsB, colsB, matrixB);
            
            add_matrices(rowsA, colsA, matrixA, matrixB, result);
            
            printf("\nMatrix A:\n");
            print_matrix