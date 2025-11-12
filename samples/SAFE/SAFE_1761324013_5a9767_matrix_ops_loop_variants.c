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

void multiply_matrices(int r1, int c1, int m1[r1][c1], int r2, int c2, int m2[r2][c2], int result[r1][c2]) {
    if (c1 != r2) {
        return;
    }
    
    for (int i = 0; i < r1; i++) {
        for (int j = 0; j < c2; j++) {
            result[i][j] = 0;
            for (int k = 0; k < c1; k++) {
                if (m1[i][k] > 0 && m2[k][j] > 0) {
                    if (m1[i][k] > INT_MAX / m2[k][j]) {
                        result[i][j] = 0;
                        break;
                    }
                } else if (m1[i][k] < 0 && m2[k][j] < 0) {
                    if (m1[i][k] < INT_MAX / m2[k][j]) {
                        result[i][j] = 0;
                        break;
                    }
                }
                result[i][j] += m1[i][k] * m2[k][j];
            }
        }
    }
}

int read_positive_int(const char* prompt) {
    char buffer[32];
    int value;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        
        if (sscanf(buffer, "%d", &value) != 1) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if (value <= 0 || value > MAX_SIZE) {
            printf("Value must be between 1 and %d.\n", MAX_SIZE);
            continue;
        }
        
        return value;
    }
}

int main(void) {
    int rows1, cols1, rows2, cols2;
    
    printf("Matrix Operations Program\n");
    printf("Maximum matrix size: %dx%d\n", MAX_SIZE, MAX_SIZE);
    
    rows1 = read_positive_int("Enter rows for matrix 1: ");
    if (rows1 == -1) return 1;
    cols1 = read_positive_int("Enter columns for matrix 1: ");
    if (cols1 == -1) return 1;
    
    rows2 = read_positive_int("Enter rows for matrix 2: ");
    if (rows2 == -1) return 1;
    cols2 = read_positive_int("Enter columns for matrix 2: ");
    if (cols2 == -1) return 1;
    
    int matrix1[rows1][cols1];
    int matrix2[rows2][cols2];
    
    fill_matrix(rows1, cols1, matrix1);
    fill_matrix(rows2, cols2, matrix2);
    
    printf("\nMatrix 1:\n");
    print_matrix(rows1, cols1, matrix1);
    
    printf("\nMatrix 2:\n");
    print_matrix(rows2, cols2, matrix2);
    
    if (rows1 == cols2) {
        int transpose[cols2][rows1];
        transpose_matrix(rows2, cols2, matrix2, transpose);
        
        printf("\nTranspose of Matrix 2:\n");
        print_matrix(cols2, rows1, transpose);
    }
    
    if (cols1 == rows2) {
        int product[rows1][cols2];
        multiply_matrices(rows1, cols1, matrix1, rows2, cols2, matrix2, product);
        
        printf("\nMatrix 1 * Matrix 2:\n");
        print_matrix(rows1, cols2, product);
    } else {
        printf("\nMatrix multiplication not possible (columns of matrix 1 must equal rows of matrix 2).\n");
    }
    
    return 0;
}