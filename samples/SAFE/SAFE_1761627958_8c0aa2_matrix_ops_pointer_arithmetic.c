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
    if (a->rows > MAX_SIZE || b->cols > MAX_SIZE) {
        return 0;
    }
    matrix_init(result, a->rows, b->cols);
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                if ((*(*(a->data + i) + k) > 0 && *(*(b->data + k) + j) > INT_MAX - sum) ||
                    (*(*(a->data + i) + k) < 0 && *(*(b->data + k) + j) < INT_MIN - sum)) {
                    return 0;
                }
                sum += *(*(a->data + i) + k) * *(*(b->data + k) + j);
            }
            *(*(result->data + i) + j) = sum;
        }
    }
    return 1;
}

int matrix_transpose(const Matrix *m, Matrix *result) {
    if (m->rows > MAX_SIZE || m->cols > MAX_SIZE) {
        return 0;
    }
    matrix_init(result, m->cols, m->rows);
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            *(*(result->data + j) + i) = *(*(m->data + i) + j);
        }
    }
    return 1;
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
    matrix_input(&a);
    
    printf("Enter dimensions for second matrix (rows cols): ");
    if (scanf("%d %d", &rows, &cols) != 2) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    matrix_init(&b, rows, cols);
    matrix_input(&b);
    
    printf("\nMatrix A:\n");
    matrix_print(&a);
    printf("\nMatrix B:\n");
    matrix_print(&b);
    
    printf("\nMatrix multiplication:\n");
    if (matrix_multiply(&a, &b, &result)) {
        matrix_print(&result);
    } else {
        printf("Multiplication not possible\n");
    }
    
    printf("\nTranspose of A:\n");
    if (matrix_transpose(&a, &result)) {
        matrix_print(&result);
    } else {
        printf("Transpose not possible\n");
    }
    
    printf("\nTranspose of B:\n");
    if (matrix_transpose(&b, &result)) {
        matrix_print(&result);
    } else {
        printf("Transpose not possible\n");
    }
    
    return EXIT_SUCCESS;
}