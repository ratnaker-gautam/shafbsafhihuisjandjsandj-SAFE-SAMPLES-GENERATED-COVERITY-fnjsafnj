//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ROWS 10
#define MAX_COLS 10
#define MIN_DIM 1

#define MATRIX_INIT(m, r, c) \
    do { \
        for (int i = 0; i < r; i++) { \
            for (int j = 0; j < c; j++) { \
                m[i][j] = 0; \
            } \
        } \
    } while (0)

#define MATRIX_PRINT(m, r, c) \
    do { \
        for (int i = 0; i < r; i++) { \
            for (int j = 0; j < c; j++) { \
                printf("%3d ", m[i][j]); \
            } \
            printf("\n"); \
        } \
    } while (0)

#define MATRIX_ADD(a, b, res, r, c) \
    do { \
        for (int i = 0; i < r; i++) { \
            for (int j = 0; j < c; j++) { \
                res[i][j] = a[i][j] + b[i][j]; \
            } \
        } \
    } while (0)

#define MATRIX_MULT(a, b, res, r1, c1, c2) \
    do { \
        for (int i = 0; i < r1; i++) { \
            for (int j = 0; j < c2; j++) { \
                res[i][j] = 0; \
                for (int k = 0; k < c1; k++) { \
                    res[i][j] += a[i][k] * b[k][j]; \
                } \
            } \
        } \
    } while (0)

int read_int(const char* prompt, int min, int max) {
    int value;
    char buffer[100];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
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

void read_matrix(int matrix[MAX_ROWS][MAX_COLS], int rows, int cols) {
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            char prompt[50];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            matrix[i][j] = read_int(prompt, INT_MIN, INT_MAX);
        }
    }
}

int main(void) {
    int rows1, cols1, rows2, cols2;
    int matrix1[MAX_ROWS][MAX_COLS];
    int matrix2[MAX_ROWS][MAX_COLS];
    int result[MAX_ROWS][MAX_COLS];
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    rows1 = read_int("Enter rows for first matrix (1-10): ", MIN_DIM, MAX_ROWS);
    cols1 = read_int("Enter columns for first matrix (1-10): ", MIN_DIM, MAX_COLS);
    
    if (rows1 == -1 || cols1 == -1) {
        return EXIT_FAILURE;
    }
    
    read_matrix(matrix1, rows1, cols1);
    
    printf("\nFirst matrix:\n");
    MATRIX_PRINT(matrix1, rows1, cols1);
    
    rows2 = read_int("\nEnter rows for second matrix (1-10): ", MIN_DIM, MAX_ROWS);
    cols2 = read_int("Enter columns for second matrix (1-10): ", MIN_DIM, MAX_COLS);
    
    if (rows2 == -1 || cols2 == -1) {
        return EXIT_FAILURE;
    }
    
    read_matrix(matrix2, rows2, cols2);
    
    printf("\nSecond matrix:\n");
    MATRIX_PRINT(matrix2, rows2, cols2);
    
    printf("\nAvailable operations:\n");
    printf("1. Matrix Addition\n");
    printf("2. Matrix Multiplication\n");
    
    int choice = read_int("Select operation (1-2): ", 1, 2);
    if (choice == -1) {
        return EXIT_FAILURE;
    }
    
    MATRIX_INIT(result, MAX_ROWS, MAX_COLS);
    
    if (choice == 1) {
        if (rows1 != rows2 || cols1 != cols2) {
            printf("Error: Matrices must have same dimensions for addition.\n");
            return EXIT_FAILURE;
        }
        
        MATRIX_ADD(matrix1, matrix2, result, rows1, cols1);
        printf("\nResult of addition:\n");
        MATRIX_PRINT(result, rows1, cols1);
    } else {
        if (cols