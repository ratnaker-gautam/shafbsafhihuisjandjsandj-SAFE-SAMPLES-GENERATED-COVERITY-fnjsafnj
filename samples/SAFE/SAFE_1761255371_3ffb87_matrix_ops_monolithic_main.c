//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 10

int read_int(const char* prompt, int min_val, int max_val) {
    char buffer[32];
    int value;
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return min_val;
        }
        if (sscanf(buffer, "%d", &value) == 1) {
            if (value >= min_val && value <= max_val) {
                return value;
            }
        }
        printf("Invalid input. Enter value between %d and %d.\n", min_val, max_val);
    }
}

void fill_matrix(int rows, int cols, int matrix[MAX_DIM][MAX_DIM]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = read_int("Enter matrix element: ", INT_MIN / MAX_DIM, INT_MAX / MAX_DIM);
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
            if ((b[i][j] > 0 && a[i][j] > INT_MAX - b[i][j]) || 
                (b[i][j] < 0 && a[i][j] < INT_MIN - b[i][j])) {
                result[i][j] = 0;
            } else {
                result[i][j] = a[i][j] + b[i][j];
            }
        }
    }
}

void multiply_matrices(int a_rows, int a_cols, int b_cols, int a[MAX_DIM][MAX_DIM], int b[MAX_DIM][MAX_DIM], int result[MAX_DIM][MAX_DIM]) {
    for (int i = 0; i < a_rows; i++) {
        for (int j = 0; j < b_cols; j++) {
            int sum = 0;
            for (int k = 0; k < a_cols; k++) {
                long long product = (long long)a[i][k] * b[k][j];
                if (product > INT_MAX || product < INT_MIN) {
                    sum = 0;
                    break;
                }
                if ((product > 0 && sum > INT_MAX - product) || 
                    (product < 0 && sum < INT_MIN - product)) {
                    sum = 0;
                    break;
                }
                sum += (int)product;
            }
            result[i][j] = sum;
        }
    }
}

int main(void) {
    int rows1, cols1, rows2, cols2;
    int matrix1[MAX_DIM][MAX_DIM];
    int matrix2[MAX_DIM][MAX_DIM];
    int result[MAX_DIM][MAX_DIM];
    
    printf("Matrix Operations Program\n");
    
    rows1 = read_int("Enter rows for matrix 1 (1-10): ", 1, MAX_DIM);
    cols1 = read_int("Enter columns for matrix 1 (1-10): ", 1, MAX_DIM);
    
    printf("Filling matrix 1:\n");
    fill_matrix(rows1, cols1, matrix1);
    
    rows2 = read_int("Enter rows for matrix 2 (1-10): ", 1, MAX_DIM);
    cols2 = read_int("Enter columns for matrix 2 (1-10): ", 1, MAX_DIM);
    
    printf("Filling matrix 2:\n");
    fill_matrix(rows2, cols2, matrix2);
    
    printf("\nMatrix 1:\n");
    print_matrix(rows1, cols1, matrix1);
    
    printf("\nMatrix 2:\n");
    print_matrix(rows2, cols2, matrix2);
    
    if (rows1 == rows2 && cols1 == cols2) {
        printf("\nMatrix Addition Result:\n");
        add_matrices(rows1, cols1, matrix1, matrix2, result);
        print_matrix(rows1, cols1, result);
    } else {
        printf("\nMatrices cannot be added (dimensions differ).\n");
    }
    
    if (cols1 == rows2) {
        printf("\nMatrix Multiplication Result:\n");
        multiply_matrices(rows1, cols1, cols2, matrix1, matrix2, result);
        print_matrix(rows1, cols2, result);
    } else {
        printf("\nMatrices cannot be multiplied (columns of first != rows of second).\n");
    }
    
    return 0;
}