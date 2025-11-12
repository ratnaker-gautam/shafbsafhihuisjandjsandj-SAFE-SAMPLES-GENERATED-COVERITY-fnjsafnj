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

int add_matrices(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a == NULL || b == NULL || result == NULL) return 0;
    if (a->rows != b->rows || a->cols != b->cols) return 0;
    
    result->rows = a->rows;
    result->cols = a->cols;
    
    for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->cols; j++) {
            int64_t sum = (int64_t)a->data[i][j] + (int64_t)b->data[i][j];
            if (sum > INT_MAX || sum < INT_MIN) return 0;
            result->data[i][j] = (int)sum;
        }
    }
    return 1;
}

int transpose_matrix(const Matrix *input, Matrix *result) {
    if (input == NULL || result == NULL) return 0;
    
    result->rows = input->cols;
    result->cols = input->rows;
    
    for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->cols; j++) {
            result->data[i][j] = input->data[j][i];
        }
    }
    return 1;
}

void clear_input_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main(void) {
    Matrix mat1, mat2, result;
    int choice;
    
    printf("Matrix Operations Program\n");
    printf("1. Matrix Multiplication\n");
    printf("2. Matrix Addition\n");
    printf("3. Matrix Transpose\n");
    printf("Enter choice (1-3): ");
    
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    clear_input_buffer();
    
    switch (choice) {
        case 1:
            printf("Enter first matrix:\n");
            if (!read_matrix(&mat1)) {
                printf("Invalid matrix input\n");
                return 1;
            }
            printf("Enter second matrix:\n");
            if (!read_matrix(&mat2)) {
                printf("Invalid matrix input\n");
                return 1;
            }
            if (!multiply_matrices(&mat1, &mat2, &result)) {
                printf("Matrix multiplication failed\n");
                return 1;
            }
            printf("Result of multiplication:\n");
            print_matrix(&result);
            break;
            
        case 2:
            printf("Enter first matrix:\n");
            if (!read_matrix(&mat1)) {
                printf("Invalid matrix input\n");
                return 1;
            }
            printf("Enter second