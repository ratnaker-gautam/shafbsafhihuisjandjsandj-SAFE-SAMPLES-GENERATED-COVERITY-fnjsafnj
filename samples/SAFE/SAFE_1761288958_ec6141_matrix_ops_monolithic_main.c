//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 10

int read_int(const char* prompt, int min, int max) {
    int value;
    char buffer[100];
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return min;
        }
        if (sscanf(buffer, "%d", &value) == 1 && value >= min && value <= max) {
            return value;
        }
        printf("Invalid input. Enter integer between %d and %d.\n", min, max);
    }
}

void fill_matrix(int rows, int cols, int matrix[MAX_DIM][MAX_DIM]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = read_int("Enter matrix element: ", INT_MIN, INT_MAX);
        }
    }
}

void print_matrix(int rows, int cols, int matrix[MAX_DIM][MAX_DIM]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%6d", matrix[i][j]);
        }
        printf("\n");
    }
}

void add_matrices(int rows, int cols, int a[MAX_DIM][MAX_DIM], int b[MAX_DIM][MAX_DIM], int result[MAX_DIM][MAX_DIM]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if ((b[i][j] > 0 && a[i][j] > INT_MAX - b[i][j]) || (b[i][j] < 0 && a[i][j] < INT_MIN - b[i][j])) {
                result[i][j] = 0;
            } else {
                result[i][j] = a[i][j] + b[i][j];
            }
        }
    }
}

void multiply_matrices(int rows1, int cols1, int a[MAX_DIM][MAX_DIM], int rows2, int cols2, int b[MAX_DIM][MAX_DIM], int result[MAX_DIM][MAX_DIM]) {
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols2; j++) {
            long sum = 0;
            for (int k = 0; k < cols1; k++) {
                long product = (long)a[i][k] * (long)b[k][j];
                if (product > INT_MAX - sum || product < INT_MIN - sum) {
                    sum = 0;
                    break;
                }
                sum += product;
            }
            if (sum > INT_MAX || sum < INT_MIN) {
                result[i][j] = 0;
            } else {
                result[i][j] = (int)sum;
            }
        }
    }
}

int main() {
    int rows1, cols1, rows2, cols2;
    int matrix1[MAX_DIM][MAX_DIM];
    int matrix2[MAX_DIM][MAX_DIM];
    int result[MAX_DIM][MAX_DIM];
    
    printf("Matrix Operations Program\n");
    
    rows1 = read_int("Enter rows for matrix 1 (1-10): ", 1, MAX_DIM);
    cols1 = read_int("Enter columns for matrix 1 (1-10): ", 1, MAX_DIM);
    
    printf("Enter elements for matrix 1:\n");
    fill_matrix(rows1, cols1, matrix1);
    
    rows2 = read_int("Enter rows for matrix 2 (1-10): ", 1, MAX_DIM);
    cols2 = read_int("Enter columns for matrix 2 (1-10): ", 1, MAX_DIM);
    
    printf("Enter elements for matrix 2:\n");
    fill_matrix(rows2, cols2, matrix2);
    
    printf("\nMatrix 1:\n");
    print_matrix(rows1, cols1, matrix1);
    
    printf("\nMatrix 2:\n");
    print_matrix(rows2, cols2, matrix2);
    
    printf("\nMatrix Addition:\n");
    if (rows1 == rows2 && cols1 == cols2) {
        add_matrices(rows1, cols1, matrix1, matrix2, result);
        print_matrix(rows1, cols1, result);
    } else {
        printf("Cannot add: matrices have different dimensions.\n");
    }
    
    printf("\nMatrix Multiplication:\n");
    if (cols1 == rows2) {
        multiply_matrices(rows1, cols1, matrix1, rows2, cols2, matrix2, result);
        print_matrix(rows1, cols2, result);
    } else {
        printf("Cannot multiply: columns of matrix 1 must equal rows of matrix 2.\n");
    }
    
    return 0;
}