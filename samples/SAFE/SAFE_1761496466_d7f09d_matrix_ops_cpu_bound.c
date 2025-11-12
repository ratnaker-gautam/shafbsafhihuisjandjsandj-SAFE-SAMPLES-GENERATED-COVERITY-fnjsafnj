//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 100

typedef struct {
    int rows;
    int cols;
    double data[MAX_DIM][MAX_DIM];
} Matrix;

int validate_matrix_dimensions(int rows, int cols) {
    if (rows <= 0 || cols <= 0) return 0;
    if (rows > MAX_DIM || cols > MAX_DIM) return 0;
    return 1;
}

int read_matrix(Matrix *mat) {
    printf("Enter matrix dimensions (rows cols): ");
    if (scanf("%d %d", &mat->rows, &mat->cols) != 2) return 0;
    if (!validate_matrix_dimensions(mat->rows, mat->cols)) return 0;
    
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            if (scanf("%lf", &mat->data[i][j]) != 1) return 0;
        }
    }
    return 1;
}

void print_matrix(const Matrix *mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%8.2f ", mat->data[i][j]);
        }
        printf("\n");
    }
}

int matrix_multiply(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a->cols != b->rows) return 0;
    if (!validate_matrix_dimensions(a->rows, b->cols)) return 0;
    
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
    return 1;
}

int matrix_transpose(const Matrix *input, Matrix *result) {
    if (!validate_matrix_dimensions(input->cols, input->rows)) return 0;
    
    result->rows = input->cols;
    result->cols = input->rows;
    
    for (int i = 0; i < input->rows; i++) {
        for (int j = 0; j < input->cols; j++) {
            result->data[j][i] = input->data[i][j];
        }
    }
    return 1;
}

double matrix_trace(const Matrix *mat) {
    if (mat->rows != mat->cols) return 0.0;
    double trace = 0.0;
    for (int i = 0; i < mat->rows; i++) {
        trace += mat->data[i][i];
    }
    return trace;
}

int main() {
    Matrix a, b, result;
    
    printf("Matrix Operations Program\n");
    printf("Maximum dimension: %d\n\n", MAX_DIM);
    
    printf("Matrix A:\n");
    if (!read_matrix(&a)) {
        printf("Error: Invalid matrix A input\n");
        return 1;
    }
    
    printf("Matrix B:\n");
    if (!read_matrix(&b)) {
        printf("Error: Invalid matrix B input\n");
        return 1;
    }
    
    printf("\nMatrix A:\n");
    print_matrix(&a);
    
    printf("\nMatrix B:\n");
    print_matrix(&b);
    
    printf("\nMatrix Multiplication A * B:\n");
    if (matrix_multiply(&a, &b, &result)) {
        print_matrix(&result);
    } else {
        printf("Error: Cannot multiply matrices\n");
    }
    
    printf("\nMatrix Transpose of A:\n");
    if (matrix_transpose(&a, &result)) {
        print_matrix(&result);
    } else {
        printf("Error: Cannot transpose matrix\n");
    }
    
    printf("\nTrace of Matrix A: ");
    double trace = matrix_trace(&a);
    if (trace != 0.0 || a.rows == a.cols) {
        printf("%.2f\n", trace);
    } else {
        printf("Not a square matrix\n");
    }
    
    printf("\nTrace of Matrix B: ");
    trace = matrix_trace(&b);
    if (trace != 0.0 || b.rows == b.cols) {
        printf("%.2f\n", trace);
    } else {
        printf("Not a square matrix\n");
    }
    
    return 0;
}