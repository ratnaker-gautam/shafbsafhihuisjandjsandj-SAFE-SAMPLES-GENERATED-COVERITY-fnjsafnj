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
            mat->data[i * mat->cols + j] = (i + 1) * 0.5 + (j + 1) * 0.3;
        }
    }
    return 1;
}

double compute_determinant_2x2(struct Matrix *mat) {
    if (!mat || mat->rows != 2 || mat->cols != 2) return 0.0;
    return mat->data[0] * mat->data[3] - mat->data[1] * mat->data[2];
}

double* compute_row_sums(struct Matrix *mat) {
    if (!mat || !mat->data) return NULL;
    
    double *sums = malloc(mat->rows * sizeof(double));
    if (!sums) return NULL;
    
    for (int i = 0; i < mat->rows; i++) {
        sums[i] = 0.0;
        for (int j = 0; j < mat->cols; j++) {
            sums[i] += mat->data[i * mat->cols + j];
        }
    }
    return sums;
}

int main() {
    int rows, cols;
    
    printf("Enter matrix dimensions (rows cols, max %dx%d): ", MAX_SIZE, MAX_SIZE);
    if (scanf("%d %d", &rows, &cols) != 2) {
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
    
    if (!fill_matrix(mat)) {
        printf("Matrix initialization failed\n");
        destroy_matrix(mat);
        return 1;
    }
    
    printf("Matrix created successfully\n");
    
    if (rows == 2 && cols == 2) {
        double det = compute_determinant_2x2(mat);
        printf("2x2 matrix determinant: %.2f\n", det);
    }
    
    double *row_sums = compute_row_sums(mat);
    if (row_sums) {
        printf("Row sums: ");
        for (int i = 0; i < rows; i++) {
            printf("%.2f ", row_sums[i]);
        }
        printf("\n");
        free(row_sums);
    }
    
    destroy_matrix(mat);
    return 0;
}