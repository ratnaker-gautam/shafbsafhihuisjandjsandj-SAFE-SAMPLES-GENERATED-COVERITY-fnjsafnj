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

void matrix_multiply(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->cols != b->rows) {
        fprintf(stderr, "Matrix dimensions incompatible for multiplication\n");
        exit(1);
    }
    
    result->rows = a->rows;
    result->cols = b->cols;
    
    for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->cols; j++) {
            result->data[i][j] = 0.0;
            for (int k = 0; k < a->cols; k++) {
                result->data[i][j] += a->data[i][k] * b->data[k][j];
            }
        }
    }
}

void matrix_transpose(const Matrix* input, Matrix* result) {
    result->rows = input->cols;
    result->cols = input->rows;
    
    for (int i = 0; i < input->rows; i++) {
        for (int j = 0; j < input->cols; j++) {
            result->data[j][i] = input->data[i][j];
        }
    }
}

double matrix_trace(const Matrix* m) {
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

void initialize_matrix(Matrix* m, int rows, int cols) {
    if (rows <= 0 || rows > MAX_SIZE || cols <= 0 || cols > MAX_SIZE) {
        fprintf(stderr, "Invalid matrix dimensions\n");
        exit(1);
    }
    
    m->rows = rows;
    m->cols = cols;
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            m->data[i][j] = (double)((i + 1) * (j + 1)) / 10.0;
        }
    }
}

void print_matrix(const Matrix* m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%8.2f", m->data[i][j]);
        }
        printf("\n");
    }
}

int main() {
    Matrix a, b, product, transpose;
    
    initialize_matrix(&a, 4, 3);
    initialize_matrix(&b, 3, 4);
    
    printf("Matrix A (%dx%d):\n", a.rows, a.cols);
    print_matrix(&a);
    
    printf("\nMatrix B (%dx%d):\n", b.rows, b.cols);
    print_matrix(&b);
    
    matrix_multiply(&a, &b, &product);
    printf("\nMatrix A * B (%dx%d):\n", product.rows, product.cols);
    print_matrix(&product);
    
    matrix_transpose(&product, &transpose);
    printf("\nTranspose of A*B (%dx%d):\n", transpose.rows, transpose.cols);
    print_matrix(&transpose);
    
    Matrix square;
    initialize_matrix(&square, 3, 3);
    printf("\nSquare matrix for trace (%dx%d):\n", square.rows, square.cols);
    print_matrix(&square);
    
    double trace_val = matrix_trace(&square);
    printf("\nTrace of square matrix: %.2f\n", trace_val);
    
    clock_t start = clock();
    
    for (int iter = 0; iter < 100000; iter++) {
        Matrix temp1, temp2, temp3;
        initialize_matrix(&temp1, 3, 3);
        initialize_matrix(&temp2, 3, 3);
        matrix_multiply(&temp1, &temp2, &temp3);
        matrix_trace(&temp3);
    }
    
    clock_t end = clock();
    double cpu_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    printf("CPU-intensive operations completed in %.2f seconds\n", cpu_time);
    
    return 0;
}