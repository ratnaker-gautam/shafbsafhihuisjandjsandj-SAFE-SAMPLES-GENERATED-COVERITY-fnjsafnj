//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 10

typedef struct {
    int rows;
    int cols;
    int data[MAX_SIZE][MAX_SIZE];
} Matrix;

void matrix_init(Matrix *m, int rows, int cols) {
    if (rows <= 0 || rows > MAX_SIZE || cols <= 0 || cols > MAX_SIZE) {
        fprintf(stderr, "Invalid matrix dimensions\n");
        exit(EXIT_FAILURE);
    }
    m->rows = rows;
    m->cols = cols;
    memset(m->data, 0, sizeof(m->data));
}

void matrix_input(Matrix *m) {
    printf("Enter matrix elements (%d x %d):\n", m->rows, m->cols);
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            if (scanf("%d", *(m->data + i) + j) != 1) {
                fprintf(stderr, "Invalid input\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

void matrix_print(const Matrix *m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%d ", *(*(m->data + i) + j));
        }
        printf("\n");
    }
}

void matrix_add(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a->rows != b->rows || a->cols != b->cols) {
        fprintf(stderr, "Matrix dimensions don't match for addition\n");
        exit(EXIT_FAILURE);
    }
    matrix_init(result, a->rows, a->cols);
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            int sum = *(*(a->data + i) + j) + *(*(b->data + i) + j);
            if ((*(*(a->data + i) + j) > 0 && *(*(b->data + i) + j) > INT_MAX - *(*(a->data + i) + j)) ||
                (*(*(a->data + i) + j) < 0 && *(*(b->data + i) + j) < INT_MIN - *(*(a->data + i) + j))) {
                fprintf(stderr, "Integer overflow in addition\n");
                exit(EXIT_FAILURE);
            }
            *(*(result->data + i) + j) = sum;
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
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                int product = *(*(a->data + i) + k) * *(*(b->data + k) + j);
                if (*(*(a->data + i) + k) != 0 && product / *(*(a->data + i) + k) != *(*(b->data + k) + j)) {
                    fprintf(stderr, "Integer overflow in multiplication\n");
                    exit(EXIT_FAILURE);
                }
                if ((product > 0 && sum > INT_MAX - product) || (product < 0 && sum < INT_MIN - product)) {
                    fprintf(stderr, "Integer overflow in summation\n");
                    exit(EXIT_FAILURE);
                }
                sum += product;
            }
            *(*(result->data + i) + j) = sum;
        }
    }
}

void matrix_transpose(const Matrix *m, Matrix *result) {
    matrix_init(result, m->cols, m->rows);
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            *(*(result->data + j) + i) = *(*(m->data + i) + j);
        }
    }
}

int main() {
    Matrix a, b, result;
    int rows1, cols1, rows2, cols2;
    int choice;

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

    printf("\nMatrix A:\n");
    matrix_print(&a);
    printf("\nMatrix B:\n");
    matrix_print(&b);

    printf("\n