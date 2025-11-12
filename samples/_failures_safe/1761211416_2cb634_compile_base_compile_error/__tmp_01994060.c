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

int set_matrix_element(struct Matrix *mat, int row, int col, double value) {
    if (!mat || !mat->data) return 0;
    if (row < 0 || row >= mat->rows || col < 0 || col >= mat->cols) return 0;
    mat->data[row * mat->cols + col] = value;
    return 1;
}

double get_matrix_element(struct Matrix *mat, int row, int col) {
    if (!mat || !mat->data) return 0.0;
    if (row < 0 || row >= mat->rows || col < 0 || col >= mat->cols) return 0.0;
    return mat->data[row * mat->cols + col];
}

int multiply_matrices(struct Matrix *a, struct Matrix *b, struct Matrix *result) {
    if (!a || !b || !result) return 0;
    if (a->cols != b->rows) return 0;
    if (result->rows != a->rows || result->cols != b->cols) return 0;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            double sum = 0.0;
            for (int k = 0; k < a->cols; k++) {
                sum += get_matrix_element(a, i, k) * get_matrix_element(b, k, j);
            }
            if (!set_matrix_element(result, i, j, sum)) return 0;
        }
    }
    return 1;
}

void print_matrix(struct Matrix *mat) {
    if (!mat || !mat->data) return;
    
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%8.2f", get_matrix_element(mat, i, j));
        }
        printf("\n");
    }
}

int main() {
    int rows1, cols1, rows2, cols2;
    
    printf("Enter dimensions of first matrix (rows cols): ");
    if (scanf("%d %d", &rows1, &cols1) != 2) {
        printf("Invalid input\n");
        return 1;
    }
    
    printf("Enter dimensions of second matrix (rows cols): ");
    if (scanf("%d %d", &rows2, &cols2) != 2) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (cols1 != rows2) {
        printf("Matrix dimensions incompatible for multiplication\n");
        return 1;
    }
    
    struct Matrix *mat1 = create_matrix(rows1, cols1);
    struct Matrix *mat2 = create_matrix(rows2, cols2);
    struct Matrix *result = create_matrix(rows1, cols2);
    
    if (!mat1 || !mat2 || !result) {
        printf("Memory allocation failed\n");
        destroy_matrix(mat1);
        destroy_matrix(mat2);
        destroy_matrix(result);
        return 1;
    }
    
    printf("Enter elements of first matrix (%d x %d):\n", rows1, cols1);
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols1; j++) {
            double value;
            if (scanf("%lf", &value) != 1) {
                printf("Invalid input\n");
                destroy_matrix(mat1);
                destroy_matrix(mat2);
                destroy_matrix(result);
                return 1;
            }
            if (!set_matrix_element(mat1, i, j, value)) {
                printf("Failed to set matrix element\n");
                destroy_matrix(mat1);
                destroy_matrix(mat2);
                destroy_matrix(result);
                return 1;
            }
        }
    }
    
    printf("Enter elements of second matrix (%d x %d):\n", rows2, cols2);
    for (int i = 0; i < rows2; i++) {
        for (int j = 0; j < cols2; j++) {
            double value;
            if (scanf("%lf", &value) != 1) {
                printf("Invalid input\n");
                destroy_matrix(mat1);
                destroy_matrix(mat2);
                destroy_matrix(result);
                return 1;
            }
            if (!set_matrix_element(mat2, i, j, value)) {
                printf("Failed to set matrix element\n");
                destroy_matrix(mat1);
                destroy_matrix(mat2);
                destroy