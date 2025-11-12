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
                    if ((mat1[i][k] > 0 && dest[i][j] > INT_MAX - product) || \
                        (mat1[i][k] < 0 && dest[i][j] < INT_MIN - product)) { \
                        printf("Integer overflow detected at [%d][%d]\n", i, j); \
                        return 1; \
                    } \
                    dest[i][j] += (int)product; \
                } \
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
    if (endptr == buffer || *endptr != '\n' || value < min || value > max) {
        return -1;
    }
    
    return (int)value;
}

int main(void) {
    int matrix1[MAX_ROWS][MAX_COLS];
    int matrix2[MAX_ROWS][MAX_COLS];
    int result[MAX_ROWS][MAX_COLS];
    int rows1, cols1, rows2, cols2;
    int operation;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    rows1 = read_int("Enter rows for matrix 1 (1-10): ", MIN_DIM, MAX_ROWS);
    if (rows1 == -1) {
        printf("Invalid rows input for matrix 1\n");
        return 1;
    }
    
    cols1 = read_int("Enter columns for matrix 1 (1-10): ", MIN_DIM, MAX_COLS);
    if (cols1 == -1) {
        printf("Invalid columns input for matrix 1\n");
        return 1;
    }
    
    printf("Enter elements for matrix 1:\n");
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols1; j++) {
            char prompt[32];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            matrix1[i][j] = read_int(prompt, INT_MIN, INT_MAX);
            if (matrix1[i][j] == -1) {
                printf("Invalid element input\n");
                return 1;
            }
        }
    }
    
    rows2 = read_int("Enter rows for matrix 2 (1-10): ", MIN_DIM, MAX_ROWS);
    if (rows2 == -1) {
        printf("Invalid rows input for matrix 2\n");
        return 1;
    }
    
    cols2 = read_int("Enter columns for matrix 2 (1-10): ", MIN_DIM, MAX_COLS);
    if (cols2 == -1) {
        printf("Invalid columns input for matrix 2