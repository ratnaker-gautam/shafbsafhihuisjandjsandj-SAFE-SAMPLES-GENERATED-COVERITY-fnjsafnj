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

int read_matrix(Matrix *mat) {
    if (scanf("%d %d", &mat->rows, &mat->cols) != 2) {
        return 0;
    }
    if (!validate_dimension(mat->rows) || !validate_dimension(mat->cols)) {
        return 0;
    }
    
    int *ptr = mat->data;
    int elements = mat->rows * mat->cols;
    for (int i = 0; i < elements; i++) {
        if (scanf("%d", ptr + i) != 1) {
            return 0;
        }
    }
    return 1;
}

void print_matrix(const Matrix *mat) {
    const int *ptr = mat->data;
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%d ", *(ptr + i * mat->cols + j));
        }
        printf("\n");
    }
}

int matrix_multiply(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a->cols != b->rows) {
        return 0;
    }
    
    result->rows = a->rows;
    result->cols = b->cols;
    
    int *res_ptr = result->data;
    const int *a_ptr = a->data;
    const int *b_ptr = b->data;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                int a_val = *(a_ptr + i * a->cols + k);
                int b_val = *(b_ptr + k * b->cols + j);
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

int matrix_transpose(const Matrix *src, Matrix *dest) {
    dest->rows = src->cols;
    dest->cols = src->rows;
    
    int *dest_ptr = dest->data;
    const int *src_ptr = src->data;
    
    for (int i = 0; i < src->rows; i++) {
        for (int j = 0; j < src->cols; j++) {
            *(dest_ptr + j * dest->cols + i) = *(src_ptr + i * src->cols + j);
        }
    }
    return 1;
}

int main() {
    Matrix mat1, mat2, result;
    
    printf("Enter first matrix (rows cols then elements):\n");
    if (!read_matrix(&mat1)) {
        printf("Invalid first matrix input\n");
        return 1;
    }
    
    printf("Enter second matrix (rows cols then elements):\n");
    if (!read_matrix(&mat2)) {
        printf("Invalid second matrix input\n");
        return 1;
    }
    
    printf("\nMatrix 1:\n");
    print_matrix(&mat1);
    printf("\nMatrix 2:\n");
    print_matrix(&mat2);
    
    printf("\nMatrix multiplication:\n");
    if (matrix_multiply(&mat1, &mat2, &result)) {
        print_matrix(&result);
    } else {
        printf("Multiplication failed: dimension mismatch or integer overflow\n");
    }
    
    printf("\nTranspose of Matrix 1:\n");
    matrix_transpose(&mat1, &result);
    print_matrix(&result);
    
    printf("\nTranspose of Matrix 2:\n");
    matrix_transpose(&mat2, &result);
    print_matrix(&result);
    
    return 0;
}