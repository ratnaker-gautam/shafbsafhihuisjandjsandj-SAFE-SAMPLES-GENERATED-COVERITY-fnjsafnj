//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ROWS 10
#define MAX_COLS 10
#define MIN_DIM 1
#define MAX_INPUT_LEN 100

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
                printf("%6d", mat[i][j]); \
            } \
            printf("\n"); \
        } \
    } while (0)

#define MATRIX_ADD(res, a, b, rows, cols) \
    do { \
        for (int i = 0; i < rows; i++) { \
            for (int j = 0; j < cols; j++) { \
                res[i][j] = a[i][j] + b[i][j]; \
            } \
        } \
    } while (0)

#define MATRIX_MULT(res, a, b, a_rows, a_cols, b_cols) \
    do { \
        for (int i = 0; i < a_rows; i++) { \
            for (int j = 0; j < b_cols; j++) { \
                res[i][j] = 0; \
                for (int k = 0; k < a_cols; k++) { \
                    res[i][j] += a[i][k] * b[k][j]; \
                } \
            } \
        } \
    } while (0)

int get_valid_int(const char* prompt, int min_val, int max_val) {
    char input[MAX_INPUT_LEN];
    long val;
    char* endptr;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(input, sizeof(input), stdin) == NULL) {
            return -1;
        }
        
        size_t len = strlen(input);
        if (len > 0 && input[len-1] == '\n') {
            input[len-1] = '\0';
            len--;
        }
        
        if (len == 0) {
            continue;
        }
        
        val = strtol(input, &endptr, 10);
        if (endptr == input || *endptr != '\0') {
            continue;
        }
        
        if (val < min_val || val > max_val) {
            continue;
        }
        
        return (int)val;
    }
}

int main(void) {
    int rows1, cols1, rows2, cols2;
    int matrix1[MAX_ROWS][MAX_COLS];
    int matrix2[MAX_ROWS][MAX_COLS];
    int result[MAX_ROWS][MAX_COLS];
    
    printf("Matrix Operations Program\n");
    printf("Maximum dimensions: %dx%d\n", MAX_ROWS, MAX_COLS);
    
    rows1 = get_valid_int("Enter rows for matrix 1: ", MIN_DIM, MAX_ROWS);
    if (rows1 == -1) return 1;
    cols1 = get_valid_int("Enter columns for matrix 1: ", MIN_DIM, MAX_COLS);
    if (cols1 == -1) return 1;
    
    printf("Enter elements for matrix 1 (%dx%d):\n", rows1, cols1);
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols1; j++) {
            char prompt[50];
            snprintf(prompt, sizeof(prompt), "  Element [%d][%d]: ", i, j);
            matrix1[i][j] = get_valid_int(prompt, INT_MIN, INT_MAX);
            if (matrix1[i][j] == -1) return 1;
        }
    }
    
    rows2 = get_valid_int("Enter rows for matrix 2: ", MIN_DIM, MAX_ROWS);
    if (rows2 == -1) return 1;
    cols2 = get_valid_int("Enter columns for matrix 2: ", MIN_DIM, MAX_COLS);
    if (cols2 == -1) return 1;
    
    printf("Enter elements for matrix 2 (%dx%d):\n", rows2, cols2);
    for (int i = 0; i < rows2; i++) {
        for (int j = 0; j < cols2; j++) {
            char prompt[50];
            snprintf(prompt, sizeof(prompt), "  Element [%d][%d]: ", i, j);
            matrix2[i][j] = get_valid_int(prompt, INT_MIN, INT_MAX);
            if (matrix2[i][j] == -1) return 1;
        }
    }
    
    printf("\nMatrix 1:\n");
    MATRIX_PRINT(matrix1, rows1, cols1);
    
    printf("\nMatrix 2:\n");
    MATRIX_PRINT(matrix2, rows2, cols2);
    
    printf("\nSelect operation:\n");
    printf("1. Matrix Addition\n");
    printf("2. Matrix Multiplication\n");
    
    int choice = get