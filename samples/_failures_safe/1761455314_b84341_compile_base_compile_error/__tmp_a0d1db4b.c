//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ROWS 10
#define MAX_COLS 10
#define MIN_DIM 1

typedef struct {
    int data[MAX_ROWS][MAX_COLS];
    int rows;
    int cols;
} Matrix;

void matrix_init(Matrix *m, int rows, int cols) {
    if (rows < MIN_DIM || rows > MAX_ROWS || cols < MIN_DIM || cols > MAX_COLS) {
        fprintf(stderr, "Invalid matrix dimensions\n");
        exit(EXIT_FAILURE);
    }
    m->rows = rows;
    m->cols = cols;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            m->data[i][j] = 0;
        }
    }
}

void matrix_read(Matrix *m) {
    int rows, cols;
    printf("Enter number of rows (1-%d): ", MAX_ROWS);
    if (scanf("%d", &rows) != 1 || rows < MIN_DIM || rows > MAX_ROWS) {
        fprintf(stderr, "Invalid rows input\n");
        exit(EXIT_FAILURE);
    }
    printf("Enter number of columns (1-%d): ", MAX_COLS);
    if (scanf("%d", &cols) != 1 || cols < MIN_DIM || cols > MAX_COLS) {
        fprintf(stderr, "Invalid columns input\n");
        exit(EXIT_FAILURE);
    }
    matrix_init(m, rows, cols);
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (scanf("%d", &m->data[i][j]) != 1) {
                fprintf(stderr, "Invalid matrix element\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

void matrix_print(const Matrix *m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%6d", m->data[i][j]);
        }
        printf("\n");
    }
}

#define MATRIX_ADD(a, b, result) do { \
    if ((a).rows != (b).rows || (a).cols != (b).cols) { \
        fprintf(stderr, "Matrix dimensions mismatch for addition\n"); \
        exit(EXIT_FAILURE); \
    } \
    matrix_init(&(result), (a).rows, (a).cols); \
    for (int i = 0; i < (a).rows; i++) { \
        for (int j = 0; j < (a).cols; j++) { \
            if (((a).data[i][j] > 0 && (b).data[i][j] > INT_MAX - (a).data[i][j]) || \
                ((a).data[i][j] < 0 && (b).data[i][j] < INT_MIN - (a).data[i][j])) { \
                fprintf(stderr, "Integer overflow in addition\n"); \
                exit(EXIT_FAILURE); \
            } \
            (result).data[i][j] = (a).data[i][j] + (b).data[i][j]; \
        } \
    } \
} while(0)

#define MATRIX_MULTIPLY(a, b, result) do { \
    if ((a).cols != (b).rows) { \
        fprintf(stderr, "Matrix dimensions incompatible for multiplication\n"); \
        exit(EXIT_FAILURE); \
    } \
    matrix_init(&(result), (a).rows, (b).cols); \
    for (int i = 0; i < (a).rows; i++) { \
        for (int j = 0; j < (b).cols; j++) { \
            int sum = 0; \
            for (int k = 0; k < (a).cols; k++) { \
                long long product = (long long)(a).data[i][k] * (long long)(b).data[k][j]; \
                if (product > INT_MAX || product < INT_MIN) { \
                    fprintf(stderr, "Integer overflow in multiplication\n"); \
                    exit(EXIT_FAILURE); \
                } \
                if ((sum > 0 && product > INT_MAX - sum) || (sum < 0 && product < INT_MIN - sum)) { \
                    fprintf(stderr, "Integer overflow in summation\n"); \
                    exit(EXIT_FAILURE); \
                } \
                sum += (int)product; \
            } \
            (result).data[i][j] = sum; \
        } \
    } \
} while(0)

#define MATRIX_TRANSPOSE(a, result) do { \
    matrix_init(&(result), (a).cols, (a).rows); \
    for (int i = 0; i < (a).rows; i++) { \
        for (int j = 0; j < (a).cols; j++) { \
            (result).data[j][i] = (a).data[i][j]; \
        } \
    } \
} while(0)

int main() {
    Matrix mat1, mat2, result;
    int choice;
    
    printf("Matrix Operations