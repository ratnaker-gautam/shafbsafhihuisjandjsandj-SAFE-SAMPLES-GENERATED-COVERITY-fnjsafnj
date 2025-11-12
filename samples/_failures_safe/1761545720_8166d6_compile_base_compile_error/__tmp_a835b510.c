//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ROWS 10
#define MAX_COLS 10
#define MIN_DIM 1
#define MAX_INPUT_LEN 32

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

#define MATRIX_ADD(a, b, r, c) \
    do { \
        for (int i = 0; i < r; i++) \
            for (int j = 0; j < c; j++) \
                a[i][j] += b[i][j]; \
    } while (0)

#define MATRIX_MULTIPLY(a, b, c, r1, c1, r2, c2) \
    do { \
        for (int i = 0; i < r1; i++) { \
            for (int j = 0; j < c2; j++) { \
                c[i][j] = 0; \
                for (int k = 0; k < c1; k++) { \
                    if (a[i][k] > 0 && b[k][j] > 0) { \
                        if (a[i][k] > INT_MAX / b[k][j]) { \
                            printf("Integer overflow detected\n"); \
                            return 1; \
                        } \
                    } else if (a[i][k] < 0 && b[k][j] < 0) { \
                        if (a[i][k] < INT_MIN / b[k][j]) { \
                            printf("Integer overflow detected\n"); \
                            return 1; \
                        } \
                    } \
                    c[i][j] += a[i][k] * b[k][j]; \
                } \
            } \
        } \
    } while (0)

int safe_str_to_int(const char *str, int *result) {
    char *endptr;
    long val = strtol(str, &endptr, 10);
    if (endptr == str || *endptr != '\0') return 0;
    if (val < INT_MIN || val > INT_MAX) return 0;
    *result = (int)val;
    return 1;
}

int read_matrix(int matrix[MAX_ROWS][MAX_COLS], int rows, int cols) {
    char input[MAX_INPUT_LEN + 1];
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("Enter element [%d][%d]: ", i, j);
            if (fgets(input, sizeof(input), stdin) == NULL) return 0;
            size_t len = strlen(input);
            if (len > 0 && input[len-1] == '\n') input[len-1] = '\0';
            int value;
            if (!safe_str_to_int(input, &value)) {
                printf("Invalid input\n");
                return 0;
            }
            matrix[i][j] = value;
        }
    }
    return 1;
}

int main(void) {
    int matrix1[MAX_ROWS][MAX_COLS];
    int matrix2[MAX_ROWS][MAX_COLS];
    int result[MAX_ROWS][MAX_COLS];
    char input[MAX_INPUT_LEN + 1];
    int rows1, cols1, rows2, cols2;
    int operation;

    printf("Enter number of rows for matrix 1 (1-10): ");
    if (fgets(input, sizeof(input), stdin) == NULL) return 1;
    if (!safe_str_to_int(input, &rows1) || rows1 < MIN_DIM || rows1 > MAX_ROWS) {
        printf("Invalid rows\n");
        return 1;
    }

    printf("Enter number of columns for matrix 1 (1-10): ");
    if (fgets(input, sizeof(input), stdin) == NULL) return 1;
    if (!safe_str_to_int(input, &cols1) || cols1 < MIN_DIM || cols1 > MAX_COLS) {
        printf("Invalid columns\n");
        return 1;
    }

    printf("Enter number of rows for matrix 2 (1-10): ");
    if (fgets(input, sizeof(input), stdin) == NULL) return 1;
    if (!safe_str_to_int(input, &rows2) || rows2 < MIN_DIM || rows2 > MAX_ROWS) {
        printf("Invalid rows\n");
        return 1;
    }

    printf("Enter number of columns for matrix 2 (1-10): ");
    if (fgets(input, sizeof(input), stdin) == NULL) return 1;
    if (!safe_str_to_int(input, &cols2) || cols2 < MIN_DIM || cols2 > MAX_COLS) {
        printf("Invalid columns\n");
        return 1;
    }

    printf("Matrix 1:\n");
    if (!read_matrix(matrix1, rows1