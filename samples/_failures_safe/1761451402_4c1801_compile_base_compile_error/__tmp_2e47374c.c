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

#define MATRIX_MULT(res, a, b, ra, ca, cb) \
    do { \
        for (int i = 0; i < ra; i++) { \
            for (int j = 0; j < cb; j++) { \
                res[i][j] = 0; \
                for (int k = 0; k < ca; k++) { \
                    res[i][j] += a[i][k] * b[k][j]; \
                } \
            } \
        } \
    } while (0)

int safe_strtol(const char *str, int *result) {
    char *endptr;
    long val = strtol(str, &endptr, 10);
    if (endptr == str || *endptr != '\0') return 0;
    if (val < INT_MIN || val > INT_MAX) return 0;
    *result = (int)val;
    return 1;
}

int get_valid_int(const char *prompt, int min, int max) {
    char buffer[32];
    int value;
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) return -1;
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len-1] == '\n') buffer[len-1] = '\0';
        if (safe_strtol(buffer, &value) && value >= min && value <= max) {
            return value;
        }
        printf("Invalid input. Enter integer between %d and %d.\n", min, max);
    }
}

void matrix_input(int mat[MAX_ROWS][MAX_COLS], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            char prompt[64];
            snprintf(prompt, sizeof(prompt), "Enter element [%d][%d]: ", i, j);
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
    if (choice == 3) return 0;

    if (choice == 1) {
        rows1 = get_valid_int("Enter rows for matrices (1-10): ", MIN_DIM, MAX_ROWS);
        cols1 = get_valid_int("Enter columns for matrices (1-10): ", MIN_DIM, MAX_COLS);
        rows2 = rows1;
        cols2 = cols1;
    } else {
        rows1 = get_valid_int("Enter rows for first matrix (1-10): ", MIN_DIM, MAX_ROWS);
        cols1 = get_valid_int("Enter columns for first matrix (1-10): ", MIN_DIM, MAX_COLS);
        rows2 = get_valid_int("Enter rows for second matrix (1-10): ", MIN_DIM, MAX_ROWS);
        cols2 = get_valid_int("Enter columns for second matrix (1-10): ", MIN_DIM, MAX_COLS);
    }

    if (choice == 2 && cols1 != rows2) {
        printf("Matrix multiplication not possible: columns of first must equal rows of second.\n");
        return 1;
    }

    printf("Enter first matrix:\n");
    matrix_input(matrix1, rows1, cols1);
    printf("Enter second matrix:\n");
    matrix_input(matrix2, rows2, cols2);

    MATRIX_INIT(result, MAX_ROWS, MAX_COLS);

    if (choice == 1) {
        MATRIX_ADD(result, matrix1, matrix2, rows1, cols1);
        printf("Matrix Addition Result:\n");
        MATRI