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
    
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            int idx = i * mat->cols + j;
            mat->data[idx] = (i + 1) * 0.5 + (j + 1) * 0.3;
        }
    }
    return 1;
}

double compute_determinant_2x2(struct Matrix *mat) {
    if (!mat || mat->rows != 2 || mat->cols != 2) return 0.0;
    
    double a = mat->data[0];
    double b = mat->data[1];
    double c = mat->data[2];
    double d = mat->data[3];
    
    return a * d - b * c;
}

double* compute_row_sums(struct Matrix *mat) {
    if (!mat || !mat->data) return NULL;
    
    double *sums = malloc(mat->rows * sizeof(double));
    if (!sums) return NULL;
    
    for (int i = 0; i < mat->rows; i++) {
        double sum = 0.0;
        for (int j = 0; j < mat->cols; j++) {
            int idx = i * mat->cols + j;
            sum += mat->data[idx];
        }
        sums[i] = sum;
    }
    
    return sums;
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
    
    struct Matrix *matrix = create_matrix(rows, cols);
    if (!matrix) {
        printf("Failed to create matrix\n");
        return 1;
    }
    
    if (!initialize_matrix(matrix)) {
        printf("Failed to initialize matrix\n");
        destroy_matrix(matrix);
        return 1;
    }
    
    printf("Matrix created and initialized successfully\n");
    
    if (rows == 2 && cols == 2) {
        double det = compute_determinant_2x2(matrix);
        printf("2x2 matrix determinant: %.2f\n", det);
    }
    
    double *row_sums = compute_row_sums(matrix);
    if (row_sums) {
        printf("Row sums:\n");
        for (int i = 0; i < rows; i++) {
            printf("Row %d: %.2f\n", i + 1, row_sums[i]);
        }
        free(row_sums);
    }
    
    double stack_buffer[10];
    for (int i = 0; i < 10 && i < rows * cols; i++) {
        stack_buffer[i] = matrix->data[i] * 2.0;
    }
    
    printf("First few elements doubled (stack computation):\n");
    for (int i = 0; i < 10 && i < rows * cols; i++) {
        printf("%.2f ", stack_buffer[i]);
    }
    printf("\n");
    
    destroy_matrix(matrix);
    return 0;
}