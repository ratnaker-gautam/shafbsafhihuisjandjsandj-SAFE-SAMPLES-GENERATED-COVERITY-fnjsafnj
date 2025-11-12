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

void matrix_init(Matrix *m, int rows, int cols) {
    if (rows <= 0 || rows > MAX_ROWS || cols <= 0 || cols > MAX_COLS) {
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

void matrix_input(Matrix *m) {
    printf("Enter matrix elements (%d x %d):\n", m->rows, m->cols);
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            if (scanf("%d", &m->data[i][j]) != 1) {
                fprintf(stderr, "Invalid input\n");
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

#define MATRIX_ADD(res, a, b) do { \
    if ((a)->rows != (b)->rows || (a)->cols != (b)->cols) { \
        fprintf(stderr, "Matrix dimensions mismatch\n"); \
        exit(EXIT_FAILURE); \
    } \
    matrix_init(res, (a)->rows, (a)->cols); \
    for (int i = 0; i < (a)->rows; i++) { \
        for (int j = 0; j < (a)->cols; j++) { \
            if (((a)->data[i][j] > 0 && (b)->data[i][j] > INT_MAX - (a)->data[i][j]) || \
                ((a)->data[i][j] < 0 && (b)->data[i][j] < INT_MIN - (a)->data[i][j])) { \
                fprintf(stderr, "Integer overflow\n"); \
                exit(EXIT_FAILURE); \
            } \
            (res)->data[i][j] = (a)->data[i][j] + (b)->data[i][j]; \
        } \
    } \
} while(0)

#define MATRIX_MULTIPLY(res, a, b) do { \
    if ((a)->cols != (b)->rows) { \
        fprintf(stderr, "Matrix dimensions incompatible\n"); \
        exit(EXIT_FAILURE); \
    } \
    matrix_init(res, (a)->rows, (b)->cols); \
    for (int i = 0; i < (a)->rows; i++) { \
        for (int j = 0; j < (b)->cols; j++) { \
            int sum = 0; \
            for (int k = 0; k < (a)->cols; k++) { \
                long long product = (long long)(a)->data[i][k] * (b)->data[k][j]; \
                if (product > INT_MAX || product < INT_MIN) { \
                    fprintf(stderr, "Integer overflow\n"); \
                    exit(EXIT_FAILURE); \
                } \
                if ((sum > 0 && product > INT_MAX - sum) || \
                    (sum < 0 && product < INT_MIN - sum)) { \
                    fprintf(stderr, "Integer overflow\n"); \
                    exit(EXIT_FAILURE); \
                } \
                sum += (int)product; \
            } \
            (res)->data[i][j] = sum; \
        } \
    } \
} while(0)

#define MATRIX_TRANSPOSE(res, a) do { \
    matrix_init(res, (a)->cols, (a)->rows); \
    for (int i = 0; i < (a)->rows; i++) { \
        for (int j = 0; j < (a)->cols; j++) { \
            (res)->data[j][i] = (a)->data[i][j]; \
        } \
    } \
} while(0)

int main() {
    Matrix mat1, mat2, result;
    int rows1, cols1, rows2, cols2;
    int choice;

    printf("Enter dimensions for first matrix (rows cols): ");
    if (scanf("%d %d", &rows1, &cols1) != 2) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    matrix_init(&mat1, rows1, cols1);
    matrix_input(&mat1);

    printf("Enter dimensions for second matrix (rows cols): ");
    if (scanf("%d %d", &rows2, &cols2) != 2) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    matrix_init(&mat2,