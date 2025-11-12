//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 10

typedef struct {
    int rows;
    int cols;
    int data[MAX_DIM * MAX_DIM];
} Matrix;

void matrix_init(Matrix *m, int rows, int cols) {
    if (rows <= 0 || cols <= 0 || rows > MAX_DIM || cols > MAX_DIM) {
        fprintf(stderr, "Invalid matrix dimensions\n");
        exit(EXIT_FAILURE);
    }
    m->rows = rows;
    m->cols = cols;
    memset(m->data, 0, sizeof(m->data));
}

int *matrix_at(Matrix *m, int row, int col) {
    if (row < 0 || row >= m->rows || col < 0 || col >= m->cols) {
        return NULL;
    }
    return m->data + row * m->cols + col;
}

void matrix_input(Matrix *m) {
    printf("Enter matrix elements (%d x %d):\n", m->rows, m->cols);
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            int *elem = matrix_at(m, i, j);
            if (scanf("%d", elem) != 1) {
                fprintf(stderr, "Invalid input\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

void matrix_print(const Matrix *m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%d ", *(m->data + i * m->cols + j));
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
    int *a_ptr = a->data;
    int *b_ptr = b->data;
    int *r_ptr = result->data;
    int total = a->rows * a->cols;
    for (int i = 0; i < total; i++) {
        if ((*b_ptr > 0 && *a_ptr > INT_MAX - *b_ptr) ||
            (*b_ptr < 0 && *a_ptr < INT_MIN - *b_ptr)) {
            fprintf(stderr, "Integer overflow in addition\n");
            exit(EXIT_FAILURE);
        }
        *r_ptr = *a_ptr + *b_ptr;
        a_ptr++;
        b_ptr++;
        r_ptr++;
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
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                int a_val = *(a->data + i * a->cols + k);
                int b_val = *(b->data + k * b->cols + j);
                if (a_val != 0 && b_val != 0) {
                    if ((a_val > 0 && b_val > 0 && a_val > INT_MAX / b_val) ||
                        (a_val > 0 && b_val < 0 && b_val < INT_MIN / a_val) ||
                        (a_val < 0 && b_val > 0 && a_val < INT_MIN / b_val) ||
                        (a_val < 0 && b_val < 0 && a_val < INT_MAX / b_val)) {
                        fprintf(stderr, "Integer overflow in multiplication\n");
                        exit(EXIT_FAILURE);
                    }
                }
                int product = a_val * b_val;
                if ((product > 0 && sum > INT_MAX - product) ||
                    (product < 0 && sum < INT_MIN - product)) {
                    fprintf(stderr, "Integer overflow in summation\n");
                    exit(EXIT_FAILURE);
                }
                sum += product;
            }
            *(result->data + i * result->cols + j) = sum;
        }
    }
}

int main(void) {
    Matrix a, b, result;
    int rows1, cols1, rows2, cols2;
    
    printf("Enter dimensions for first matrix (rows cols): ");
    if (scanf("%d %d", &rows1, &cols1) != 2) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    matrix_init(&a, rows1, cols1);
    matrix_input(&a);
    
    printf("Enter dimensions for second matrix (rows cols): ");
    if (scanf("%d %d", &rows2, &cols2) != 2) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    matrix_init(&b, rows2, cols2);
    matrix_input(&b);
    
    printf