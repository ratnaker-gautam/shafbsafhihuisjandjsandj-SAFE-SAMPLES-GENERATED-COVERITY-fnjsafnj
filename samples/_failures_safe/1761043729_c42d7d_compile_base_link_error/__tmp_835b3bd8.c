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

int validate_matrix_size(int rows, int cols) {
    return (rows > 0 && rows <= MAX_SIZE && cols > 0 && cols <= MAX_SIZE);
}

struct Matrix* create_matrix(int rows, int cols) {
    if (!validate_matrix_size(rows, cols)) {
        return NULL;
    }
    
    struct Matrix *mat = malloc(sizeof(struct Matrix));
    if (mat == NULL) {
        return NULL;
    }
    
    mat->data = malloc(rows * cols * sizeof(double));
    if (mat->data == NULL) {
        free(mat);
        return NULL;
    }
    
    mat->rows = rows;
    mat->cols = cols;
    return mat;
}

void destroy_matrix(struct Matrix *mat) {
    if (mat != NULL) {
        free(mat->data);
        free(mat);
    }
}

int initialize_matrix(struct Matrix *mat) {
    if (mat == NULL || mat->data == NULL) {
        return 0;
    }
    
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            mat->data[i * mat->cols + j] = (i + 1.0) * (j + 1.0) / (mat->rows * mat->cols);
        }
    }
    return 1;
}

double matrix_determinant_2x2(struct Matrix *mat) {
    if (mat == NULL || mat->rows != 2 || mat->cols != 2) {
        return 0.0;
    }
    
    return mat->data[0] * mat->data[3] - mat->data[1] * mat->data[2];
}

double compute_matrix_norm(struct Matrix *mat) {
    if (mat == NULL || mat->data == NULL) {
        return 0.0;
    }
    
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
    
    printf("Enter matrix columns (1-%d): ", MAX_SIZE);
    if (scanf("%d", &cols) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (!validate_matrix_size(rows, cols)) {
        printf("Invalid matrix size\n");
        return 1;
    }
    
    struct Matrix *mat = create_matrix(rows, cols);
    if (mat == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }
    
    if (!initialize_matrix(mat)) {
        printf("Matrix initialization failed\n");
        destroy_matrix(mat);
        return 1;
    }
    
    printf("Matrix created and initialized successfully\n");
    
    double matrix_data[4] = {1.0, 2.0, 3.0, 4.0};
    struct Matrix stack_mat = {matrix_data, 2, 2};
    
    printf("Stack matrix determinant: %.2f\n", matrix_determinant_2x2(&stack_mat));
    
    double norm = compute_matrix_norm(mat);
    printf("Heap matrix Frobenius norm: %.6f\n", norm);
    
    double local_array[MAX_SIZE];
    for (int i = 0; i < rows && i < MAX_SIZE; i++) {
        local_array[i] = mat->data[i * cols];
        printf("First element of row %d: %.4f\n", i + 1, local_array[i]);
    }
    
    destroy_matrix(mat);
    return 0;
}