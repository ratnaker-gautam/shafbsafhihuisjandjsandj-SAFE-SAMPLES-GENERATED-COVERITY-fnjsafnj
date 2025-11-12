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

void matrix_transpose(const Matrix *src, Matrix *dest) {
    dest->rows = src->cols;
    dest->cols = src->rows;
    
    for (int i = 0; i < src->rows; i++) {
        for (int j = 0; j < src->cols; j++) {
            dest->data[j][i] = src->data[i][j];
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
    srand(42);
    
    int size;
    printf("Enter matrix size (1-%d): ", MAX_SIZE);
    if (scanf("%d", &size) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        return 1;
    }
    
    if (!validate_matrix_size(size, size)) {
        fprintf(stderr, "Error: Invalid matrix size\n");
        return 1;
    }
    
    Matrix a, b, result;
    
    initialize_matrix(&a, size, size);
    initialize_matrix(&b, size, size);
    
    printf("\nMatrix A:\n");
    print_matrix(&a);
    
    printf("\nMatrix B:\n");
    print_matrix(&b);
    
    printf("\nMatrix multiplication result:\n");
    matrix_multiply(&a, &b, &result);
    print_matrix(&result);
    
    printf("\nTrace of result matrix: %.2f\n", matrix_trace(&result));
    
    Matrix transposed;
    matrix_transpose(&result, &transposed);
    printf("\nTranspose of result matrix:\n");
    print_matrix(&transposed);
    
    printf("\nTrace of transposed matrix: %.2f\n", matrix_trace(&transposed));
    
    return 0;
}