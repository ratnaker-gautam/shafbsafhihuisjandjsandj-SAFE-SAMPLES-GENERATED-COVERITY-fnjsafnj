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

int fill_matrix(struct Matrix *mat) {
    if (!mat || !mat->data) return 0;
    
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            double value = sin(i + j) * cos(i - j);
            if (isnan(value) || isinf(value)) value = 0.0;
            mat->data[i * mat->cols + j] = value;
        }
    }
    return 1;
}

double compute_matrix_trace(struct Matrix *mat) {
    if (!mat || !mat->data) return 0.0;
    if (mat->rows != mat->cols) return 0.0;
    
    double trace = 0.0;
    for (int i = 0; i < mat->rows; i++) {
        trace += mat->data[i * mat->cols + i];
    }
    return trace;
}

double compute_determinant_2x2(struct Matrix *mat) {
    if (!mat || !mat->data) return 0.0;
    if (mat->rows != 2 || mat->cols != 2) return 0.0;
    
    double a = mat->data[0];
    double b = mat->data[1];
    double c = mat->data[2];
    double d = mat->data[3];
    
    return a * d - b * c;
}

int main() {
    int size;
    printf("Enter matrix size (1-10): ");
    if (scanf("%d", &size) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (size < 1 || size > 10) {
        printf("Size must be between 1 and 10\n");
        return 1;
    }
    
    struct Matrix *matrix = create_matrix(size, size);
    if (!matrix) {
        printf("Failed to create matrix\n");
        return 1;
    }
    
    if (!fill_matrix(matrix)) {
        printf("Failed to fill matrix\n");
        destroy_matrix(matrix);
        return 1;
    }
    
    printf("Matrix created and filled with computed values\n");
    
    double trace = compute_matrix_trace(matrix);
    printf("Matrix trace: %.6f\n", trace);
    
    if (size == 2) {
        double det = compute_determinant_2x2(matrix);
        printf("2x2 determinant: %.6f\n", det);
    }
    
    double stack_array[MAX_SIZE];
    int valid_count = 0;
    for (int i = 0; i < size && valid_count < MAX_SIZE; i++) {
        for (int j = 0; j < size && valid_count < MAX_SIZE; j++) {
            double val = matrix->data[i * size + j];
            if (!isnan(val) && !isinf(val)) {
                stack_array[valid_count] = val;
                valid_count++;
            }
        }
    }
    
    double sum = 0.0;
    for (int i = 0; i < valid_count; i++) {
        sum += stack_array[i];
    }
    
    printf("Sum of %d valid matrix elements: %.6f\n", valid_count, sum);
    
    destroy_matrix(matrix);
    return 0;
}