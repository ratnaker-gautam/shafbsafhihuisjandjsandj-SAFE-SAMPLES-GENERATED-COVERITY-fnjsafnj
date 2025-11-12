//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 10

typedef struct {
    int rows;
    int cols;
    int data[MAX_DIM][MAX_DIM];
} Matrix;

int validate_dimension(int dim) {
    return dim > 0 && dim <= MAX_DIM;
}

int read_matrix(Matrix *mat) {
    if (mat == NULL) return 0;
    
    printf("Enter number of rows (1-%d): ", MAX_DIM);
    if (scanf("%d", &mat->rows) != 1) return 0;
    if (!validate_dimension(mat->rows)) return 0;
    
    printf("Enter number of columns (1-%d): ", MAX_DIM);
    if (scanf("%d", &mat->cols) != 1) return 0;
    if (!validate_dimension(mat->cols)) return 0;
    
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            if (scanf("%d", &mat->data[i][j]) != 1) return 0;
        }
    }
    return 1;
}

void print_matrix(const Matrix *mat) {
    if (mat == NULL) return;
    
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%6d", mat->data[i][j]);
        }
        printf("\n");
    }
}

int multiply_matrices(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a == NULL || b == NULL || result == NULL) return 0;
    if (a->cols != b->rows) return 0;
    if (!validate_dimension(a->rows) || !validate_dimension(a->cols) ||
        !validate_dimension(b->rows) || !validate_dimension(b->cols)) return 0;
    
    result->rows = a->rows;
    result->cols = b->cols;
    
    for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->cols; j++) {
            int64_t sum = 0;
            for (int k = 0; k < a->cols; k++) {
                int64_t product = (int64_t)a->data[i][k] * (int64_t)b->data[k][j];
                if (product > INT_MAX || product < INT_MIN) return 0;
                sum += product;
                if (sum > INT_MAX || sum < INT_MIN) return 0;
            }
            result->data[i][j] = (int)sum;
        }
    }
    return 1;
}

int transpose_matrix(const Matrix *input, Matrix *result) {
    if (input == NULL || result == NULL) return 0;
    if (!validate_dimension(input->rows) || !validate_dimension(input->cols)) return 0;
    
    result->rows = input->cols;
    result->cols = input->rows;
    
    for (int i = 0; i < input->rows; i++) {
        for (int j = 0; j < input->cols; j++) {
            result->data[j][i] = input->data[i][j];
        }
    }
    return 1;
}

int main() {
    Matrix mat1, mat2, result;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n\n");
    
    printf("First matrix:\n");
    if (!read_matrix(&mat1)) {
        printf("Error reading first matrix\n");
        return 1;
    }
    
    printf("\nSecond matrix:\n");
    if (!read_matrix(&mat2)) {
        printf("Error reading second matrix\n");
        return 1;
    }
    
    printf("\nFirst matrix:\n");
    print_matrix(&mat1);
    
    printf("\nSecond matrix:\n");
    print_matrix(&mat2);
    
    printf("\nMatrix multiplication:\n");
    if (multiply_matrices(&mat1, &mat2, &result)) {
        print_matrix(&result);
    } else {
        printf("Cannot multiply matrices - dimension mismatch or overflow\n");
    }
    
    printf("\nTranspose of first matrix:\n");
    if (transpose_matrix(&mat1, &result)) {
        print_matrix(&result);
    } else {
        printf("Cannot transpose matrix\n");
    }
    
    printf("\nTranspose of second matrix:\n");
    if (transpose_matrix(&mat2, &result)) {
        print_matrix(&result);
    } else {
        printf("Cannot transpose matrix\n");
    }
    
    return 0;
}