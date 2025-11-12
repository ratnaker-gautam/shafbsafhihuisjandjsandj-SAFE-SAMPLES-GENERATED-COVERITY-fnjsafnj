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
        for (int i = 0; i < rows; i++) \
            for (int j = 0; j < cols; j++) \
                mat[i][j] = 0; \
    } while (0)

#define MATRIX_PRINT(mat, rows, cols) \
    do { \
        for (int i = 0; i < rows; i++) { \
            for (int j = 0; j < cols; j++) \
                printf("%6d ", mat[i][j]); \
            printf("\n"); \
        } \
    } while (0)

#define MATRIX_ADD(dest, src1, src2, rows, cols) \
    do { \
        for (int i = 0; i < rows; i++) \
            for (int j = 0; j < cols; j++) \
                dest[i][j] = src1[i][j] + src2[i][j]; \
    } while (0)

#define MATRIX_MULT(dest, mat1, r1, c1, mat2, r2, c2) \
    do { \
        for (int i = 0; i < r1; i++) { \
            for (int j = 0; j < c2; j++) { \
                dest[i][j] = 0; \
                for (int k = 0; k < c1; k++) \
                    dest[i][j] += mat1[i][k] * mat2[k][j]; \
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
    int rows1, cols1, rows2, cols2;
    int operation;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    rows1 = read_int("Enter rows for matrix 1 (1-10): ", MIN_DIM, MAX_ROWS);
    if (rows1 == -1) return 1;
    cols1 = read_int("Enter columns for matrix 1 (1-10): ", MIN_DIM, MAX_COLS);
    if (cols1 == -1) return 1;
    
    printf("Enter elements for matrix 1:\n");
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols1; j++) {
            char prompt[32];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            matrix1[i][j] = read_int(prompt, INT_MIN, INT_MAX);
            if (matrix1[i][j] == -1) return 1;
        }
    }
    
    printf("\nMatrix 1:\n");
    MATRIX_PRINT(matrix1, rows1, cols1);
    
    operation = read_int("\nSelect operation (1=Add, 2=Multiply): ", 1, 2);
    if (operation == -1) return 1;
    
    if (operation == 1) {
        rows2 = read_int("Enter rows for matrix 2 (1-10): ", MIN_DIM, MAX_ROWS);
        if (rows2 == -1) return 1;
        cols2 = read_int("Enter columns for matrix 2 (1-10): ", MIN_DIM, MAX_COLS);
        if (cols2 == -1) return 1;
        
        if (rows1 != rows2 || cols1 != cols2) {
            printf("Error: Matrices must have same dimensions for addition.\n");
            return 1;
        }
        
        printf("Enter elements for matrix 2:\n");
        for (int i = 0; i < rows2; i++) {
            for (int j = 0; j < cols2; j++) {
                char prompt[32];
                snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
                matrix2[i][j] = read_int(prompt, INT_MIN, INT_MAX);
                if (matrix2[i][j] == -1) return 1;
            }
        }
        
        printf("\nMatrix 2:\n");
        MATRIX_PRINT(matrix2, rows2, cols2);
        
        MATRIX_ADD(result, matrix1, matrix2, rows1, cols1);