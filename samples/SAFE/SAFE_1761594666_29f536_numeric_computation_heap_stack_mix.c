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
    if (rows * cols > 10000) return 0;
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
    
    for (int i = 0; i < mat->rows * mat->cols; i++) {
        mat->data[i] = (double)(i % 10) + 1.0;
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
    
    if (sum < 0.0) return 0.0;
    return sqrt(sum);
}

int main() {
    int rows, cols;
    
    printf("Enter matrix rows (1-10): ");
    if (scanf("%d", &rows) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    printf("Enter matrix cols (1-10): ");
    if (scanf("%d", &cols) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (!validate_matrix_params(rows, cols)) {
        printf("Invalid matrix dimensions\n");
        return 1;
    }
    
    struct Matrix *matrix = create_matrix(rows, cols);
    if (!matrix) {
        printf("Memory allocation failed\n");
        return 1;
    }
    
    if (!initialize_matrix(matrix)) {
        printf("Matrix initialization failed\n");
        destroy_matrix(matrix);
        return 1;
    }
    
    printf("Matrix created with %d rows and %d columns\n", rows, cols);
    
    double stack_array[MAX_SIZE];
    int stack_size = rows * cols;
    if (stack_size > MAX_SIZE) stack_size = MAX_SIZE;
    
    for (int i = 0; i < stack_size; i++) {
        stack_array[i] = matrix->data[i] * 2.0;
    }
    
    printf("First %d elements doubled and stored on stack:\n", stack_size);
    for (int i = 0; i < stack_size; i++) {
        printf("%.2f ", stack_array[i]);
    }
    printf("\n");
    
    double norm = compute_matrix_norm(matrix);
    printf("Matrix Frobenius norm: %.4f\n", norm);
    
    if (rows == 2 && cols == 2) {
        double det = compute_determinant_2x2(matrix);
        printf("2x2 matrix determinant: %.4f\n", det);
    }
    
    destroy_matrix(matrix);
    return 0;
}