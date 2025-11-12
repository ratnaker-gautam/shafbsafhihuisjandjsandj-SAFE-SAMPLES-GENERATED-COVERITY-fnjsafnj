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

int multiply_matrices(const struct Matrix *a, const struct Matrix *b, struct Matrix *result) {
    if (!a || !b || !result) return 0;
    if (a->cols != b->rows) return 0;
    if (result->rows != a->rows || result->cols != b->cols) return 0;
    
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

void print_matrix(const struct Matrix *mat) {
    if (!mat || !mat->data) return;
    
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%8.2f", mat->data[i * mat->cols + j]);
        }
        printf("\n");
    }
}

int main() {
    int rows_a, cols_a, rows_b, cols_b;
    
    printf("Enter dimensions for matrix A (rows cols): ");
    if (scanf("%d %d", &rows_a, &cols_a) != 2) {
        printf("Invalid input\n");
        return 1;
    }
    
    printf("Enter dimensions for matrix B (rows cols): ");
    if (scanf("%d %d", &rows_b, &cols_b) != 2) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (cols_a != rows_b) {
        printf("Matrix dimensions incompatible for multiplication\n");
        return 1;
    }
    
    struct Matrix *a = create_matrix(rows_a, cols_a);
    struct Matrix *b = create_matrix(rows_b, cols_b);
    struct Matrix *result = create_matrix(rows_a, cols_b);
    
    if (!a || !b || !result) {
        printf("Memory allocation failed\n");
        destroy_matrix(a);
        destroy_matrix(b);
        destroy_matrix(result);
        return 1;
    }
    
    if (!initialize_matrix(a) || !initialize_matrix(b)) {
        printf("Matrix initialization failed\n");
        destroy_matrix(a);
        destroy_matrix(b);
        destroy_matrix(result);
        return 1;
    }
    
    printf("\nMatrix A:\n");
    print_matrix(a);
    
    printf("\nMatrix B:\n");
    print_matrix(b);
    
    if (multiply_matrices(a, b, result)) {
        printf("\nResult (A * B):\n");
        print_matrix(result);
    } else {
        printf("Matrix multiplication failed\n");
    }
    
    destroy_matrix(a);
    destroy_matrix(b);
    destroy_matrix(result);
    
    return 0;
}