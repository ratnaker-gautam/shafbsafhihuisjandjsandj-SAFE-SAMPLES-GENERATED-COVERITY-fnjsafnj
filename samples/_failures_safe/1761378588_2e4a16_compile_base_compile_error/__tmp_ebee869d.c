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
        for (int i = 0; i < (r); i++) \
            for (int j = 0; j < (c); j++) \
                (m)[i][j] = 0; \
    } while (0)

#define MATRIX_PRINT(m, r, c) \
    do { \
        for (int i = 0; i < (r); i++) { \
            for (int j = 0; j < (c); j++) \
                printf("%6d", (m)[i][j]); \
            printf("\n"); \
        } \
    } while (0)

#define MATRIX_ADD(dest, src1, src2, r, c) \
    do { \
        for (int i = 0; i < (r); i++) \
            for (int j = 0; j < (c); j++) \
                (dest)[i][j] = (src1)[i][j] + (src2)[i][j]; \
    } while (0)

#define MATRIX_MULT(dest, src1, r1, c1, src2, r2, c2) \
    do { \
        for (int i = 0; i < (r1); i++) { \
            for (int j = 0; j < (c2); j++) { \
                int sum = 0; \
                for (int k = 0; k < (c1); k++) { \
                    if (src1[i][k] > 0 && src2[k][j] > 0) { \
                        if (src1[i][k] > INT_MAX / src2[k][j]) { \
                            sum = INT_MAX; \
                            break; \
                        } \
                        sum += src1[i][k] * src2[k][j]; \
                    } else if (src1[i][k] < 0 && src2[k][j] < 0) { \
                        if (src1[i][k] < INT_MIN / src2[k][j]) { \
                            sum = INT_MIN; \
                            break; \
                        } \
                        sum += src1[i][k] * src2[k][j]; \
                    } else { \
                        sum += src1[i][k] * src2[k][j]; \
                    } \
                } \
                dest[i][j] = sum; \
            } \
        } \
    } while (0)

int read_int(const char* prompt, int min, int max) {
    int value;
    char buffer[100];
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return min;
        }
        if (sscanf(buffer, "%d", &value) == 1 && value >= min && value <= max) {
            return value;
        }
        printf("Invalid input. Enter integer between %d and %d.\n", min, max);
    }
}

void read_matrix(int matrix[MAX_ROWS][MAX_COLS], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            char prompt[50];
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

    rows1 = read_int("Enter rows for first matrix: ", MIN_DIM, MAX_ROWS);
    cols1 = read_int("Enter columns for first matrix: ", MIN_DIM, MAX_COLS);

    printf("Enter first matrix elements:\n");
    read_matrix(matrix1, rows1, cols1);

    rows2 = read_int("Enter rows for second matrix: ", MIN_DIM, MAX_ROWS);
    cols2 = read_int("Enter columns for second matrix: ", MIN_DIM, MAX_COLS);

    printf("Enter second matrix elements:\n");
    read_matrix(matrix2, rows2, cols2);

    printf("\nFirst matrix:\n");
    MATRIX_PRINT(matrix1, rows1, cols1);

    printf("\nSecond matrix:\n");
    MATRIX_PRINT(matrix2, rows2, cols2);

    printf("\nMatrix Addition:\n");
    if (rows1 == rows2 && cols1 == cols2) {
        MATRIX_INIT(result, rows1, cols1);
        MATRIX_ADD(result, matrix1, matrix2, rows1, cols1);
        MATRIX_PRINT(result, rows1, cols1);
    } else {
        printf("Cannot add: dimension mismatch\n");
    }

    printf("\nMatrix Multiplication:\n");
    if (cols1 == rows2) {
        MATRIX_INIT(result, rows1, cols