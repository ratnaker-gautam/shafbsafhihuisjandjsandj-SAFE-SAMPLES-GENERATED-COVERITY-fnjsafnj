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

#define MATRIX_MULT(res, a, b, rows, cols, inner) \
    do { \
        for (int i = 0; i < rows; i++) { \
            for (int j = 0; j < cols; j++) { \
                res[i][j] = 0; \
                for (int k = 0; k < inner; k++) { \
                    res[i][j] += a[i][k] * b[k][j]; \
                } \
            } \
        } \
    } while (0)

int safe_str_to_int(const char *str, int *result) {
    char *endptr;
    long val = strtol(str, &endptr, 10);
    if (endptr == str || *endptr != '\0') {
        return 0;
    }
    if (val < INT_MIN || val > INT_MAX) {
        return 0;
    }
    *result = (int)val;
    return 1;
}

int read_matrix(int mat[MAX_ROWS][MAX_COLS], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            char buffer[32];
            if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
                return 0;
            }
            int value;
            if (!safe_str_to_int(buffer, &value)) {
                return 0;
            }
            mat[i][j] = value;
        }
    }
    return 1;
}

int main(void) {
    int rows1, cols1, rows2, cols2;
    char buffer[32];

    printf("Enter rows for matrix 1 (1-10): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 1;
    }
    if (!safe_str_to_int(buffer, &rows1) || rows1 < MIN_DIM || rows1 > MAX_ROWS) {
        printf("Invalid rows for matrix 1\n");
        return 1;
    }

    printf("Enter columns for matrix 1 (1-10): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 1;
    }
    if (!safe_str_to_int(buffer, &cols1) || cols1 < MIN_DIM || cols1 > MAX_COLS) {
        printf("Invalid columns for matrix 1\n");
        return 1;
    }

    printf("Enter rows for matrix 2 (1-10): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 1;
    }
    if (!safe_str_to_int(buffer, &rows2) || rows2 < MIN_DIM || rows2 > MAX_ROWS) {
        printf("Invalid rows for matrix 2\n");
        return 1;
    }

    printf("Enter columns for matrix 2 (1-10): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 1;
    }
    if (!safe_str_to_int(buffer, &cols2) || cols2 < MIN_DIM || cols2 > MAX_COLS) {
        printf("Invalid columns for matrix 2\n");
        return 1;
    }

    int matrix1[MAX_ROWS][MAX_COLS];
    int matrix2[MAX_ROWS][MAX_COLS];
    int result[MAX_ROWS][MAX_COLS];

    MATRIX_INIT(matrix1, rows1, cols1);
    MATRIX_INIT(matrix2, rows2, cols2);
    MATRIX_INIT(result, MAX_ROWS, MAX_COLS);

    printf("Enter %d elements for matrix 1:\n", rows1 * cols1);
    if (!read_matrix(matrix1, rows1, cols1)) {
        printf("Invalid input for matrix 1\n");
        return 1;
    }

    printf("Enter %d elements for matrix 2:\n", rows2 * cols2);
    if (!read_matrix(matrix2, rows2, cols2)) {
        printf("Invalid input for matrix 2\n");
        return 1;
    }

    printf("\nMatrix 1:\n");
    MATRIX_PRINT(matrix1, rows1,