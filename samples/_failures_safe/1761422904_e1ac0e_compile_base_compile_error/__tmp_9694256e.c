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

#define MATRIX_MULT(a, b, c, m, n, p) \
    do { \
        int temp[MAX_ROWS][MAX_COLS]; \
        for (int i = 0; i < m; i++) { \
            for (int j = 0; j < p; j++) { \
                temp[i][j] = 0; \
                for (int k = 0; k < n; k++) { \
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
                    temp[i][j] += a[i][k] * b[k][j]; \
                } \
            } \
        } \
        for (int i = 0; i < m; i++) \
            for (int j = 0; j < p; j++) \
                c[i][j] = temp[i][j]; \
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
            printf("Element [%d][%d]: ", i, j);
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
    int rows1, cols1, rows2, cols2;
    char input[MAX_INPUT_LEN + 1];
    
    printf("Enter first matrix dimensions (rows cols): ");
    if (fgets(input, sizeof(input), stdin) == NULL) return 1;
    if (sscanf(input, "%d %d", &rows1, &cols1) != 2) {
        printf("Invalid dimensions\n");
        return 1;
    }
    if (rows1 < MIN_DIM || rows1 > MAX_ROWS || cols1 < MIN_DIM || cols1 > MAX_COLS) {
        printf("Dimensions out of range\n");
        return 1;
    }
    
    printf("Enter second matrix dimensions (rows cols): ");
    if (fgets(input, sizeof(input), stdin) == NULL) return 1;
    if (sscanf(input, "%d %d", &rows2, &cols2) != 2) {
        printf("Invalid dimensions\n");
        return 1;
    }
    if (rows2 < MIN_DIM || rows2 > MAX_ROWS || cols2 < MIN_DIM || cols2 > MAX_COLS) {
        printf("Dimensions out of range\n");
        return 1;
    }
    
    int matrix1[MAX_ROWS][MAX_COLS];
    int matrix2[MAX_ROWS][MAX_COLS];
    
    printf("Enter first matrix elements:\n");
    if (!read_matrix(matrix1, rows1, cols1)) return 1;
    
    printf("Enter second matrix elements:\n");
    if (!read_matrix(matrix2, rows2, cols2)) return 1;
    
    printf("\nMatrix