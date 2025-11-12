//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_SIZE 100

typedef struct {
    int rows;
    int cols;
    double data[MAX_SIZE][MAX_SIZE];
} Matrix;

int validate_matrix_size(int rows, int cols) {
    return (rows > 0 && rows <= MAX_SIZE && cols > 0 && cols <= MAX_SIZE);
}

void initialize_matrix(Matrix *mat, int rows, int cols) {
    if (!validate_matrix_size(rows, cols)) {
        fprintf(stderr, "Invalid matrix dimensions\n");
        exit(1);
    }
    mat->rows = rows;
    mat->cols = cols;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            mat->data[i][j] = (double)((i + j) % 10);
        }
    }
}

void matrix_multiply(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a->cols != b->rows) {
        fprintf(stderr, "Matrix dimensions incompatible for multiplication\n");
        exit(1);
    }
    if (!validate_matrix_size(a->rows, b->cols)) {
        fprintf(stderr, "Result matrix would exceed maximum size\n");
        exit(1);
    }
    
    result->rows = a->rows;
    result->cols = b->cols;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            double sum = 0.0;
            for (int k = 0; k < a->cols; k++) {
                sum += a->data[i][k] * b->data[k][j];
            }
            result->data[i][j] = sum;
        }
    }
}

void matrix_transpose(const Matrix *input, Matrix *result) {
    if (!validate_matrix_size(input->cols, input->rows)) {
        fprintf(stderr, "Transposed matrix would exceed maximum size\n");
        exit(1);
    }
    
    result->rows = input->cols;
    result->cols = input->rows;
    
    for (int i = 0; i < input->rows; i++) {
        for (int j = 0; j < input->cols; j++) {
            result->data[j][i] = input->data[i][j];
        }
    }
}

double matrix_trace(const Matrix *mat) {
    if (mat->rows != mat->cols) {
        fprintf(stderr, "Matrix must be square for trace calculation\n");
        exit(1);
    }
    
    double trace = 0.0;
    for (int i = 0; i < mat->rows; i++) {
        trace += mat->data[i][i];
    }
    return trace;
}

void print_matrix(const Matrix *mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%8.2f", mat->data[i][j]);
        }
        printf("\n");
    }
}

int main() {
    Matrix a, b, result;
    int size;
    
    printf("Enter matrix size (2-10): ");
    if (scanf("%d", &size) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    if (size < 2 || size > 10) {
        fprintf(stderr, "Size must be between 2 and 10\n");
        return 1;
    }
    
    initialize_matrix(&a, size, size);
    initialize_matrix(&b, size, size);
    
    printf("\nMatrix A:\n");
    print_matrix(&a);
    
    printf("\nMatrix B:\n");
    print_matrix(&b);
    
    printf("\nMatrix multiplication A * B:\n");
    matrix_multiply(&a, &b, &result);
    print_matrix(&result);
    
    printf("\nTranspose of A:\n");
    matrix_transpose(&a, &result);
    print_matrix(&result);
    
    printf("\nTrace of A: %.2f\n", matrix_trace(&a));
    printf("Trace of B: %.2f\n", matrix_trace(&b));
    
    return 0;
}