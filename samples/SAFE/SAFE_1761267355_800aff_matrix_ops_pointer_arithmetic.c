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

int matrix_multiply(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a->cols != b->rows) {
        return 0;
    }
    if (a->rows > MAX_DIM || b->cols > MAX_DIM) {
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
                if ((a_val > 0 && b_val > 0 && sum > INT_MAX - product) ||
                    (a_val < 0 && b_val < 0 && sum > INT_MAX - product) ||
                    (a_val * b_val < 0 && sum < INT_MIN - product)) {
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

int main(void) {
    Matrix a, b, result;
    
    printf("Enter first matrix (rows cols then elements):\n");
    if (!read_matrix(&a)) {
        printf("Invalid first matrix input\n");
        return 1;
    }
    
    printf("Enter second matrix (rows cols then elements):\n");
    if (!read_matrix(&b)) {
        printf("Invalid second matrix input\n");
        return 1;
    }
    
    if (!matrix_multiply(&a, &b, &result)) {
        printf("Matrix multiplication failed: dimension mismatch or integer overflow\n");
        return 1;
    }
    
    printf("Result matrix:\n");
    print_matrix(&result);
    
    return 0;
}