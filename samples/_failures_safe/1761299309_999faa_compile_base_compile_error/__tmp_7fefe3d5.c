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
                    long long product = (long long)mat1[i][k] * mat2[k][j]; \
                    if (product > INT_MAX || product < INT_MIN) { \
                        printf("Integer overflow detected at [%d][%d]\n", i, j); \
                        return 1; \
                    } \
                    long long sum = (long long)dest[i][j] + product; \
                    if (sum > INT_MAX || sum < INT_MIN) { \
                        printf("Integer overflow detected at [%d][%d]\n", i, j); \
                        return 1; \
                    } \
                    dest[i][j] = (int)sum; \
                } \
            } \
        } \
    } while (0)

int get_valid_int(const char* prompt, int min_val, int max_val) {
    char buffer[100];
    int value;
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        if (sscanf(buffer, "%d", &value) != 1) {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        if (value < min_val || value > max_val) {
            printf("Value must be between %d and %d.\n", min_val, max_val);
            continue;
        }
        return value;
    }
}

void get_matrix_values(int matrix[MAX_ROWS][MAX_COLS], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            char prompt[50];
            snprintf(prompt, sizeof(prompt), "Enter value for [%d][%d]: ", i, j);
            matrix[i][j] = get_valid_int(prompt, INT_MIN, INT_MAX);
        }
    }
}

int main(void) {
    int rows1, cols1, rows2, cols2;
    int matrix1[MAX_ROWS][MAX_COLS];
    int matrix2[MAX_ROWS][MAX_COLS];
    int result[MAX_ROWS][MAX_COLS];
    int choice;

    printf("Matrix Operations Program\n");
    printf("=========================\n");

    rows1 = get_valid_int("Enter rows for matrix 1: ", MIN_DIM, MAX_ROWS);
    if (rows1 == -1) return 1;
    cols1 = get_valid_int("Enter columns for matrix 1: ", MIN_DIM, MAX_COLS);
    if (cols1 == -1) return 1;

    printf("Enter values for matrix 1:\n");
    get_matrix_values(matrix1, rows1, cols1);

    rows2 = get_valid_int("Enter rows for matrix 2: ", MIN_DIM, MAX_ROWS);
    if (rows2 == -1) return 1;
    cols2 = get_valid_int("Enter columns for matrix 2: ", MIN_DIM, MAX_COLS);
    if (cols2 == -1) return 1;

    printf("Enter values for matrix 2:\n");
    get_matrix_values(matrix2, rows2, cols2);

    printf("\nMatrix 1:\n");
    MATRIX_PRINT(matrix1, rows1, cols1);

    printf("\nMatrix