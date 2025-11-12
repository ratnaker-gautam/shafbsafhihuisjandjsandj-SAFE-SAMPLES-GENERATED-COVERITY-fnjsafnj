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
        fprintf(stderr, "Error: Matrix dimensions incompatible for multiplication\n");
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
        fprintf(stderr, "Error: Matrix must be square for trace calculation\n");
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
    srand(time(NULL));
    
    int size1 = 3 + (rand() % 4);
    int size2 = 3 + (rand() % 4);
    int size3 = size2;
    
    if (!validate_matrix_size(size1, size2) || !validate_matrix_size(size2, size3)) {
        fprintf(stderr, "Error: Invalid matrix size\n");
        return 1;
    }
    
    Matrix A, B, C, D;
    
    printf("Matrix A (%dx%d):\n", size1, size2);
    initialize_matrix(&A, size1, size2);
    print_matrix(&A);
    
    printf("\nMatrix B (%dx%d):\n", size2, size3);
    initialize_matrix(&B, size2, size3);
    print_matrix(&B);
    
    printf("\nMatrix multiplication A * B:\n");
    matrix_multiply(&A, &B, &C);
    print_matrix(&C);
    
    printf("\nTranspose of result:\n");
    matrix_transpose(&C, &D);
    print_matrix(&D);
    
    if (D.rows == D.cols) {
        printf("\nTrace of transposed matrix: %.2f\n", matrix_trace(&D));
    }
    
    return 0;
}