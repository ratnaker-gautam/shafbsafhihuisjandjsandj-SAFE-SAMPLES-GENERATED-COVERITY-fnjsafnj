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
        for (int i = 0; i < r; i++) { \
            for (int j = 0; j < c; j++) { \
                m[i][j] = 0; \
            } \
        } \
    } while (0)

#define MATRIX_PRINT(m, r, c) \
    do { \
        for (int i = 0; i < r; i++) { \
            for (int j = 0; j < c; j++) { \
                printf("%d ", m[i][j]); \
            } \
            printf("\n"); \
        } \
    } while (0)

#define MATRIX_ADD(a, b, r, c) \
    do { \
        for (int i = 0; i < r; i++) { \
            for (int j = 0; j < c; j++) { \
                if ((a[i][j] > 0 && b[i][j] > INT_MAX - a[i][j]) || \
                    (a[i][j] < 0 && b[i][j] < INT_MIN - a[i][j])) { \
                    printf("Integer overflow detected\n"); \
                    return 1; \
                } \
                a[i][j] += b[i][j]; \
            } \
        } \
    } while (0)

int read_int(const char* prompt, int min, int max) {
    char buffer[32];
    long value;
    char* endptr;

    printf("%s", prompt);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return -1;
    }

    value = strtol(buffer, &endptr, 10);
    if (endptr == buffer || *endptr != '\n') {
        return -1;
    }

    if (value < min || value > max) {
        return -1;
    }

    return (int)value;
}

int main(void) {
    int rows1, cols1, rows2, cols2;
    int matrix1[MAX_ROWS][MAX_COLS];
    int matrix2[MAX_ROWS][MAX_COLS];

    rows1 = read_int("Enter rows for matrix 1 (1-10): ", MIN_DIM, MAX_ROWS);
    if (rows1 == -1) {
        printf("Invalid input for rows1\n");
        return 1;
    }

    cols1 = read_int("Enter columns for matrix 1 (1-10): ", MIN_DIM, MAX_COLS);
    if (cols1 == -1) {
        printf("Invalid input for cols1\n");
        return 1;
    }

    printf("Enter elements for matrix 1:\n");
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols1; j++) {
            char prompt[32];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            int val = read_int(prompt, INT_MIN, INT_MAX);
            if (val == -1) {
                printf("Invalid input for element\n");
                return 1;
            }
            matrix1[i][j] = val;
        }
    }

    rows2 = read_int("Enter rows for matrix 2 (1-10): ", MIN_DIM, MAX_ROWS);
    if (rows2 == -1) {
        printf("Invalid input for rows2\n");
        return 1;
    }

    cols2 = read_int("Enter columns for matrix 2 (1-10): ", MIN_DIM, MAX_COLS);
    if (cols2 == -1) {
        printf("Invalid input for cols2\n");
        return 1;
    }

    if (rows1 != rows2 || cols1 != cols2) {
        printf("Matrix dimensions must match for addition\n");
        return 1;
    }

    printf("Enter elements for matrix 2:\n");
    for (int i = 0; i < rows2; i++) {
        for (int j = 0; j < cols2; j++) {
            char prompt[32];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            int val = read_int(prompt, INT_MIN, INT_MAX);
            if (val == -1) {
                printf("Invalid input for element\n");
                return 1;
            }
            matrix2[i][j] = val;
        }
    }

    printf("\nMatrix 1:\n");
    MATRIX_PRINT(matrix1, rows1, cols1);

    printf("\nMatrix 2:\n");
    MATRIX_PRINT(matrix2, rows2, cols2);

    printf("\nAdding matrices...\n");
    MATRIX_ADD(matrix1, matrix2, rows1, cols1);

    printf("\nResult:\n");
    MATRIX_PRINT(matrix1, rows1, cols1);

    return 0;
}