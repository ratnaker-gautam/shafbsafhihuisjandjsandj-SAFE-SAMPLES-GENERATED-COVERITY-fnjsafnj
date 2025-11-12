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
        printf("Matrix dimensions incompatible for multiplication\n");
        return;
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

void matrix_transpose(const Matrix *m, Matrix *result) {
    result->rows = m->cols;
    result->cols = m->rows;
    
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            result->data[j][i] = m->data[i][j];
        }
    }
}

double matrix_trace(const Matrix *m) {
    if (m->rows != m->cols) {
        printf("Matrix must be square for trace calculation\n");
        return 0.0;
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
    srand(42);
    
    int size = 4;
    if (!validate_matrix_size(size, size)) {
        printf("Invalid matrix size\n");
        return 1;
    }
    
    Matrix a, b, result;
    
    printf("Matrix A:\n");
    initialize_matrix(&a, size, size);
    print_matrix(&a);
    
    printf("\nMatrix B:\n");
    initialize_matrix(&b, size, size);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            b.data[i][j] = (double)((i + 1) * (j + 1));
        }
    }
    print_matrix(&b);
    
    printf("\nMatrix A * B:\n");
    matrix_multiply(&a, &b, &result);
    print_matrix(&result);
    
    printf("\nTrace of A * B: %.2f\n", matrix_trace(&result));
    
    printf("\nTranspose of A:\n");
    matrix_transpose(&a, &result);
    print_matrix(&result);
    
    printf("\nTrace of A transpose: %.2f\n", matrix_trace(&result));
    
    return 0;
}