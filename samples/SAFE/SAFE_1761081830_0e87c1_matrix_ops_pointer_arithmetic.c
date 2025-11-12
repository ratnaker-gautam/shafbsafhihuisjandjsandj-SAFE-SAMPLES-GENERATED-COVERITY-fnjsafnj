//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: matrix_ops
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

int matrix_transpose(const Matrix *src, Matrix *dest) {
    if (src->rows != dest->cols || src->cols != dest->rows) {
        return 0;
    }
    for (int i = 0; i < src->rows; i++) {
        for (int j = 0; j < src->cols; j++) {
            *(*(dest->data + j) + i) = *(*(src->data + i) + j);
        }
    }
    return 1;
}

int main() {
    Matrix a, b, result, transposed;
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
    
    printf("\nMatrix A:\n");
    matrix_print(&a);
    printf("\nMatrix B:\n");
    matrix_print(&b);
    
    matrix_init(&result, a.rows, b.cols);
    if (matrix_multiply(&a, &b, &result)) {
        printf("\nMatrix A * B:\n");
        matrix_print(&result);
    } else {
        printf("\nMatrix multiplication failed: dimension mismatch or integer overflow\n");
    }
    
    matrix_init(&transposed, a.cols, a.rows);
    if (matrix_transpose(&a, &transposed)) {
        printf("\nTranspose of A:\n");
        matrix_print(&transposed);
    } else {
        printf("\nTranspose failed\n");
    }
    
    return EXIT_SUCCESS;
}