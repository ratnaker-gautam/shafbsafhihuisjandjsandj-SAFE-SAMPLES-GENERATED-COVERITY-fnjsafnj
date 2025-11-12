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

void initialize_matrix(Matrix *m, int rows, int cols) {
    m->rows = rows;
    m->cols = cols;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            m->data[i][j] = (double)(i * cols + j + 1);
        }
    }
}

void matrix_multiply(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a->cols != b->rows) {
        fprintf(stderr, "Matrix dimensions incompatible for multiplication\n");
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
    result->rows = input->cols;
    result->cols = input->rows;
    
    for (int i = 0; i < input->rows; i++) {
        for (int j = 0; j < input->cols; j++) {
            result->data[j][i] = input->data[i][j];
        }
    }
}

double matrix_trace(const Matrix *m) {
    if (m->rows != m->cols) {
        fprintf(stderr, "Matrix must be square for trace calculation\n");
        exit(1);
    }
    
    double trace = 0.0;
    for (int i = 0; i < m->rows; i++) {
        trace += m->data[i][i];
    }
    return trace;
}

void print_matrix(const Matrix *m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%8.2f", m->data[i][j]);
        }
        printf("\n");
    }
}

int main() {
    int rows1, cols1, rows2, cols2;
    
    printf("Enter dimensions for first matrix (rows cols): ");
    if (scanf("%d %d", &rows1, &cols1) != 2) {
        fprintf(stderr, "Invalid input for matrix dimensions\n");
        return 1;
    }
    
    if (!validate_matrix_size(rows1, cols1)) {
        fprintf(stderr, "Matrix dimensions must be between 1 and %d\n", MAX_SIZE);
        return 1;
    }
    
    printf("Enter dimensions for second matrix (rows cols): ");
    if (scanf("%d %d", &rows2, &cols2) != 2) {
        fprintf(stderr, "Invalid input for matrix dimensions\n");
        return 1;
    }
    
    if (!validate_matrix_size(rows2, cols2)) {
        fprintf(stderr, "Matrix dimensions must be between 1 and %d\n", MAX_SIZE);
        return 1;
    }
    
    Matrix mat1, mat2, result;
    
    initialize_matrix(&mat1, rows1, cols1);
    initialize_matrix(&mat2, rows2, cols2);
    
    printf("\nMatrix 1:\n");
    print_matrix(&mat1);
    
    printf("\nMatrix 2:\n");
    print_matrix(&mat2);
    
    if (mat1.cols == mat2.rows) {
        matrix_multiply(&mat1, &mat2, &result);
        printf("\nMatrix multiplication result:\n");
        print_matrix(&result);
    } else {
        printf("\nMatrices cannot be multiplied (incompatible dimensions)\n");
    }
    
    printf("\nTranspose of matrix 1:\n");
    matrix_transpose(&mat1, &result);
    print_matrix(&result);
    
    if (mat1.rows == mat1.cols) {
        double trace = matrix_trace(&mat1);
        printf("\nTrace of matrix 1: %.2f\n", trace);
    } else {
        printf("\nMatrix 1 is not square, cannot compute trace\n");
    }
    
    if (mat2.rows == mat2.cols) {
        double trace = matrix_trace(&mat2);
        printf("Trace of matrix 2: %.2f\n", trace);
    } else {
        printf("Matrix 2 is not square, cannot compute trace\n");
    }
    
    return 0;
}