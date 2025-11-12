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

int initialize_matrix(struct Matrix *mat) {
    if (!mat || !mat->data) return 0;
    
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            mat->data[i * mat->cols + j] = (i + 1) * (j + 1) * 0.1;
        }
    }
    return 1;
}

double compute_determinant_2x2(struct Matrix *mat) {
    if (!mat || mat->rows != 2 || mat->cols != 2) return 0.0;
    return mat->data[0] * mat->data[3] - mat->data[1] * mat->data[2];
}

double compute_matrix_norm(struct Matrix *mat) {
    if (!mat || !mat->data) return 0.0;
    
    double sum = 0.0;
    for (int i = 0; i < mat->rows * mat->cols; i++) {
        sum += mat->data[i] * mat->data[i];
    }
    return sqrt(sum);
}

int main() {
    int rows, cols;
    
    printf("Enter matrix rows (1-%d): ", MAX_SIZE);
    if (scanf("%d", &rows) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    printf("Enter matrix cols (1-%d): ", MAX_SIZE);
    if (scanf("%d", &cols) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (!validate_matrix_params(rows, cols)) {
        printf("Invalid matrix dimensions\n");
        return 1;
    }
    
    struct Matrix *mat = create_matrix(rows, cols);
    if (!mat) {
        printf("Memory allocation failed\n");
        return 1;
    }
    
    if (!initialize_matrix(mat)) {
        printf("Matrix initialization failed\n");
        destroy_matrix(mat);
        return 1;
    }
    
    printf("Matrix created and initialized successfully\n");
    
    double small_matrix[2][2] = {{1.5, 2.3}, {0.8, 1.7}};
    struct Matrix stack_mat;
    stack_mat.rows = 2;
    stack_mat.cols = 2;
    stack_mat.data = (double*)small_matrix;
    
    double det = compute_determinant_2x2(&stack_mat);
    printf("2x2 matrix determinant: %.2f\n", det);
    
    double norm = compute_matrix_norm(mat);
    printf("Matrix Frobenius norm: %.2f\n", norm);
    
    double local_array[MAX_SIZE];
    int valid_size = (rows * cols < MAX_SIZE) ? rows * cols : MAX_SIZE;
    for (int i = 0; i < valid_size; i++) {
        local_array[i] = mat->data[i] * 2.0;
    }
    
    double local_sum = 0.0;
    for (int i = 0; i < valid_size; i++) {
        local_sum += local_array[i];
    }
    printf("Local array sum: %.2f\n", local_sum);
    
    destroy_matrix(mat);
    return 0;
}