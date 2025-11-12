//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: matrix_ops
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
    int matrix1[MAX_ROWS][MAX_COLS];
    int matrix2[MAX_ROWS][MAX_COLS];
    int result[MAX_ROWS][MAX_COLS];
    int rows1, cols1, rows2, cols2;
    int operation;
    
    printf("Matrix Operations Program\n");
    
    rows1 = read_int("Enter rows for matrix 1 (1-10): ", MIN_DIM, MAX_ROWS);
    if (rows1 == -1) {
        printf("Invalid input for rows\n");
        return 1;
    }
    
    cols1 = read_int("Enter columns for matrix 1 (1-10): ", MIN_DIM, MAX_COLS);
    if (cols1 == -1) {
        printf("Invalid input for columns\n");
        return 1;
    }
    
    printf("Enter elements for matrix 1:\n");
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols1; j++) {
            int val = read_int("", INT_MIN, INT_MAX);
            if (val == -1) {
                printf("Invalid matrix element\n");
                return 1;
            }
            matrix1[i][j] = val;
        }
    }
    
    rows2 = read_int("Enter rows for matrix 2 (1-10): ", MIN_DIM, MAX_ROWS);
    if (rows2 == -1) {
        printf("Invalid input for rows\n");
        return 1;
    }
    
    cols2 = read_int("Enter columns for matrix 2 (1-10): ", MIN_DIM, MAX_COLS);
    if (cols2 == -1) {
        printf("Invalid input for columns\n");
        return 1;
    }
    
    printf("Enter elements for matrix 2:\n");
    for (int i = 0; i < rows2; i++) {
        for (int j = 0; j < cols2; j++) {
            int val = read_int("", INT_MIN, INT_MAX);
            if (val == -1) {
                printf("Invalid matrix element\n");
                return 1;
            }
            matrix2[i][j] = val;
        }
    }
    
    printf("\nMatrix 1:\n");
    MATRIX_PRINT(matrix1, rows1, cols1);
    
    printf("\nMatrix 2:\n");
    MATRIX_PRINT(matrix2, rows2, cols2);
    
    printf("\nOperations:\n");
    printf("1. Matrix Addition\n");
    printf("2. Matrix