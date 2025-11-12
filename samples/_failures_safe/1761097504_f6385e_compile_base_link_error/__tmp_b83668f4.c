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
    return rows > 0 && rows <= MAX_SIZE && cols > 0 && cols <= MAX_SIZE;
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

double compute_frobenius_norm(const struct Matrix *mat) {
    if (mat == NULL || mat->data == NULL) {
        return -1.0;
    }
    
    double sum = 0.0;
    for (int i = 0; i < mat->rows * mat->cols; i++) {
        sum += mat->data[i] * mat->data[i];
    }
    return sqrt(sum);
}

int read_matrix_dimensions(int *rows, int *cols) {
    printf("Enter matrix dimensions (rows cols, max %d each): ", MAX_SIZE);
    if (scanf("%d %d", rows, cols) != 2) {
        return 0;
    }
    return 1;
}

int main(void) {
    int rows, cols;
    
    if (!read_matrix_dimensions(&rows, &cols)) {
        printf("Invalid input format.\n");
        return 1;
    }
    
    if (!validate_matrix_size(rows, cols)) {
        printf("Invalid matrix dimensions.\n");
        return 1;
    }
    
    struct Matrix *mat = create_matrix(rows, cols);
    if (mat == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }
    
    if (!initialize_matrix(mat)) {
        printf("Matrix initialization failed.\n");
        destroy_matrix(mat);
        return 1;
    }
    
    double norm = compute_frobenius_norm(mat);
    if (norm < 0) {
        printf("Norm computation failed.\n");
        destroy_matrix(mat);
        return 1;
    }
    
    printf("Frobenius norm of the matrix: %.6f\n", norm);
    
    double stack_matrix[4] = {1.0, 2.0, 3.0, 4.0};
    double stack_norm = 0.0;
    for (int i = 0; i < 4; i++) {
        stack_norm += stack_matrix[i] * stack_matrix[i];
    }
    stack_norm = sqrt(stack_norm);
    printf("Stack matrix norm: %.6f\n", stack_norm);
    
    destroy_matrix(mat);
    return 0;
}