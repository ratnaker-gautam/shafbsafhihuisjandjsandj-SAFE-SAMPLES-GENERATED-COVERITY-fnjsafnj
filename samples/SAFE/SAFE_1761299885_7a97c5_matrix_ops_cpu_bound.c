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
            m->data[i][j] = (double)((i + j) % 10);
        }
    }
}

void print_matrix(const Matrix *m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%6.2f ", m->data[i][j]);
        }
        printf("\n");
    }
}

int matrix_multiply(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a->cols != b->rows) {
        return 0;
    }
    if (!validate_matrix_size(a->rows, b->cols)) {
        return 0;
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
    return 1;
}

double matrix_trace(const Matrix *m) {
    if (m->rows != m->cols) {
        return 0.0;
    }
    double trace = 0.0;
    for (int i = 0; i < m->rows; i++) {
        trace += m->data[i][i];
    }
    return trace;
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

int main() {
    srand(42);
    
    int size;
    printf("Enter matrix size (2-10): ");
    if (scanf("%d", &size) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (size < 2 || size > 10) {
        printf("Size must be between 2 and 10\n");
        return 1;
    }
    
    Matrix a, b, result;
    
    initialize_matrix(&a, size, size);
    initialize_matrix(&b, size, size);
    
    printf("Matrix A:\n");
    print_matrix(&a);
    printf("\nMatrix B:\n");
    print_matrix(&b);
    
    if (matrix_multiply(&a, &b, &result)) {
        printf("\nMatrix A * B:\n");
        print_matrix(&result);
        
        double trace = matrix_trace(&result);
        printf("\nTrace of result: %.2f\n", trace);
    } else {
        printf("Matrix multiplication failed\n");
    }
    
    Matrix transposed;
    matrix_transpose(&a, &transposed);
    printf("\nTranspose of A:\n");
    print_matrix(&transposed);
    
    clock_t start = clock();
    
    for (int iter = 0; iter < 100000; iter++) {
        Matrix temp1, temp2;
        initialize_matrix(&temp1, 3, 3);
        initialize_matrix(&temp2, 3, 3);
        matrix_multiply(&temp1, &temp2, &result);
        matrix_transpose(&temp1, &temp2);
        matrix_trace(&temp1);
    }
    
    clock_t end = clock();
    double cpu_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("\nCPU-intensive operations completed in %.3f seconds\n", cpu_time);
    
    return 0;
}