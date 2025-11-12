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
        if (sscanf(buffer, "%d", &value) == 1 && value >= min_val && value <= max_val) {
            return value;
        }
        printf("Invalid input. Enter integer between %d and %d.\n", min_val, max_val);
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

void multiply_matrices(int rows1, int cols1, int matrix1[MAX_DIM][MAX_DIM],
                      int rows2, int cols2, int matrix2[MAX_DIM][MAX_DIM],
                      int result[MAX_DIM][MAX_DIM]) {
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols2; j++) {
            result[i][j] = 0;
            for (int k = 0; k < cols1; k++) {
                if (matrix1[i][k] > 0 && matrix2[k][j] > 0) {
                    if (matrix1[i][k] > INT_MAX / matrix2[k][j]) {
                        result[i][j] = INT_MAX;
                    } else {
                        result[i][j] += matrix1[i][k] * matrix2[k][j];
                    }
                } else if (matrix1[i][k] < 0 && matrix2[k][j] < 0) {
                    if (matrix1[i][k] < INT_MIN / matrix2[k][j]) {
                        result[i][j] = INT_MAX;
                    } else {
                        result[i][j] += matrix1[i][k] * matrix2[k][j];
                    }
                } else {
                    result[i][j] += matrix1[i][k] * matrix2[k][j];
                }
            }
        }
    }
}

int main(void) {
    int rows1, cols1, rows2, cols2;
    int matrix1[MAX_DIM][MAX_DIM];
    int matrix2[MAX_DIM][MAX_DIM];
    int result[MAX_DIM][MAX_DIM];
    
    printf("Matrix Multiplication Program\n");
    printf("Matrix 1 dimensions:\n");
    rows1 = read_int("Rows (1-10): ", 1, MAX_DIM);
    cols1 = read_int("Columns (1-10): ", 1, MAX_DIM);
    
    printf("Matrix 2 dimensions:\n");
    rows2 = read_int("Rows (1-10): ", 1, MAX_DIM);
    cols2 = read_int("Columns (1-10): ", 1, MAX_DIM);
    
    if (cols1 != rows2) {
        printf("Error: Matrix 1 columns (%d) must equal Matrix 2 rows (%d)\n", cols1, rows2);
        return 1;
    }
    
    printf("Enter Matrix 1 elements:\n");
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols1; j++) {
            matrix1[i][j] = read_int("Element: ", -1000, 1000);
        }
    }
    
    printf("Enter Matrix 2 elements:\n");
    for (int i = 0; i < rows2; i++) {
        for (int j = 0; j < cols2; j++) {
            matrix2[i][j] = read_int("Element: ", -1000, 1000);
        }
    }
    
    printf("\nMatrix 1:\n");
    print_matrix(rows1, cols1, matrix1);
    
    printf("\nMatrix 2:\n");
    print_matrix(rows2, cols2, matrix2);
    
    multiply_matrices(rows1, cols1, matrix1, rows2, cols2, matrix2, result);
    
    printf("\nResult Matrix:\n");
    print_matrix(rows1, cols2, result);
    
    return 0;
}