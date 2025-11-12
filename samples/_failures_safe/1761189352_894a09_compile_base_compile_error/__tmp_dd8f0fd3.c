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
                    exit(1); \
                } \
                a[i][j] += b[i][j]; \
            } \
        } \
    } while (0)

#define MATRIX_MULTIPLY(a, b, c, ra, ca, cb) \
    do { \
        int temp[MAX_ROWS][MAX_COLS]; \
        for (int i = 0; i < ra; i++) { \
            for (int j = 0; j < cb; j++) { \
                temp[i][j] = 0; \
                for (int k = 0; k < ca; k++) { \
                    if ((a[i][k] > 0 && b[k][j] > 0 && a[i][k] > INT_MAX / b[k][j]) || \
                        (a[i][k] < 0 && b[k][j] < 0 && a[i][k] < INT_MAX / b[k][j]) || \
                        (a[i][k] > 0 && b[k][j] < 0 && b[k][j] < INT_MIN / a[i][k]) || \
                        (a[i][k] < 0 && b[k][j] > 0 && a[i][k] < INT_MIN / b[k][j])) { \
                        printf("Integer overflow detected\n"); \
                        exit(1); \
                    } \
                    int product = a[i][k] * b[k][j]; \
                    if ((product > 0 && temp[i][j] > INT_MAX - product) || \
                        (product < 0 && temp[i][j] < INT_MIN - product)) { \
                        printf("Integer overflow detected\n"); \
                        exit(1); \
                    } \
                    temp[i][j] += product; \
                } \
            } \
        } \
        for (int i = 0; i < ra; i++) { \
            for (int j = 0; j < cb; j++) { \
                c[i][j] = temp[i][j]; \
            } \
        } \
    } while (0)

int read_int(const char* prompt, int min, int max) {
    char buffer[32];
    long value;
    char* endptr;
    
    printf("%s", prompt);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        printf("Input error\n");
        exit(1);
    }
    
    value = strtol(buffer, &endptr, 10);
    if (endptr == buffer || *endptr != '\n') {
        printf("Invalid input format\n");
        exit(1);
    }
    
    if (value < min || value > max) {
        printf("Value out of range [%d, %d]\n", min, max);
        exit(1);
    }
    
    return (int)value;
}

void read_matrix(int matrix[MAX_ROWS][MAX_COLS], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            char prompt[32];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            matrix[i][j] = read_int(prompt, INT_MIN, INT_MAX);
        }
    }
}

int main(void) {
    int matrix1[MAX_ROWS][MAX_COLS];
    int matrix2[MAX_ROWS][MAX_COLS];
    int result[MAX_ROWS][MAX_COLS];
    
    printf("Matrix Operations Program\n");
    
    int rows1 = read_int("Enter rows for matrix 1: ", MIN_DIM, MAX_ROWS);
    int cols1 = read_int("Enter columns for matrix 1: ", MIN_DIM, MAX_COLS);
    
    printf("Enter elements for matrix 1:\n");
    read_matrix(matrix1, rows1, cols1);
    
    int rows2 = read_int("Enter rows for matrix 2: ", MIN_DIM, MAX_ROWS);
    int cols2 = read_int("Enter columns for matrix 2: ",