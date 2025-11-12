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
    printf("Enter matrix elements (%d x %d): ", m->rows, m->cols);
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
            printf("%4d", *(m->data + i * m->cols + j));
        }
        printf("\n");
    }
}

int matrix_multiply(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a->cols != b->rows) {
        return 0;
    }
    if (a->rows > MAX_DIM || b->cols > MAX_DIM) {
        return 0;
    }
    matrix_init(result, a->rows, b->cols);
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                int *a_elem = (int *)(a->data + i * a->cols + k);
                int *b_elem = (int *)(b->data + k * b->cols + j);
                if (*a_elem > 0 && *b_elem > 0 && *a_elem > INT_MAX / *b_elem) {
                    return 0;
                }
                if (*a_elem < 0 && *b_elem < 0 && *a_elem < INT_MAX / *b_elem) {
                    return 0;
                }
                int product = *a_elem * *b_elem;
                if ((*a_elem > 0 && *b_elem > 0 && product < 0) ||
                    (*a_elem < 0 && *b_elem < 0 && product < 0)) {
                    return 0;
                }
                if (sum > 0 && product > INT_MAX - sum) {
                    return 0;
                }
                if (sum < 0 && product < INT_MIN - sum) {
                    return 0;
                }
                sum += product;
            }
            *(result->data + i * result->cols + j) = sum;
        }
    }
    return 1;
}

int main(void) {
    Matrix a, b, result;
    int rows1, cols1, rows2, cols2;

    printf("Enter dimensions of first matrix (rows cols): ");
    if (scanf("%d %d", &rows1, &cols1) != 2) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    matrix_init(&a, rows1, cols1);
    matrix_read(&a);

    printf("Enter dimensions of second matrix (rows cols): ");
    if (scanf("%d %d", &rows2, &cols2) != 2) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    matrix_init(&b, rows2, cols2);
    matrix_read(&b);

    printf("\nMatrix A:\n");
    matrix_print(&a);
    printf("\nMatrix B:\n");
    matrix_print(&b);

    if (matrix_multiply(&a, &b, &result)) {
        printf("\nMatrix A * B:\n");
        matrix_print(&result);
    } else {
        printf("\nMatrix multiplication failed: dimension mismatch or integer overflow\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}