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
    if (mat == NULL || mat->data == NULL || mat->rows != 2 || mat->cols != 2) {
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
    if (a->cols != b->rows) {
        return 0;
    }
    if (result->rows != a->rows || result->cols != b->cols) {
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
    int rows1, cols1, rows2, cols2;
    
    printf("Enter dimensions for first matrix (rows cols): ");
    if (scanf("%d %d", &rows1, &cols1) != 2) {
        printf("Invalid input\n");
        return 1;
    }
    
    printf("Enter dimensions for second matrix (rows cols): ");
    if (scanf("%d %d", &rows2, &cols2) != 2) {
        printf("Invalid input\n");
        return 1;
    }
    
    struct Matrix *mat1 = create_matrix(rows1, cols1);
    struct Matrix *mat2 = create_matrix(rows2, cols2);
    
    if (mat1 == NULL || mat2 == NULL) {
        printf("Failed to create matrices\n");
        if (mat1 != NULL) destroy_matrix(mat1);
        if (mat2 != NULL) destroy_matrix(mat2);
        return 1;
    }
    
    if (!initialize_matrix(mat1) || !initialize_matrix(mat2)) {
        printf("Failed to initialize matrices\n");
        destroy_matrix(mat1);
        destroy_matrix(mat2);
        return 1;
    }
    
    printf("Matrix 1 norm: %.6f\n", compute_matrix_norm(mat1));
    printf("Matrix 2 norm: %.6f\n", compute_matrix_norm(mat2));
    
    if (mat1->rows == 2 && mat1->cols == 2) {
        printf("Matrix 1 determinant: %.6f\n", compute_determinant_2x2(mat1));
    }
    
    if (mat2->rows == 2 && mat2->cols == 2) {
        printf("Matrix 2 determinant: %.6f\n", compute_determinant_2x2(mat2));
    }
    
    if (mat1->cols == mat2->rows) {
        struct Matrix *result = create_matrix(mat1->rows, mat2->cols);
        if (result != NULL) {
            if (multiply_matrices(mat1, mat2, result)) {
                printf("Matrix multiplication successful\n");
                printf("Result matrix norm: %.6f\n", compute_matrix_norm(result));
            } else {
                printf("Matrix multiplication failed\n");
            }
            destroy_matrix(result);
        } else {
            printf("Failed to create result matrix\n");
        }
    } else {
        printf("Matrices cannot be multiplied (dimension mismatch)\n