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

#define MATRIX_ADD(dest, src1, src2, rows, cols) \
    do { \
        for (int i = 0; i < rows; i++) { \
            for (int j = 0; j < cols; j++) { \
                if ((src1[i][j] > 0 && src2[i][j] > INT_MAX - src1[i][j]) || \
                    (src1[i][j] < 0 && src2[i][j] < INT_MIN - src1[i][j])) { \
                    printf("Integer overflow detected at [%d][%d]\n", i, j); \
                    return 1; \
                } \
                dest[i][j] = src1[i][j] + src2[i][j]; \
            } \
        } \
    } while (0)

#define MATRIX_MULTIPLY(dest, mat1, rows1, cols1, mat2, rows2, cols2) \
    do { \
        for (int i = 0; i < rows1; i++) { \
            for (int j = 0; j < cols2; j++) { \
                dest[i][j] = 0; \
                for (int k = 0; k < cols1; k++) { \
                    if (mat1[i][k] != 0 && mat2[k][j] != 0) { \
                        if ((mat1[i][k] > 0 && mat2[k][j] > INT_MAX / mat1[i][k]) || \
                            (mat1[i][k] < 0 && mat2[k][j] < INT_MAX / mat1[i][k])) { \
                            printf("Integer overflow detected at [%d][%d]\n", i, j); \
                            return 1; \
                        } \
                    } \
                    long long product = (long long)mat1[i][k] * mat2[k][j]; \
                    if (product > INT_MAX || product < INT_MIN) { \
                        printf("Integer overflow detected at [%d][%d]\n", i, j); \
                        return 1; \
                    } \
                    if ((dest[i][j] > 0 && product > INT_MAX - dest[i][j]) || \
                        (dest[i][j] < 0 && product < INT_MIN - dest[i][j])) { \
                        printf("Integer overflow detected at [%d][%d]\n", i, j); \
                        return 1; \
                    } \
                    dest[i][j] += (int)product; \
                } \
            } \
        } \
    } while (0)

int read_int(const char* prompt, int min_val, int max_val) {
    int value;
    char buffer[100];
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Input error\n");
            exit(1);
        }
        if (sscanf(buffer, "%d", &value) == 1 && value >= min_val && value <= max_val) {
            return value;
        }
        printf("Invalid input. Enter integer between %d and %d\n", min_val, max_val);
    }
}

void read_matrix(int mat[MAX_ROWS][MAX_COLS], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            char prompt[50];
            snprintf(prompt, sizeof(prompt), "Enter element [%d][%d]: ", i, j);
            mat[i][j] = read_int(prompt, INT_MIN, INT_MAX);
        }
    }
}

int main() {
    int rows1, cols1, rows2, cols2;
    int matrix1[MAX_ROWS][MAX_COLS];
    int matrix2[MAX_ROWS][MAX_COLS];
    int result[MAX_ROWS][MAX_COLS];
    int choice;

    printf("Matrix Operations Program\n");
    printf("Matrix dimensions must be between %d and %d\n", MIN_DIM, MAX_ROWS);

    rows1 = read_int("Enter rows for matrix 1: ", MIN_DIM, MAX_ROWS);
    cols1 = read_int("Enter columns for matrix 1: ", MIN_DIM, MAX_COLS);

    printf("Enter elements for matrix 1:\n");
    read_matrix(matrix1, rows1, cols1);

    rows2 = read_int("Enter rows for matrix 2: ", MIN_DIM, MAX_ROWS);
    cols2 = read_int("Enter