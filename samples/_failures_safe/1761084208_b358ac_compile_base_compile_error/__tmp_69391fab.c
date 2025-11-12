//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: matrix_ops
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
                mat[i][j] = 0; \
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

#define MATRIX_ADD(dest, src1, src2, rows, cols) \
    do { \
        for (int i = 0; i < rows; i++) { \
            for (int j = 0; j < cols; j++) { \
                dest[i][j] = src1[i][j] + src2[i][j]; \
            } \
        } \
    } while (0)

#define MATRIX_MULT(dest, src1, src2, r1, c1, c2) \
    do { \
        for (int i = 0; i < r1; i++) { \
            for (int j = 0; j < c2; j++) { \
                dest[i][j] = 0; \
                for (int k = 0; k < c1; k++) { \
                    dest[i][j] += src1[i][k] * src2[k][j]; \
                } \
            } \
        } \
    } while (0)

int read_int(const char* prompt, int min_val, int max_val) {
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
        
        if (temp < min_val || temp > max_val) {
            printf("Value must be between %d and %d.\n", min_val, max_val);
            continue;
        }
        
        value = (int)temp;
        break;
    }
    
    return value;
}

void read_matrix(int mat[MAX_ROWS][MAX_COLS], int rows, int cols) {
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            char prompt[50];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            mat[i][j] = read_int(prompt, INT_MIN, INT_MAX);
        }
    }
}

int main(void) {
    int matrix1[MAX_ROWS][MAX_COLS];
    int matrix2[MAX_ROWS][MAX_COLS];
    int result[MAX_ROWS][MAX_COLS];
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    int rows1 = read_int("Enter rows for first matrix (1-10): ", MIN_DIM, MAX_ROWS);
    int cols1 = read_int("Enter columns for first matrix (1-10): ", MIN_DIM, MAX_COLS);
    
    printf("\nFirst Matrix:\n");
    read_matrix(matrix1, rows1, cols1);
    
    int operation = read_int("\nSelect operation:\n1. Matrix Addition\n2. Matrix Multiplication\nChoice: ", 1, 2);
    
    if (operation == 1) {
        int rows2 = read_int("Enter rows for second matrix (1-10): ", MIN_DIM, MAX_ROWS);
        int cols2 = read_int("Enter columns for second matrix (1-10): ", MIN_DIM, MAX_COLS);
        
        if (rows1 != rows2 || cols1 != cols2) {
            printf("Error: Matrices must have same dimensions for addition.\n");
            return 1;
        }
        
        printf("\nSecond Matrix:\n");
        read_matrix(matrix2, rows2, cols2);
        
        MATRIX_ADD(result, matrix1, matrix2, rows1, cols1);
        
        printf("\nResult of Matrix Addition:\n");
        MATRIX_PRINT(result, rows1, cols1);
        
    } else if (operation == 2) {
        int rows2 = read_int("Enter rows for second matrix (1-10): ", MIN_DIM, MAX_ROWS);
        int cols2 = read_int("Enter columns for second matrix (1-10): ", MIN_DIM, MAX_COLS);
        
        if (cols1 != rows2) {
            printf("Error: First matrix columns must equal second matrix rows for multiplication.\n");
            return 1;
        }
        
        printf("\nSecond Matrix:\n");
        read_matrix(matrix2, rows2, cols2);
        
        MATRIX_INIT(result, rows1, cols2);