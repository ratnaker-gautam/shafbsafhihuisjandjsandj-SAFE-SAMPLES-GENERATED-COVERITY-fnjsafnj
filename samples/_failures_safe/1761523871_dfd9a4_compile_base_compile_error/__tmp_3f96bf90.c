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
    memset(m->data, 0, sizeof(m->data));
}

void matrix_read(Matrix *m) {
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

void matrix_add(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a->rows != b->rows || a->cols != b->cols) {
        fprintf(stderr, "Matrix dimensions do not match for addition\n");
        exit(EXIT_FAILURE);
    }
    matrix_init(result, a->rows, a->cols);
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            if ((a->data[i][j] > 0 && b->data[i][j] > INT_MAX - a->data[i][j]) ||
                (a->data[i][j] < 0 && b->data[i][j] < INT_MIN - a->data[i][j])) {
                fprintf(stderr, "Integer overflow in addition\n");
                exit(EXIT_FAILURE);
            }
            result->data[i][j] = a->data[i][j] + b->data[i][j];
        }
    }
}

void matrix_multiply(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a->cols != b->rows) {
        fprintf(stderr, "Matrix dimensions incompatible for multiplication\n");
        exit(EXIT_FAILURE);
    }
    matrix_init(result, a->rows, b->cols);
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            long long sum = 0;
            for (int k = 0; k < a->cols; k++) {
                long long product = (long long)a->data[i][k] * b->data[k][j];
                if (a->data[i][k] != 0 && product / a->data[i][k] != b->data[k][j]) {
                    fprintf(stderr, "Integer overflow in multiplication\n");
                    exit(EXIT_FAILURE);
                }
                if ((product > 0 && sum > LLONG_MAX - product) ||
                    (product < 0 && sum < LLONG_MIN - product)) {
                    fprintf(stderr, "Integer overflow in summation\n");
                    exit(EXIT_FAILURE);
                }
                sum += product;
            }
            if (sum > INT_MAX || sum < INT_MIN) {
                fprintf(stderr, "Result exceeds integer range\n");
                exit(EXIT_FAILURE);
            }
            result->data[i][j] = (int)sum;
        }
    }
}

void matrix_transpose(const Matrix *m, Matrix *result) {
    matrix_init(result, m->cols, m->rows);
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            result->data[j][i] = m->data[i][j];
        }
    }
}

int main() {
    Matrix a, b, result;
    int rows, cols;
    
    printf("Enter dimensions for first matrix (rows cols): ");
    if (scanf("%d %d", &rows, &cols) != 2) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    matrix_init(&a, rows, cols);
    matrix_read(&a);
    
    printf("Enter dimensions for second matrix (rows cols): ");
    if (scanf("%d %d", &rows, &cols) != 2) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    matrix_init(&b, rows, cols);
    matrix_read(&b);
    
    printf("\nMatrix A:\n");
    matrix_print(&a);
    printf("\nMatrix B:\n");
    matrix_print(&b);
    
    printf("\nMatrix A + B:\n");
    matrix_add(&a, &b, &result);
    matrix_print