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

void matrix_init(Matrix *m, int rows, int cols) {
    if (rows <= 0 || cols <= 0 || rows > MAX_SIZE || cols > MAX_SIZE) {
        fprintf(stderr, "Invalid matrix dimensions\n");
        exit(1);
    }
    m->rows = rows;
    m->cols = cols;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            m->data[i][j] = 0.0;
        }
    }
}

void matrix_random_fill(Matrix *m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            m->data[i][j] = (double)(rand() % 1000) / 100.0;
        }
    }
}

void matrix_multiply(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a->cols != b->rows) {
        fprintf(stderr, "Matrix dimensions incompatible for multiplication\n");
        exit(1);
    }
    if (result->rows != a->rows || result->cols != b->cols) {
        fprintf(stderr, "Result matrix has wrong dimensions\n");
        exit(1);
    }
    
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
    if (result->rows != m->cols || result->cols != m->rows) {
        fprintf(stderr, "Result matrix has wrong dimensions for transpose\n");
        exit(1);
    }
    
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            result->data[j][i] = m->data[i][j];
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

void matrix_print(const Matrix *m) {
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
    printf("Enter matrix size (2-10): ");
    if (scanf("%d", &size) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    if (size < 2 || size > 10) {
        fprintf(stderr, "Size must be between 2 and 10\n");
        return 1;
    }
    
    Matrix a, b, result;
    
    matrix_init(&a, size, size);
    matrix_init(&b, size, size);
    matrix_init(&result, size, size);
    
    matrix_random_fill(&a);
    matrix_random_fill(&b);
    
    printf("Matrix A:\n");
    matrix_print(&a);
    printf("\nMatrix B:\n");
    matrix_print(&b);
    
    matrix_multiply(&a, &b, &result);
    printf("\nMatrix A * B:\n");
    matrix_print(&result);
    
    Matrix transposed;
    matrix_init(&transposed, size, size);
    matrix_transpose(&a, &transposed);
    printf("\nTranspose of A:\n");
    matrix_print(&transposed);
    
    double trace_a = matrix_trace(&a);
    double trace_b = matrix_trace(&b);
    printf("\nTrace of A: %.2f\n", trace_a);
    printf("Trace of B: %.2f\n", trace_b);
    
    return 0;
}