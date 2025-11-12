//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: heap_stack_mix ; Variation: numeric_computation
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
    if (mat == NULL) return NULL;
    
    mat->rows = rows;
    mat->cols = cols;
    mat->data = malloc(rows * cols * sizeof(double));
    if (mat->data == NULL) {
        free(mat);
        return NULL;
    }
    
    return mat;
}

void destroy_matrix(struct Matrix *mat) {
    if (mat != NULL) {
        free(mat->data);
        free(mat);
    }
}

int initialize_matrix(struct Matrix *mat) {
    if (mat == NULL || mat->data == NULL) return 0;
    
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            int idx = i * mat->cols + j;
            mat->data[idx] = sin((double)(i + j)) * cos((double)(i - j));
        }
    }
    return 1;
}

double compute_matrix_norm(const struct Matrix *mat) {
    if (mat == NULL || mat->data == NULL) return 0.0;
    
    double sum = 0.0;
    for (int i = 0; i < mat->rows * mat->cols; i++) {
        sum += mat->data[i] * mat->data[i];
    }
    return sqrt(sum);
}

int multiply_matrices(const struct Matrix *a, const struct Matrix *b, struct Matrix *result) {
    if (a == NULL || b == NULL || result == NULL) return 0;
    if (a->cols != b->rows) return 0;
    if (result->rows != a->rows || result->cols != b->cols) return 0;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            double sum = 0.0;
            for (int k = 0; k < a->cols; k++) {
                int idx_a = i * a->cols + k;
                int idx_b = k * b->cols + j;
                sum += a->data[idx_a] * b->data[idx_b];
            }
            int idx_result = i * result->cols + j;
            result->data[idx_result] = sum;
        }
    }
    return 1;
}

int main() {
    int rows, cols;
    
    printf("Enter matrix dimensions (rows cols, max %d): ", MAX_SIZE);
    if (scanf("%d %d", &rows, &cols) != 2) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (!validate_matrix_params(rows, cols)) {
        printf("Invalid matrix dimensions\n");
        return 1;
    }
    
    struct Matrix *mat1 = create_matrix(rows, cols);
    struct Matrix *mat2 = create_matrix(cols, rows);
    struct Matrix *result = create_matrix(rows, rows);
    
    if (mat1 == NULL || mat2 == NULL || result == NULL) {
        printf("Memory allocation failed\n");
        destroy_matrix(mat1);
        destroy_matrix(mat2);
        destroy_matrix(result);
        return 1;
    }
    
    if (!initialize_matrix(mat1) || !initialize_matrix(mat2)) {
        printf("Matrix initialization failed\n");
        destroy_matrix(mat1);
        destroy_matrix(mat2);
        destroy_matrix(result);
        return 1;
    }
    
    double norm1 = compute_matrix_norm(mat1);
    double norm2 = compute_matrix_norm(mat2);
    
    printf("Matrix 1 norm: %.6f\n", norm1);
    printf("Matrix 2 norm: %.6f\n", norm2);
    
    if (multiply_matrices(mat1, mat2, result)) {
        double result_norm = compute_matrix_norm(result);
        printf("Result matrix norm: %.6f\n", result_norm);
        
        double stack_array[MAX_SIZE];
        int size = (rows * rows < MAX_SIZE) ? rows * rows : MAX_SIZE;
        for (int i = 0; i < size; i++) {
            stack_array[i] = result->data[i] * 0.5;
        }
        
        double stack_sum = 0.0;
        for (int i = 0; i < size; i++) {
            stack_sum += stack_array[i];
        }
        printf("Stack processed sum: %.6f\n", stack_sum);
    } else {
        printf("Matrix multiplication failed\n");
    }
    
    destroy_matrix(mat1);
    destroy_matrix(mat2);
    destroy_matrix(result);
    
    return 0;
}