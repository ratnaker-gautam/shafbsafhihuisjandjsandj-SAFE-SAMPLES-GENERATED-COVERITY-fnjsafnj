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
    
    for (int i = 0; i < mat->rows * mat->cols; i++) {
        mat->data[i] = (double)(i % 10) + 1.0;
    }
    return 1;
}

double compute_determinant_2x2(struct Matrix *mat) {
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

int multiply_matrices(struct Matrix *a, struct Matrix *b, struct Matrix *result) {
    if (a == NULL || b == NULL || result == NULL) {
        return 0;
    }
    
    if (a->cols != b->rows || result->rows != a->rows || result->cols != b->cols) {
        return 0;
    }
    
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

int main() {
    int rows, cols;
    
    printf("Enter matrix dimensions (rows cols, max %d): ", MAX_SIZE);
    if (scanf("%d %d", &rows, &cols) != 2) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (!validate_matrix_size(rows, cols)) {
        printf("Invalid matrix dimensions\n");
        return 1;
    }
    
    struct Matrix *mat1 = create_matrix(rows, cols);
    if (mat1 == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }
    
    if (!initialize_matrix(mat1)) {
        printf("Matrix initialization failed\n");
        destroy_matrix(mat1);
        return 1;
    }
    
    double stack_array[MAX_SIZE];
    int valid_size = (rows * cols <= MAX_SIZE) ? rows * cols : MAX_SIZE;
    
    for (int i = 0; i < valid_size; i++) {
        stack_array[i] = mat1->data[i] * 2.0;
    }
    
    printf("Matrix norm: %.6f\n", compute_matrix_norm(mat1));
    
    if (rows == 2 && cols == 2) {
        printf("2x2 determinant: %.6f\n", compute_determinant_2x2(mat1));
    }
    
    if (rows == cols) {
        struct Matrix *mat2 = create_matrix(rows, cols);
        struct Matrix *result = create_matrix(rows, cols);
        
        if (mat2 != NULL && result != NULL) {
            initialize_matrix(mat2);
            
            if (multiply_matrices(mat1, mat2, result)) {
                printf("Matrix multiplication successful\n");
                printf("Result norm: %.6f\n", compute_matrix_norm(result));
            } else {
                printf("Matrix multiplication failed\n");
            }
            
            destroy_matrix(mat2);
            destroy_matrix(result);
        }
    }
    
    double stack_sum = 0.0;
    for (int i = 0; i < valid_size; i++) {
        stack_sum += stack_array[i];
    }
    printf("Stack array sum: %.6f\n", stack_sum);
    
    destroy_matrix(mat1);
    return 0;
}