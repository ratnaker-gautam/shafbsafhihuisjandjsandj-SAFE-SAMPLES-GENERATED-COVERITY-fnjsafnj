//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 10

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
    if (cols1 != rows2) return;
    
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols2; j++) {
            result[i][j] = 0;
            for (int k = 0; k < cols1; k++) {
                if (mat1[i][k] > 0 && mat2[k][j] > 0) {
                    if (mat1[i][k] > INT_MAX / mat2[k][j]) continue;
                }
                result[i][j] += mat1[i][k] * mat2[k][j];
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
            continue;
        }
        
        char* endptr;
        long val = strtol(buffer, &endptr, 10);
        if (endptr == buffer || *endptr != '\n') {
            continue;
        }
        if (val < min || val > max) {
            continue;
        }
        value = (int)val;
        break;
    }
    return value;
}

int main() {
    int rows1, cols1, rows2, cols2;
    
    printf("Matrix Operations Program\n");
    
    rows1 = get_valid_int("Enter rows for matrix 1 (1-10): ", 1, MAX_SIZE);
    cols1 = get_valid_int("Enter columns for matrix 1 (1-10): ", 1, MAX_SIZE);
    rows2 = get_valid_int("Enter rows for matrix 2 (1-10): ", 1, MAX_SIZE);
    cols2 = get_valid_int("Enter columns for matrix 2 (1-10): ", 1, MAX_SIZE);
    
    int matrix1[MAX_SIZE][MAX_SIZE];
    int matrix2[MAX_SIZE][MAX_SIZE];
    int result[MAX_SIZE][MAX_SIZE];
    
    fill_matrix(rows1, cols1, matrix1);
    fill_matrix(rows2, cols2, matrix2);
    
    printf("\nMatrix 1:\n");
    print_matrix(rows1, cols1, matrix1);
    
    printf("\nMatrix 2:\n");
    print_matrix(rows2, cols2, matrix2);
    
    printf("\nTranspose of Matrix 1:\n");
    int transpose1[MAX_SIZE][MAX_SIZE];
    transpose_matrix(rows1, cols1, matrix1, transpose1);
    print_matrix(cols1, rows1, transpose1);
    
    printf("\nTranspose of Matrix 2:\n");
    int transpose2[MAX_SIZE][MAX_SIZE];
    transpose_matrix(rows2, cols2, matrix2, transpose2);
    print_matrix(cols2, rows2, transpose2);
    
    if (cols1 == rows2) {
        printf("\nMatrix Multiplication (Matrix1 * Matrix2):\n");
        multiply_matrices(rows1, cols1, matrix1, rows2, cols2, matrix2, result);
        print_matrix(rows1, cols2, result);
    } else {
        printf("\nCannot multiply matrices: columns of matrix1 != rows of matrix2\n");
    }
    
    if (cols2 == rows1) {
        printf("\nMatrix Multiplication (Matrix2 * Matrix1):\n");
        multiply_matrices(rows2, cols2, matrix2, rows1, cols1, matrix1, result);
        print_matrix(rows2, cols1, result);
    } else {
        printf("\nCannot multiply matrices: columns of matrix2 != rows of matrix1\n");
    }
    
    return 0;
}