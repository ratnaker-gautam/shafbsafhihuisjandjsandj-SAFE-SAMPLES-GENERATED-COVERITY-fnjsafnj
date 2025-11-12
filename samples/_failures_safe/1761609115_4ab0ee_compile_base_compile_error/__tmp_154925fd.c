//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
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
    if (scanf("%d", &mat->rows) != 1) return 0;
    if (scanf("%d", &mat->cols) != 1) return 0;
    if (!validate_dimension(mat->rows) || !validate_dimension(mat->cols)) return 0;
    
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            if (scanf("%d", &mat->data[i][j]) != 1) return 0;
        }
    }
    return 1;
}

void print_matrix(const Matrix *mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%d ", mat->data[i][j]);
        }
        printf("\n");
    }
}

int multiply_matrices(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a->cols != b->rows) return 0;
    
    result->rows = a->rows;
    result->cols = b->cols;
    
    for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                if (a->data[i][k] > 0 && b->data[k][j] > 0) {
                    if (a->data[i][k] > INT_MAX / b->data[k][j]) return 0;
                } else if (a->data[i][k] < 0 && b->data[k][j] < 0) {
                    if (a->data[i][k] < INT_MAX / b->data[k][j]) return 0;
                } else if (a->data[i][k] != 0 && b->data[k][j] != 0) {
                    if (a->data[i][k] > INT_MAX / b->data[k][j] || 
                        a->data[i][k] < INT_MIN / b->data[k][j]) return 0;
                }
                int product = a->data[i][k] * b->data[k][j];
                if ((sum > 0 && product > 0 && sum > INT_MAX - product) ||
                    (sum < 0 && product < 0 && sum < INT_MIN - product) ||
                    (sum > 0 && product < 0 && sum < INT_MIN - product) ||
                    (sum < 0 && product > 0 && sum > INT_MAX - product)) {
                    return 0;
                }
                sum += product;
            }
            result->data[i][j] = sum;
        }
    }
    return 1;
}

int add_matrices(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a->rows != b->rows || a->cols != b->cols) return 0;
    
    result->rows = a->rows;
    result->cols = a->cols;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            if ((a->data[i][j] > 0 && b->data[i][j] > INT_MAX - a->data[i][j]) ||
                (a->data[i][j] < 0 && b->data[i][j] < INT_MIN - a->data[i][j])) {
                return 0;
            }
            result->data[i][j] = a->data[i][j] + b->data[i][j];
        }
    }
    return 1;
}

void transpose_matrix(const Matrix *input, Matrix *result) {
    result->rows = input->cols;
    result->cols = input->rows;
    
    for (int i = 0; i < input->rows; i++) {
        for (int j = 0; j < input->cols; j++) {
            result->data[j][i] = input->data[i][j];
        }
    }
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
    
    printf("\nFirst matrix:\n");
    print_matrix(&mat1);
    
    printf("\nSecond matrix:\n");
    print_matrix(&mat2);
    
    printf("\nMatrix addition: ");
    if (add_matrices(&mat1, &mat2, &result)) {
        printf("Success\n");
        print_matrix(&result);
    } else {
        printf("Failed - dimension mismatch or overflow\n");
    }
    
    printf("\nMatrix multiplication: ");
    if