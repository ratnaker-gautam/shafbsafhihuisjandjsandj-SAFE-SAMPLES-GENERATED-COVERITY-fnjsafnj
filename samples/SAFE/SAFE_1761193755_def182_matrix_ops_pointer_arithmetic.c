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
    int *ptr = m->data;
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%d ", *(ptr + i * m->cols + j));
        }
        printf("\n");
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
                if ((a_val > 0 && b_val > INT_MAX / a_val) || 
                    (a_val < 0 && b_val < INT_MAX / a_val)) {
                    return 0;
                }
                sum += a_val * b_val;
                if ((sum > 0 && a_val * b_val > INT_MAX - sum) ||
                    (sum < 0 && a_val * b_val < INT_MIN - sum)) {
                    return 0;
                }
            }
            *(res_ptr + i * result->cols + j) = sum;
        }
    }
    return 1;
}

int transpose_matrix(const Matrix *src, Matrix *dest) {
    dest->rows = src->cols;
    dest->cols = src->rows;
    int *src_ptr = src->data;
    int *dest_ptr = dest->data;
    for (int i = 0; i < src->rows; i++) {
        for (int j = 0; j < src->cols; j++) {
            *(dest_ptr + j * dest->cols + i) = *(src_ptr + i * src->cols + j);
        }
    }
    return 1;
}

int main() {
    Matrix mat1, mat2, result;
    
    printf("Enter first matrix (rows cols elements): ");
    if (!read_matrix(&mat1)) {
        printf("Invalid first matrix input\n");
        return 1;
    }
    
    printf("Enter second matrix (rows cols elements): ");
    if (!read_matrix(&mat2)) {
        printf("Invalid second matrix input\n");
        return 1;
    }
    
    printf("\nMatrix 1:\n");
    print_matrix(&mat1);
    printf("\nMatrix 2:\n");
    print_matrix(&mat2);
    
    printf("\nMatrix 1 transpose:\n");
    if (transpose_matrix(&mat1, &result)) {
        print_matrix(&result);
    }
    
    printf("\nMatrix multiplication:\n");
    if (multiply_matrices(&mat1, &mat2, &result)) {
        print_matrix(&result);
    } else {
        printf("Multiplication failed: dimension mismatch or overflow\n");
    }
    
    return 0;
}