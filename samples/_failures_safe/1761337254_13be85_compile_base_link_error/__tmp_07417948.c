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
    return rows > 0 && rows <= MAX_SIZE && cols > 0 && cols <= MAX_SIZE;
}

struct Matrix* create_matrix(int rows, int cols) {
    if (!validate_matrix_size(rows, cols)) return NULL;
    
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

int read_matrix(struct Matrix *mat) {
    if (!mat) return 0;
    
    printf("Enter %d x %d matrix elements:\n", mat->rows, mat->cols);
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            if (scanf("%lf", &mat->data[i * mat->cols + j]) != 1) {
                return 0;
            }
        }
    }
    return 1;
}

double compute_determinant_2x2(struct Matrix *mat) {
    if (!mat || mat->rows != 2 || mat->cols != 2) return 0.0;
    return mat->data[0] * mat->data[3] - mat->data[1] * mat->data[2];
}

double compute_matrix_norm(struct Matrix *mat) {
    if (!mat) return 0.0;
    
    double norm = 0.0;
    for (int i = 0; i < mat->rows * mat->cols; i++) {
        norm += mat->data[i] * mat->data[i];
    }
    return sqrt(norm);
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
    
    struct Matrix *mat = create_matrix(rows, cols);
    if (!mat) {
        printf("Memory allocation failed\n");
        return 1;
    }
    
    if (!read_matrix(mat)) {
        printf("Failed to read matrix\n");
        destroy_matrix(mat);
        return 1;
    }
    
    double matrix_stack[MAX_SIZE];
    int stack_size = 0;
    
    for (int i = 0; i < mat->rows * mat->cols && stack_size < MAX_SIZE; i++) {
        matrix_stack[stack_size++] = mat->data[i];
    }
    
    double sum_stack = 0.0;
    for (int i = 0; i < stack_size; i++) {
        sum_stack += matrix_stack[i];
    }
    
    double norm = compute_matrix_norm(mat);
    double determinant = 0.0;
    
    if (rows == 2 && cols == 2) {
        determinant = compute_determinant_2x2(mat);
    }
    
    printf("Matrix norm: %.6f\n", norm);
    printf("Sum of elements (via stack): %.6f\n", sum_stack);
    
    if (rows == 2 && cols == 2) {
        printf("Determinant: %.6f\n", determinant);
    }
    
    destroy_matrix(mat);
    return 0;
}