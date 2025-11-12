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

#define MATRIX_VALID_DIMS(r, c) ((r) > 0 && (r) <= MAX_ROWS && (c) > 0 && (c) <= MAX_COLS)

#define MATRIX_ADD(a, b, result) do { \
    if ((a).rows != (b).rows || (a).cols != (b).cols) { \
        printf("Matrix dimensions mismatch for addition\n"); \
        return; \
    } \
    for (int i = 0; i < (a).rows; i++) { \
        for (int j = 0; j < (a).cols; j++) { \
            (result).data[i][j] = (a).data[i][j] + (b).data[i][j]; \
        } \
    } \
    (result).rows = (a).rows; \
    (result).cols = (a).cols; \
} while(0)

#define MATRIX_MULTIPLY(a, b, result) do { \
    if ((a).cols != (b).rows) { \
        printf("Matrix dimensions incompatible for multiplication\n"); \
        return; \
    } \
    for (int i = 0; i < (a).rows; i++) { \
        for (int j = 0; j < (b).cols; j++) { \
            int sum = 0; \
            for (int k = 0; k < (a).cols; k++) { \
                sum += (a).data[i][k] * (b).data[k][j]; \
            } \
            (result).data[i][j] = sum; \
        } \
    } \
    (result).rows = (a).rows; \
    (result).cols = (b).cols; \
} while(0)

#define MATRIX_PRINT(m) do { \
    for (int i = 0; i < (m).rows; i++) { \
        for (int j = 0; j < (m).cols; j++) { \
            printf("%6d", (m).data[i][j]); \
        } \
        printf("\n"); \
    } \
} while(0)

#define MATRIX_INPUT(m) do { \
    printf("Enter matrix elements row by row:\n"); \
    for (int i = 0; i < (m).rows; i++) { \
        for (int j = 0; j < (m).cols; j++) { \
            if (scanf("%d", &(m).data[i][j]) != 1) { \
                printf("Invalid input\n"); \
                return; \
            } \
        } \
    } \
} while(0)

int read_int(const char* prompt, int min, int max) {
    int value;
    while (1) {
        printf("%s", prompt);
        if (scanf("%d", &value) != 1) {
            printf("Invalid input\n");
            while (getchar() != '\n');
            continue;
        }
        if (value >= min && value <= max) {
            return value;
        }
        printf("Value must be between %d and %d\n", min, max);
    }
}

void matrix_add_wrapper(Matrix a, Matrix b, Matrix* result) {
    MATRIX_ADD(a, b, *result);
}

void matrix_multiply_wrapper(Matrix a, Matrix b, Matrix* result) {
    MATRIX_MULTIPLY(a, b, *result);
}

void matrix_input_wrapper(Matrix* m) {
    MATRIX_INPUT(*m);
}

int main() {
    Matrix mat1, mat2, result;
    int operation;
    
    printf("Matrix Operations Program\n");
    printf("1. Matrix Addition\n");
    printf("2. Matrix Multiplication\n");
    
    operation = read_int("Select operation (1-2): ", 1, 2);
    
    printf("Enter dimensions for first matrix:\n");
    int rows1 = read_int("Rows (1-10): ", 1, MAX_ROWS);
    int cols1 = read_int("Columns (1-10): ", 1, MAX_COLS);
    
    MATRIX_INIT(mat1, rows1, cols1);
    printf("First matrix:\n");
    matrix_input_wrapper(&mat1);
    
    printf("Enter dimensions for second matrix:\n");
    int rows2 = read_int("Rows (1-10): ", 1, MAX_ROWS);
    int cols2 = read_int("Columns (1-10): ", 1, MAX_COLS);
    
    MATRIX_INIT(mat2, rows2, cols2);
    printf("Second matrix:\n");
    matrix_input_wrapper(&mat2);
    
    MATRIX_INIT(result, 0, 0);
    
    switch (operation) {
        case 1:
            matrix_add_wrapper(mat1, mat2, &