//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: matrix_ops
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

#define MATRIX_ADD(res, a, b) do { \
    for (int i = 0; i < (a).rows; i++) { \
        for (int j = 0; j < (a).cols; j++) { \
            (res).data[i][j] = (a).data[i][j] + (b).data[i][j]; \
        } \
    } \
    (res).rows = (a).rows; \
    (res).cols = (a).cols; \
} while(0)

#define MATRIX_MULTIPLY(res, a, b) do { \
    for (int i = 0; i < (a).rows; i++) { \
        for (int j = 0; j < (b).cols; j++) { \
            int sum = 0; \
            for (int k = 0; k < (a).cols; k++) { \
                sum += (a).data[i][k] * (b).data[k][j]; \
            } \
            (res).data[i][j] = sum; \
        } \
    } \
    (res).rows = (a).rows; \
    (res).cols = (b).cols; \
} while(0)

#define MATRIX_PRINT(m) do { \
    for (int i = 0; i < (m).rows; i++) { \
        for (int j = 0; j < (m).cols; j++) { \
            printf("%4d", (m).data[i][j]); \
        } \
        printf("\n"); \
    } \
} while(0)

int safe_str_to_int(const char* str, int* result) {
    char* endptr;
    long val = strtol(str, &endptr, 10);
    if (endptr == str || *endptr != '\0') return 0;
    if (val > INT_MAX || val < INT_MIN) return 0;
    *result = (int)val;
    return 1;
}

int read_matrix(Matrix* m) {
    char input[256];
    int rows, cols;
    
    printf("Enter number of rows (1-%d): ", MAX_ROWS);
    if (fgets(input, sizeof(input), stdin) == NULL) return 0;
    if (!safe_str_to_int(input, &rows)) return 0;
    if (rows < 1 || rows > MAX_ROWS) return 0;
    
    printf("Enter number of columns (1-%d): ", MAX_COLS);
    if (fgets(input, sizeof(input), stdin) == NULL) return 0;
    if (!safe_str_to_int(input, &cols)) return 0;
    if (cols < 1 || cols > MAX_COLS) return 0;
    
    MATRIX_INIT(*m, rows, cols);
    
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("Element [%d][%d]: ", i, j);
            if (fgets(input, sizeof(input), stdin) == NULL) return 0;
            int value;
            if (!safe_str_to_int(input, &value)) return 0;
            MATRIX_SET(*m, i, j, value);
        }
    }
    return 1;
}

int main() {
    Matrix a, b, result;
    char choice[256];
    
    printf("Matrix Operations Program\n");
    printf("1. Matrix Addition\n");
    printf("2. Matrix Multiplication\n");
    printf("Choose operation (1 or 2): ");
    
    if (fgets(choice, sizeof(choice), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    
    int op;
    if (!safe_str_to_int(choice, &op) || (op != 1 && op != 2)) {
        printf("Invalid choice\n");
        return 1;
    }
    
    printf("Enter first matrix:\n");
    if (!read_matrix(&a)) {
        printf("Invalid matrix input\n");
        return 1;
    }
    
    printf("Enter second matrix:\n");
    if (!read_matrix(&b)) {
        printf("Invalid matrix input\n");
        return 1;
    }
    
    MATRIX_INIT(result, 0, 0);
    
    if (op == 1) {
        if (a.rows != b.rows || a.cols != b.cols) {
            printf("Matrix dimensions must match for addition\n");
            return 1;
        }