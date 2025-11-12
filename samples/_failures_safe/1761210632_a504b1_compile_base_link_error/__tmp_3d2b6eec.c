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

int fill_matrix(struct Matrix *mat) {
    if (mat == NULL || mat->data == NULL) {
        return 0;
    }
    
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            double value = sin(i * 0.1) * cos(j * 0.1) + (i + j) * 0.01;
            mat->data[i * mat->cols + j] = value;
        }
    }
    return 1;
}

double compute_determinant_2x2(struct Matrix *mat) {
    if (mat == NULL || mat->rows != 2 || mat->cols != 2) {
        return 0.0;
    }
    
    return mat->data[0] * mat->data[3] - mat->data[1] * mat->data[2];
}

void matrix_multiply(struct Matrix *a, struct Matrix *b, struct Matrix *result) {
    if (a == NULL || b == NULL || result == NULL) {
        return;
    }
    
    if (a->cols != b->rows || result->rows != a->rows || result->cols != b->cols) {
        return;
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
    
    if (!fill_matrix(mat1)) {
        printf("Matrix initialization failed\n");
        destroy_matrix(mat1);
        return 1;
    }
    
    struct Matrix *mat2 = create_matrix(cols, rows);
    if (mat2 == NULL) {
        printf("Memory allocation failed\n");
        destroy_matrix(mat1);
        return 1;
    }
    
    if (!fill_matrix(mat2)) {
        printf("Matrix initialization failed\n");
        destroy_matrix(mat1);
        destroy_matrix(mat2);
        return 1;
    }
    
    struct Matrix *result = create_matrix(rows, rows);
    if (result == NULL) {
        printf("Memory allocation failed\n");
        destroy_matrix(mat1);
        destroy_matrix(mat2);
        return 1;
    }
    
    matrix_multiply(mat1, mat2, result);
    
    double stack_buffer[MAX_SIZE];
    int buffer_size = (rows < MAX_SIZE) ? rows : MAX_SIZE;
    
    for (int i = 0; i < buffer_size; i++) {
        if (i < result->rows && i < result->cols) {
            stack_buffer[i] = result->data[i * result->cols + i];
        } else {
            stack_buffer[i] = 0.0;
        }
    }
    
    double trace = 0.0;
    for (int i = 0; i < buffer_size; i++) {
        trace += stack_buffer[i];
    }
    
    printf("Matrix multiplication completed\n");
    printf("Trace of result matrix: %.6f\n", trace);
    
    if (rows == 2 && cols == 2) {
        double det = compute_determinant_2x2(mat1);
        printf("Determinant of first matrix: %.6f\n", det);
    }
    
    destroy_matrix(mat1);
    destroy_matrix(mat2);
    destroy_matrix(result);
    
    return 0;
}