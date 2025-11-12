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
    
    int size1 = 4 + rand() % 3;
    int size2 = 4 + rand() % 3;
    int size3 = size2;
    
    if (!validate_matrix_size(size1, size2) || !validate_matrix_size(size2, size3)) {
        fprintf(stderr, "Error: Invalid matrix dimensions\n");
        return 1;
    }
    
    Matrix a, b, c, result;
    
    printf("Matrix A (%dx%d):\n", size1, size2);
    initialize_matrix(&a, size1, size2);
    print_matrix(&a);
    
    printf("\nMatrix B (%dx%d):\n", size2, size3);
    initialize_matrix(&b, size2, size3);
    print_matrix(&b);
    
    printf("\nMatrix multiplication A * B:\n");
    matrix_multiply(&a, &b, &result);
    print_matrix(&result);
    
    printf("\nTranspose of result matrix:\n");
    Matrix transposed;
    matrix_transpose(&result, &transposed);
    print_matrix(&transposed);
    
    if (transposed.rows == transposed.cols) {
        double trace = matrix_trace(&transposed);
        printf("\nTrace of transposed matrix: %.2f\n", trace);
    }
    
    printf("\nPerforming additional CPU-intensive operations...\n");
    
    Matrix temp1, temp2;
    for (int iter = 0; iter < 5; iter++) {
        matrix_multiply(&a, &b, &temp1);
        matrix_transpose(&temp1, &temp2);
        if (temp2.rows == temp2.cols) {
            double t = matrix_trace(&temp2);
        }
    }
    
    printf("Matrix operations completed successfully.\n");
    
    return 0;
}