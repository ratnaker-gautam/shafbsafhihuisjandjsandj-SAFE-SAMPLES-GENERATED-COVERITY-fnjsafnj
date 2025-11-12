//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ROWS 10
#define MAX_COLS 10
#define MIN_DIM 1

#define MATRIX_INIT(mat, rows, cols) \
    do { \
        for (int i = 0; i < rows; i++) { \
            for (int j = 0; j < cols; j++) { \
                mat[i][j] = (i == j) ? 1 : 0; \
            } \
        } \
    } while (0)

#define MATRIX_PRINT(mat, rows, cols) \
    do { \
        for (int i = 0; i < rows; i++) { \
            for (int j = 0; j < cols; j++) { \
                printf("%3d ", mat[i][j]); \
            } \
            printf("\n"); \
        } \
    } while (0)

#define MATRIX_ADD(a, b, result, rows, cols) \
    do { \
        for (int i = 0; i < rows; i++) { \
            for (int j = 0; j < cols; j++) { \
                result[i][j] = a[i][j] + b[i][j]; \
            } \
        } \
    } while (0)

#define MATRIX_MULTIPLY(a, b, result, rows, cols, inner) \
    do { \
        for (int i = 0; i < rows; i++) { \
            for (int j = 0; j < cols; j++) { \
                result[i][j] = 0; \
                for (int k = 0; k < inner; k++) { \
                    result[i][j] += a[i][k] * b[k][j]; \
                } \
            } \
        } \
    } while (0)

int read_int(const char* prompt, int min, int max) {
    char buffer[32];
    long value;
    char* endptr;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        
        value = strtol(buffer, &endptr, 10);
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter a valid integer.\n");
            continue;
        }
        
        if (value < min || value > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        
        return (int)value;
    }
}

int main(void) {
    int matrix1[MAX_ROWS][MAX_COLS];
    int matrix2[MAX_ROWS][MAX_COLS];
    int result[MAX_ROWS][MAX_COLS];
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    int rows1 = read_int("Enter rows for matrix 1 (1-10): ", MIN_DIM, MAX_ROWS);
    if (rows1 == -1) return 1;
    
    int cols1 = read_int("Enter columns for matrix 1 (1-10): ", MIN_DIM, MAX_COLS);
    if (cols1 == -1) return 1;
    
    int rows2 = read_int("Enter rows for matrix 2 (1-10): ", MIN_DIM, MAX_ROWS);
    if (rows2 == -1) return 1;
    
    int cols2 = read_int("Enter columns for matrix 2 (1-10): ", MIN_DIM, MAX_COLS);
    if (cols2 == -1) return 1;
    
    printf("\nInitializing matrix 1 as identity matrix...\n");
    MATRIX_INIT(matrix1, rows1, cols1);
    printf("Matrix 1:\n");
    MATRIX_PRINT(matrix1, rows1, cols1);
    
    printf("\nInitializing matrix 2 as identity matrix...\n");
    MATRIX_INIT(matrix2, rows2, cols2);
    printf("Matrix 2:\n");
    MATRIX_PRINT(matrix2, rows2, cols2);
    
    printf("\nPerforming matrix addition...\n");
    if (rows1 == rows2 && cols1 == cols2) {
        MATRIX_ADD(matrix1, matrix2, result, rows1, cols1);
        printf("Result of addition:\n");
        MATRIX_PRINT(result, rows1, cols1);
    } else {
        printf("Cannot add matrices: dimensions don't match.\n");
    }
    
    printf("\nPerforming matrix multiplication...\n");
    if (cols1 == rows2) {
        MATRIX_MULTIPLY(matrix1, matrix2, result, rows1, cols2, cols1);
        printf("Result of multiplication:\n");
        MATRIX_PRINT(result, rows1, cols2);
    } else {
        printf("Cannot multiply matrices: inner dimensions don't match.\n");
    }
    
    printf("\nDemonstrating scalar operations...\n");
    int scalar = read_int("Enter a scalar value (-100 to 100): ", -100, 100);
    if (scalar == -1) return 1;
    
    printf("Matrix 1 multiplied by scalar %d:\n", scalar);
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j <