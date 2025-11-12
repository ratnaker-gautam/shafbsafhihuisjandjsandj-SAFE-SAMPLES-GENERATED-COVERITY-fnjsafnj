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
    
    for (int i = 0; i < mat->rows * mat->cols; i++) {
        mat->data[i] = (double)(i % 10) + 1.0;
    }
    return 1;
}

double compute_determinant_2x2(struct Matrix *mat) {
    if (!mat || mat->rows != 2 || mat->cols != 2) return 0.0;
    return mat->data[0] * mat->data[3] - mat->data[1] * mat->data[2];
}

double compute_trace(struct Matrix *mat) {
    if (!mat || mat->rows != mat->cols) return 0.0;
    
    double trace = 0.0;
    for (int i = 0; i < mat->rows; i++) {
        trace += mat->data[i * mat->cols + i];
    }
    return trace;
}

void print_matrix(struct Matrix *mat) {
    if (!mat || !mat->data) return;
    
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%8.2f", mat->data[i * mat->cols + j]);
        }
        printf("\n");
    }
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
    
    printf("\nGenerated matrix:\n");
    print_matrix(matrix);
    
    double stack_buffer[4];
    if (rows == 2 && cols == 2) {
        for (int i = 0; i < 4; i++) {
            stack_buffer[i] = matrix->data[i];
        }
        double det = compute_determinant_2x2(matrix);
        printf("\n2x2 Determinant: %.2f\n", det);
    }
    
    if (rows == cols) {
        double trace = compute_trace(matrix);
        printf("Matrix trace: %.2f\n", trace);
        
        double norm_sq = 0.0;
        for (int i = 0; i < rows * cols; i++) {
            norm_sq += matrix->data[i] * matrix->data[i];
        }
        printf("Frobenius norm: %.2f\n", sqrt(norm_sq));
    }
    
    destroy_matrix(matrix);
    return 0;
}