//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 16

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

void matrix_read(Matrix *m) {
    printf("Enter matrix dimensions (rows cols): ");
    if (scanf("%d %d", &m->rows, &m->cols) != 2) {
        fprintf(stderr, "Invalid input\n");
        exit(EXIT_FAILURE);
    }
    if (m->rows <= 0 || m->cols <= 0 || m->rows > MAX_DIM || m->cols > MAX_DIM) {
        fprintf(stderr, "Invalid matrix dimensions\n");
        exit(EXIT_FAILURE);
    }
    printf("Enter %d elements:\n", m->rows * m->cols);
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

void matrix_print(Matrix *m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            int *elem = matrix_at(m, i, j);
            printf("%d ", *elem);
        }
        printf("\n");
    }
}

void matrix_add(Matrix *a, Matrix *b, Matrix *result) {
    if (a->rows != b->rows || a->cols != b->cols) {
        fprintf(stderr, "Matrix dimensions do not match for addition\n");
        exit(EXIT_FAILURE);
    }
    matrix_init(result, a->rows, a->cols);
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            int *elem_a = matrix_at(a, i, j);
            int *elem_b = matrix_at(b, i, j);
            int *elem_r = matrix_at(result, i, j);
            if (elem_a && elem_b && elem_r) {
                if ((*elem_b > 0 && *elem_a > INT_MAX - *elem_b) ||
                    (*elem_b < 0 && *elem_a < INT_MIN - *elem_b)) {
                    fprintf(stderr, "Integer overflow in addition\n");
                    exit(EXIT_FAILURE);
                }
                *elem_r = *elem_a + *elem_b;
            }
        }
    }
}

void matrix_multiply(Matrix *a, Matrix *b, Matrix *result) {
    if (a->cols != b->rows) {
        fprintf(stderr, "Matrix dimensions do not match for multiplication\n");
        exit(EXIT_FAILURE);
    }
    matrix_init(result, a->rows, b->cols);
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int *elem_r = matrix_at(result, i, j);
            if (elem_r) {
                *elem_r = 0;
                for (int k = 0; k < a->cols; k++) {
                    int *elem_a = matrix_at(a, i, k);
                    int *elem_b = matrix_at(b, k, j);
                    if (elem_a && elem_b) {
                        long long product = (long long)*elem_a * (long long)*elem_b;
                        if (product > INT_MAX || product < INT_MIN) {
                            fprintf(stderr, "Integer overflow in multiplication\n");
                            exit(EXIT_FAILURE);
                        }
                        if ((*elem_r > 0 && product > INT_MAX - *elem_r) ||
                            (*elem_r < 0 && product < INT_MIN - *elem_r)) {
                            fprintf(stderr, "Integer overflow in summation\n");
                            exit(EXIT_FAILURE);
                        }
                        *elem_r += (int)product;
                    }
                }
            }
        }
    }
}

int main() {
    Matrix a, b, result;
    int choice;
    
    printf("Matrix A:\n");
    matrix_read(&a);
    printf("Matrix B:\n");
    matrix_read(&b);
    
    printf("Choose operation:\n");
    printf("1. Addition\n");
    printf("2. Multiplication\n");
    if (scanf("%d", &choice) != 1) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    
    switch (choice) {