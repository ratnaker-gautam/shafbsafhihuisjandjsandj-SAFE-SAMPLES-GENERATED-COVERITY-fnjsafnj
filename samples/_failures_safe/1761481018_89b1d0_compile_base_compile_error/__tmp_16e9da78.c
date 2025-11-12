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

int safe_str_to_int(const char* str, int* result) {
    char* endptr;
    long val = strtol(str, &endptr, 10);
    if (endptr == str || *endptr != '\0') {
        return 0;
    }
    if (val > INT_MAX || val < INT_MIN) {
        return 0;
    }
    *result = (int)val;
    return 1;
}

int get_valid_int(const char* prompt, int min_val, int max_val) {
    char input[32];
    int value;
    while (1) {
        printf("%s", prompt);
        if (fgets(input, sizeof(input), stdin) == NULL) {
            return min_val;
        }
        size_t len = strlen(input);
        if (len > 0 && input[len-1] == '\n') {
            input[len-1] = '\0';
        }
        if (safe_str_to_int(input, &value) && value >= min_val && value <= max_val) {
            return value;
        }
        printf("Invalid input. Enter a number between %d and %d.\n", min_val, max_val);
    }
}

void matrix_input(int mat[MAX_ROWS][MAX_COLS], int rows, int cols) {
    printf("Enter matrix elements (%d x %d):\n", rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            char prompt[32];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            mat[i][j] = get_valid_int(prompt, -1000, 1000);
        }
    }
}

int main(void) {
    int choice;
    int rows1, cols1, rows2, cols2;
    int matrix1[MAX_ROWS][MAX_COLS];
    int matrix2[MAX_ROWS][MAX_COLS];
    int result[MAX_ROWS][MAX_COLS];
    
    printf("Matrix Operations Program\n");
    printf("1. Matrix Addition\n");
    printf("2. Matrix Multiplication\n");
    printf("3. Exit\n");
    
    choice = get_valid_int("Select operation (1-3): ", 1, 3);
    
    if (choice == 3) {
        return 0;
    }
    
    printf("\nMatrix 1 dimensions:\n");
    rows1 = get_valid_int("Rows: ", MIN_DIM, MAX_ROWS);
    cols1 = get_valid_int("Columns: ", MIN_DIM, MAX_COLS);
    matrix_input(matrix1, rows1, cols1);
    
    printf("\nMatrix 2 dimensions:\n");
    rows2 = get_valid_int("Rows: ", MIN_DIM, MAX_ROWS);
    cols2 = get_valid_int("Columns: ", MIN_DIM, MAX_COLS);
    matrix_input(matrix2, rows2, cols2);
    
    printf("\nMatrix 1:\n");
    MATRIX_PRINT(matrix1, rows1, cols1);
    printf("\nMatrix 2:\n");
    MATRIX_PRINT(matrix2, rows2, cols2);
    
    MATRIX_INIT(result, MAX_ROWS, MAX_COLS);
    
    switch (choice) {
        case 1:
            if (rows1 != rows2 || cols1 != cols2) {
                printf("Error: Matrices must have same dimensions for addition.\n");
                return 1;
            }
            MATRIX_ADD(result, matrix1, matrix2, rows1, cols1);
            printf("\nMatrix Addition Result