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
    if (rows <= 0 || cols <= 0 || rows > MAX_SIZE || cols > MAX_SIZE) {
        fprintf(stderr, "Invalid matrix dimensions\n");
        exit(EXIT_FAILURE);
    }
    m->rows = rows;
    m->cols = cols;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            *(*(m->data + i) + j) = 0;
        }
    }
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

int matrix_multiply(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a->cols != b->rows) {
        return 0;
    }
    if (result->rows != a->rows || result->cols != b->cols) {
        return 0;
    }
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                if ((*(*(a->data + i) + k) > 0 && *(*(b->data + k) + j) > INT_MAX / *(*(a->data + i) + k)) ||
                    (*(*(a->data + i) + k) < 0 && *(*(b->data + k) + j) < INT_MIN / *(*(a->data + i) + k))) {
                    return 0;
                }
                int product = *(*(a->data + i) + k) * *(*(b->data + k) + j);
                if ((product > 0 && sum > INT_MAX - product) ||
                    (product < 0 && sum < INT_MIN - product)) {
                    return 0;
                }
                sum += product;
            }
            *(*(result->data + i) + j) = sum;
        }
    }
    return 1;
}

int main(void) {
    Matrix mat1, mat2, result;
    int rows1, cols1, rows2, cols2;

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
    matrix_init(&mat2, rows2, cols2);
    matrix_input(&mat2);

    if (cols1 != rows2) {
        fprintf(stderr, "Matrix multiplication not possible\n");
        return EXIT_FAILURE;
    }

    matrix_init(&result, rows1, cols2);
    if (!matrix_multiply(&mat1, &mat2, &result)) {
        fprintf(stderr, "Matrix multiplication failed\n");
        return EXIT_FAILURE;
    }

    printf("Result matrix:\n");
    matrix_print(&result);

    return EXIT_SUCCESS;
}