//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: matrix_ops
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
        for (int i = 0; i < r; i++) \
            for (int j = 0; j < c; j++) \
                m[i][j] = 0; \
    } while (0)

#define MATRIX_PRINT(m, r, c) \
    do { \
        for (int i = 0; i < r; i++) { \
            for (int j = 0; j < c; j++) \
                printf("%6d", m[i][j]); \
            printf("\n"); \
        } \
    } while (0)

#define MATRIX_ADD(dest, src1, src2, r, c) \
    do { \
        for (int i = 0; i < r; i++) \
            for (int j = 0; j < c; j++) \
                dest[i][j] = src1[i][j] + src2[i][j]; \
    } while (0)

#define MATRIX_MULT(dest, a, b, ra, ca, cb) \
    do { \
        for (int i = 0; i < ra; i++) { \
            for (int j = 0; j < cb; j++) { \
                int sum = 0; \
                for (int k = 0; k < ca; k++) \
                    sum += a[i][k] * b[k][j]; \
                dest[i][j] = sum; \
            } \
        } \
    } while (0)

int read_int(const char* prompt, int min, int max) {
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
        if (value < min || value > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        break;
    }
    return value;
}

void read_matrix(int matrix[MAX_ROWS][MAX_COLS], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            char prompt[64];
            snprintf(prompt, sizeof(prompt), "Enter element [%d][%d]: ", i, j);
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
    printf("Matrix dimensions must be between %d and %d\n", MIN_DIM, MAX_ROWS);

    rows1 = read_int("Enter rows for matrix 1: ", MIN_DIM, MAX_ROWS);
    cols1 = read_int("Enter columns for matrix 1: ", MIN_DIM, MAX_COLS);

    printf("Enter elements for matrix 1:\n");
    read_matrix(matrix1, rows1, cols1);

    rows2 = read_int("Enter rows for matrix 2: ", MIN_DIM, MAX_ROWS);
    cols2 = read_int("Enter columns for matrix 2: ", MIN_DIM, MAX_COLS);

    printf("Enter elements for matrix 2:\n");
    read_matrix(matrix2, rows2, cols2);

    printf("\nMatrix 1:\n");
    MATRIX_PRINT(matrix1, rows1, cols1);

    printf("\nMatrix 2:\n");
    MATRIX_PRINT(matrix2, rows2, cols2);

    printf("\nSelect operation:\n");
    printf("1. Matrix Addition\n");
    printf("2. Matrix Multiplication\n");
    
    int choice = read_int("Enter choice (1-2): ", 1, 2);

    MATRIX_INIT(result, MAX_ROWS, MAX_COLS);

    if (choice == 1) {
        if (rows1 != rows2 || cols1 != cols2) {
            printf("Error: Matrices must have same dimensions for addition.\n");
            return 1;
        }
        MATRIX_ADD(result, matrix1, matrix2, rows1, cols1);
        printf("\nMatrix Addition Result:\n");
        MATRIX_PRINT(result, rows1, cols1);
    } else {
        if (cols1 != rows2) {
            printf("Error: Matrix 1 columns must equal matrix 2 rows for multiplication.\n");
            return 1;
        }
        MATRIX_MULT(result, matrix1, matrix2, rows1, cols1, cols2);
        printf("\nMatrix Multiplication Result:\n");
        MATRIX_PRINT(result, rows1, cols2);
    }

    return 0;
}