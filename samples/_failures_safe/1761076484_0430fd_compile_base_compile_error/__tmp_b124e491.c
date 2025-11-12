//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ROWS 10
#define MAX_COLS 10
#define MATRIX_SIZE (MAX_ROWS * MAX_COLS)

typedef struct {
    int data[MAX_ROWS][MAX_COLS];
    int rows;
    int cols;
} Matrix;

#define MATRIX_INIT(m, r, c) do { \
    (m).rows = (r); \
    (m).cols = (c); \
    memset((m).data, 0, sizeof((m).data)); \
} while(0)

#define MATRIX_GET(m, i, j) ((m).data[(i)][(j)])
#define MATRIX_SET(m, i, j, val) ((m).data[(i)][(j)] = (val))

#define MATRIX_ADD(a, b, result) do { \
    if ((a).rows != (b).rows || (a).cols != (b).cols) { \
        printf("Matrix dimensions mismatch for addition\n"); \
        return 1; \
    } \
    (result).rows = (a).rows; \
    (result).cols = (a).cols; \
    for (int i = 0; i < (a).rows; i++) { \
        for (int j = 0; j < (a).cols; j++) { \
            int sum = (a).data[i][j] + (b).data[i][j]; \
            if (((a).data[i][j] > 0 && (b).data[i][j] > INT_MAX - (a).data[i][j]) || \
                ((a).data[i][j] < 0 && (b).data[i][j] < INT_MIN - (a).data[i][j])) { \
                printf("Integer overflow in addition\n"); \
                return 1; \
            } \
            (result).data[i][j] = sum; \
        } \
    } \
} while(0)

#define MATRIX_MULTIPLY(a, b, result) do { \
    if ((a).cols != (b).rows) { \
        printf("Matrix dimensions incompatible for multiplication\n"); \
        return 1; \
    } \
    (result).rows = (a).rows; \
    (result).cols = (b).cols; \
    for (int i = 0; i < (a).rows; i++) { \
        for (int j = 0; j < (b).cols; j++) { \
            int sum = 0; \
            for (int k = 0; k < (a).cols; k++) { \
                long long product = (long long)(a).data[i][k] * (long long)(b).data[k][j]; \
                if (product > INT_MAX || product < INT_MIN) { \
                    printf("Integer overflow in multiplication\n"); \
                    return 1; \
                } \
                long long new_sum = (long long)sum + product; \
                if (new_sum > INT_MAX || new_sum < INT_MIN) { \
                    printf("Integer overflow in multiplication sum\n"); \
                    return 1; \
                } \
                sum = (int)new_sum; \
            } \
            (result).data[i][j] = sum; \
        } \
    } \
} while(0)

#define MATRIX_PRINT(m) do { \
    for (int i = 0; i < (m).rows; i++) { \
        for (int j = 0; j < (m).cols; j++) { \
            printf("%6d", (m).data[i][j]); \
        } \
        printf("\n"); \
    } \
} while(0)

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

void read_matrix_elements(Matrix* mat, const char* name) {
    printf("Enter elements for %s matrix:\n", name);
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            char prompt[64];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            int val = read_int(prompt, INT_MIN, INT_MAX);
            if (val == -1) {
                printf("Invalid input\n");
                exit(1);
            }
            mat->data[i][j] = val;
        }
    }
}

int main(void) {
    Matrix mat1, mat2, result;
    int rows1, cols1, rows2, cols2;
    int operation;
    
    printf("Matrix Operations Program\n");
    
    printf("Enter dimensions for first matrix:\n");
    rows1 = read_int("Rows (1-10): ", 1, MAX_ROWS);
    cols1 = read_int("Columns (1-10): ", 1, MAX_COLS);
    if