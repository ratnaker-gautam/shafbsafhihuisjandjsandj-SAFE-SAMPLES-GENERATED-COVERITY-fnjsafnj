//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_SIZE 100

struct Matrix {
    double *data;
    int rows;
    int cols;
};

int validate_matrix_params(int rows, int cols) {
    if (rows <= 0 || cols <= 0) return 0;
    if (rows > MAX_SIZE || cols > MAX_SIZE) return 0;
    return 1;
}

struct Matrix* create_matrix(int rows, int cols) {
    if (!validate_matrix_params(rows, cols)) return NULL;
    
    struct Matrix *mat = malloc(sizeof(struct Matrix));
    if (!mat) return NULL;
    
    mat->rows = rows;
    mat->cols = cols;
    mat->data = malloc(rows * cols * sizeof(double));
    if (!mat->data) {
        free(mat);
        return NULL;
    }
    
    return mat;
}

void destroy_matrix(struct Matrix *mat) {
    if (mat) {
        free(mat->data);
        free(mat);
    }
}

int fill_matrix_identity(struct Matrix *mat) {
    if (!mat || !mat->data) return 0;
    
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            if (i == j) {
                mat->data[i * mat->cols + j] = 1.0;
            } else {
                mat->data[i * mat->cols + j] = 0.0;
            }
        }
    }
    return 1;
}

int matrix_multiply(const struct Matrix *a, const struct Matrix *b, struct Matrix *result) {
    if (!a || !b || !result) return 0;
    if (a->cols != b->rows) return 0;
    if (result->rows != a->rows || result->cols != b->cols) return 0;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            double sum = 0.0;
            for (int k = 0; k < a->cols; k++) {
                sum += a->data[i * a->cols + k] * b->data[k * b->cols + j];
            }
            result->data[i * result->cols + j] = sum;
        }
    }
    return 1;
}

double compute_matrix_trace(const struct Matrix *mat) {
    if (!mat || !mat->data) return 0.0;
    if (mat->rows != mat->cols) return 0.0;
    
    double trace = 0.0;
    for (int i = 0; i < mat->rows; i++) {
        trace += mat->data[i * mat->cols + i];
    }
    return trace;
}

int main() {
    int size;
    printf("Enter matrix size (1-%d): ", MAX_SIZE);
    if (scanf("%d", &size) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (size < 1 || size > MAX_SIZE) {
        printf("Size out of range\n");
        return 1;
    }
    
    struct Matrix *a = create_matrix(size, size);
    struct Matrix *b = create_matrix(size, size);
    struct Matrix *result = create_matrix(size, size);
    
    if (!a || !b || !result) {
        printf("Memory allocation failed\n");
        if (a) destroy_matrix(a);
        if (b) destroy_matrix(b);
        if (result) destroy_matrix(result);
        return 1;
    }
    
    if (!fill_matrix_identity(a) || !fill_matrix_identity(b)) {
        printf("Matrix initialization failed\n");
        destroy_matrix(a);
        destroy_matrix(b);
        destroy_matrix(result);
        return 1;
    }
    
    if (!matrix_multiply(a, b, result)) {
        printf("Matrix multiplication failed\n");
        destroy_matrix(a);
        destroy_matrix(b);
        destroy_matrix(result);
        return 1;
    }
    
    double trace = compute_matrix_trace(result);
    printf("Trace of identity matrix product: %.2f\n", trace);
    
    destroy_matrix(a);
    destroy_matrix(b);
    destroy_matrix(result);
    
    return 0;
}