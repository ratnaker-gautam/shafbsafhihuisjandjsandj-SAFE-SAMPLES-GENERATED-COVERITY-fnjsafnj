//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 100

typedef struct {
    int rows;
    int cols;
    int data[MAX_DIM * MAX_DIM];
} Matrix;

int validate_dimension(int dim) {
    return dim > 0 && dim <= MAX_DIM;
}

int read_matrix(Matrix *m) {
    if (scanf("%d %d", &m->rows, &m->cols) != 2) {
        return 0;
    }
    if (!validate_dimension(m->rows) || !validate_dimension(m->cols)) {
        return 0;
    }
    int elements = m->rows * m->cols;
    for (int i = 0; i < elements; i++) {
        if (scanf("%d", m->data + i) != 1) {
            return 0;
        }
    }
    return 1;
}

void print_matrix(const Matrix *m) {
    printf("%d %d\n", m->rows, m->cols);
    int elements = m->rows * m->cols;
    for (int i = 0; i < elements; i++) {
        printf("%d", *(m->data + i));
        if ((i + 1) % m->cols == 0) {
            printf("\n");
        } else {
            printf(" ");
        }
    }
}

int multiply_matrices(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a->cols != b->rows) {
        return 0;
    }
    result->rows = a->rows;
    result->cols = b->cols;
    int *res_ptr = result->data;
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                int a_val = *(a->data + i * a->cols + k);
                int b_val = *(b->data + k * b->cols + j);
                if ((a_val > 0 && b_val > 0 && a_val > INT_MAX / b_val) ||
                    (a_val < 0 && b_val < 0 && a_val < INT_MAX / b_val) ||
                    (a_val > 0 && b_val < 0 && b_val < INT_MIN / a_val) ||
                    (a_val < 0 && b_val > 0 && a_val < INT_MIN / b_val)) {
                    return 0;
                }
                int product = a_val * b_val;
                if ((product > 0 && sum > INT_MAX - product) ||
                    (product < 0 && sum < INT_MIN - product)) {
                    return 0;
                }
                sum += product;
            }
            *res_ptr = sum;
            res_ptr++;
        }
    }
    return 1;
}

int transpose_matrix(const Matrix *m, Matrix *result) {
    result->rows = m->cols;
    result->cols = m->rows;
    int *res_ptr = result->data;
    for (int j = 0; j < m->cols; j++) {
        for (int i = 0; i < m->rows; i++) {
            *res_ptr = *(m->data + i * m->cols + j);
            res_ptr++;
        }
    }
    return 1;
}

int main() {
    Matrix a, b, result;
    
    printf("Enter first matrix (rows cols elements):\n");
    if (!read_matrix(&a)) {
        printf("Invalid first matrix\n");
        return 1;
    }
    
    printf("Enter second matrix (rows cols elements):\n");
    if (!read_matrix(&b)) {
        printf("Invalid second matrix\n");
        return 1;
    }
    
    printf("\nMatrix A:\n");
    print_matrix(&a);
    
    printf("\nMatrix B:\n");
    print_matrix(&b);
    
    printf("\nTranspose of A:\n");
    if (transpose_matrix(&a, &result)) {
        print_matrix(&result);
    }
    
    printf("\nTranspose of B:\n");
    if (transpose_matrix(&b, &result)) {
        print_matrix(&result);
    }
    
    printf("\nA * B:\n");
    if (multiply_matrices(&a, &b, &result)) {
        print_matrix(&result);
    } else {
        printf("Multiplication failed: dimension mismatch or integer overflow\n");
    }
    
    printf("\nB * A:\n");
    if (multiply_matrices(&b, &a, &result)) {
        print_matrix(&result);
    } else {
        printf("Multiplication failed: dimension mismatch or integer overflow\n");
    }
    
    return 0;
}