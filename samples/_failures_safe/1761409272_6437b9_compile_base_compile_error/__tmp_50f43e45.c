//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 8

void fill_matrix(int rows, int cols, int matrix[rows][cols]) {
    int counter = 1;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = counter++;
        }
    }
}

void print_matrix(int rows, int cols, int matrix[rows][cols]) {
    int i = 0;
    while (i < rows) {
        int j = 0;
        do {
            printf("%4d", matrix[i][j]);
            j++;
        } while (j < cols);
        printf("\n");
        i++;
    }
}

void transpose_matrix(int rows, int cols, int src[rows][cols], int dest[cols][rows]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            dest[j][i] = src[i][j];
        }
    }
}

void multiply_matrices(int rows1, int cols1, int mat1[rows1][cols1],
                      int rows2, int cols2, int mat2[rows2][cols2],
                      int result[rows1][cols2]) {
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols2; j++) {
            result[i][j] = 0;
            for (int k = 0; k < cols1; k++) {
                int term = mat1[i][k] * mat2[k][j];
                if ((mat1[i][k] > 0 && mat2[k][j] > 0 && mat1[i][k] > INT_MAX / mat2[k][j]) ||
                    (mat1[i][k] < 0 && mat2[k][j] < 0 && mat1[i][k] < INT_MAX / mat2[k][j]) ||
                    (result[i][j] > 0 && term > INT_MAX - result[i][j]) ||
                    (result[i][j] < 0 && term < INT_MIN - result[i][j])) {
                    result[i][j] = 0;
                    break;
                }
                result[i][j] += term;
            }
        }
    }
}

int get_valid_int(const char* prompt, int min, int max) {
    int value;
    char buffer[100];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return min;
        }
        
        if (sscanf(buffer, "%d", &value) == 1) {
            if (value >= min && value <= max) {
                return value;
            }
        }
        printf("Invalid input. Please enter a number between %d and %d.\n", min, max);
    }
}

int main(void) {
    int rows1, cols1, rows2, cols2;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    rows1 = get_valid_int("Enter rows for matrix 1 (1-8): ", 1, MAX_SIZE);
    cols1 = get_valid_int("Enter columns for matrix 1 (1-8): ", 1, MAX_SIZE);
    
    int matrix1[MAX_SIZE][MAX_SIZE];
    fill_matrix(rows1, cols1, matrix1);
    
    printf("\nMatrix 1:\n");
    print_matrix(rows1, cols1, matrix1);
    
    int transposed[MAX_SIZE][MAX_SIZE];
    transpose_matrix(rows1, cols1, matrix1, transposed);
    
    printf("\nTranspose of Matrix 1:\n");
    print_matrix(cols1, rows1, transposed);
    
    printf("\nMatrix Multiplication:\n");
    rows2 = get_valid_int("Enter rows for matrix 2 (1-8): ", 1, MAX_SIZE);
    cols2 = get_valid_int("Enter columns for matrix 2 (1-8): ", 1, MAX_SIZE);
    
    if (cols1 != rows2) {
        printf("Error: Matrix dimensions incompatible for multiplication.\n");
        printf("Columns of first matrix (%d) must equal rows of second matrix (%d).\n", cols1, rows2);
        return 1;
    }
    
    int matrix2[MAX_SIZE][MAX_SIZE];
    fill_matrix(rows2, cols2, matrix2);
    
    printf("\nMatrix 2:\n");
    print_matrix(rows2, cols2, matrix2);
    
    int result[MAX_SIZE][MAX_SIZE];
    multiply_matrices(rows1, cols1, matrix1, rows2, cols2, matrix2, result);
    
    printf("\nMatrix 1 * Matrix 2:\n");
    print_matrix(rows1, cols2, result);
    
    printf("\nElement-wise operations:\n");
    int sum = 0;
    int* ptr = &matrix1[0][0];
    for (int i = 0; i < rows1 * cols1; i++) {
        sum += *ptr++;
    }
    printf("Sum of all elements in Matrix 1: %d\n", sum);
    
    int min_val = matrix1[0][0];
    int max_val = matrix1[0][0];
    int idx = 0;
    while (idx < rows1 *