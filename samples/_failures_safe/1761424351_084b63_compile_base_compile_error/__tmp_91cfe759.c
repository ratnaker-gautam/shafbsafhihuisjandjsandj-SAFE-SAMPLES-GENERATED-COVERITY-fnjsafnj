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
        for (int i = 0; i < rows; i++) \
            for (int j = 0; j < cols; j++) \
                mat[i][j] = 0; \
    } while(0)

#define MATRIX_PRINT(mat, rows, cols) \
    do { \
        for (int i = 0; i < rows; i++) { \
            for (int j = 0; j < cols; j++) \
                printf("%6d", mat[i][j]); \
            printf("\n"); \
        } \
    } while(0)

#define MATRIX_ADD(dest, src1, src2, rows, cols) \
    do { \
        for (int i = 0; i < rows; i++) \
            for (int j = 0; j < cols; j++) \
                dest[i][j] = src1[i][j] + src2[i][j]; \
    } while(0)

#define MATRIX_MULT(dest, mat1, r1, c1, mat2, r2, c2) \
    do { \
        for (int i = 0; i < r1; i++) { \
            for (int j = 0; j < c2; j++) { \
                dest[i][j] = 0; \
                for (int k = 0; k < c1; k++) \
                    dest[i][j] += mat1[i][k] * mat2[k][j]; \
            } \
        } \
    } while(0)

#define SAFE_ATOI(str, num) \
    do { \
        char *endptr; \
        long val = strtol(str, &endptr, 10); \
        if (*endptr != '\0' || val > INT_MAX || val < INT_MIN) { \
            printf("Invalid number: %s\n", str); \
            exit(1); \
        } \
        num = (int)val; \
    } while(0)

int get_valid_int(const char *prompt, int min, int max) {
    char input[MAX_INPUT_LEN];
    int value;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Input error\n");
            exit(1);
        }
        
        char *newline = strchr(input, '\n');
        if (newline) *newline = '\0';
        else {
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            printf("Input too long\n");
            continue;
        }
        
        if (strlen(input) == 0) {
            printf("Empty input\n");
            continue;
        }
        
        SAFE_ATOI(input, value);
        
        if (value >= min && value <= max) {
            return value;
        }
        printf("Value must be between %d and %d\n", min, max);
    }
}

void get_matrix_values(int mat[MAX_ROWS][MAX_COLS], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            char prompt[50];
            snprintf(prompt, sizeof(prompt), "Enter value for [%d][%d]: ", i, j);
            mat[i][j] = get_valid_int(prompt, INT_MIN, INT_MAX);
        }
    }
}

int main(void) {
    int matrix1[MAX_ROWS][MAX_COLS];
    int matrix2[MAX_ROWS][MAX_COLS];
    int result[MAX_ROWS][MAX_COLS];
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    int rows1 = get_valid_int("Enter rows for matrix 1 (1-10): ", MIN_DIM, MAX_ROWS);
    int cols1 = get_valid_int("Enter columns for matrix 1 (1-10): ", MIN_DIM, MAX_COLS);
    
    printf("\nEnter values for matrix 1:\n");
    get_matrix_values(matrix1, rows1, cols1);
    
    int operation;
    printf("\nSelect operation:\n");
    printf("1. Matrix Addition\n");
    printf("2. Matrix Multiplication\n");
    operation = get_valid_int("Enter choice (1-2): ", 1, 2);
    
    if (operation == 1) {
        int rows2 = get_valid_int("Enter rows for matrix 2 (1-10): ", MIN_DIM, MAX_ROWS);
        int cols2 = get_valid_int("Enter columns for matrix 2 (1-10): ", MIN_DIM, MAX_COLS);
        
        if (rows1 != rows2 || cols1 != cols2) {
            printf("Matrix dimensions must match for addition\n");
            return 1;
        }
        
        printf("\nEnter values for matrix 2:\n");
        get_matrix_values(matrix2, rows2, cols2);
        
        MATRIX_ADD(result, matrix1, matrix2, rows1, cols1);
        
        printf("\nMatrix 1:\n");
        MATRIX_PRINT(matrix1, rows1,