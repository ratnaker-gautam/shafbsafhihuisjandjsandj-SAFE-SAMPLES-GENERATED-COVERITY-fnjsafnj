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

#define MATRIX_MULTIPLY(a, b, result, a_rows, a_cols, b_cols) \
    do { \
        for (int i = 0; i < a_rows; i++) { \
            for (int j = 0; j < b_cols; j++) { \
                result[i][j] = 0; \
                for (int k = 0; k < a_cols; k++) { \
                    result[i][j] += a[i][k] * b[k][j]; \
                } \
            } \
        } \
    } while (0)

int get_valid_int(const char* prompt, int min_val, int max_val) {
    char input[32];
    long value;
    char* endptr;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(input, sizeof(input), stdin) == NULL) {
            return min_val;
        }
        
        value = strtol(input, &endptr, 10);
        if (endptr == input || *endptr != '\n') {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (value < min_val || value > max_val) {
            printf("Value must be between %d and %d.\n", min_val, max_val);
            continue;
        }
        
        return (int)value;
    }
}

int main(void) {
    int rows1, cols1, rows2, cols2;
    int matrix1[MAX_ROWS][MAX_COLS];
    int matrix2[MAX_ROWS][MAX_COLS];
    int result[MAX_ROWS][MAX_COLS];
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    rows1 = get_valid_int("Enter rows for matrix 1 (1-10): ", MIN_DIM, MAX_ROWS);
    cols1 = get_valid_int("Enter columns for matrix 1 (1-10): ", MIN_DIM, MAX_COLS);
    
    printf("\nMatrix 1 (identity):\n");
    MATRIX_INIT(matrix1, rows1, cols1);
    MATRIX_PRINT(matrix1, rows1, cols1);
    
    rows2 = get_valid_int("\nEnter rows for matrix 2 (1-10): ", MIN_DIM, MAX_ROWS);
    cols2 = get_valid_int("Enter columns for matrix 2 (1-10): ", MIN_DIM, MAX_COLS);
    
    printf("\nMatrix 2 (identity):\n");
    MATRIX_INIT(matrix2, rows2, cols2);
    MATRIX_PRINT(matrix2, rows2, cols2);
    
    printf("\nMatrix Addition:\n");
    if (rows1 == rows2 && cols1 == cols2) {
        MATRIX_ADD(matrix1, matrix2, result, rows1, cols1);
        MATRIX_PRINT(result, rows1, cols1);
    } else {
        printf("Cannot add: matrices must have same dimensions.\n");
    }
    
    printf("\nMatrix Multiplication:\n");
    if (cols1 == rows2) {
        MATRIX_MULTIPLY(matrix1, matrix2, result, rows1, cols1, cols2);
        MATRIX_PRINT(result, rows1, cols2);
    } else {
        printf("Cannot multiply: columns of first must equal rows of second.\n");
    }
    
    printf("\nScalar Operations:\n");
    int scalar = get_valid_int("Enter scalar value (-100 to 100): ", -100, 100);
    
    printf("\nMatrix 1 multiplied by scalar:\n");
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols1; j++) {
            printf("%3d ", matrix1[i][j] * scalar);
        }
        printf("\n");
    }
    
    printf("\nMatrix 2 multiplied by scalar:\n");
    for (int i = 0; i < rows2; i++) {
        for (int j = 0; j < cols2; j++) {
            printf("%3d ", matrix2[i][