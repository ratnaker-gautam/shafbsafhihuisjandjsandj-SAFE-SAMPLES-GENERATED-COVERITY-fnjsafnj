//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct Matrix {
    size_t rows;
    size_t cols;
    double *data;
};

struct Matrix create_matrix(size_t rows, size_t cols) {
    struct Matrix mat;
    mat.rows = rows;
    mat.cols = cols;
    mat.data = malloc(rows * cols * sizeof(double));
    if (mat.data == NULL) {
        mat.rows = 0;
        mat.cols = 0;
    }
    return mat;
}

void destroy_matrix(struct Matrix *mat) {
    if (mat != NULL && mat->data != NULL) {
        free(mat->data);
        mat->data = NULL;
        mat->rows = 0;
        mat->cols = 0;
    }
}

int set_matrix_element(struct Matrix *mat, size_t row, size_t col, double value) {
    if (mat == NULL || mat->data == NULL) return 0;
    if (row >= mat->rows || col >= mat->cols) return 0;
    mat->data[row * mat->cols + col] = value;
    return 1;
}

double get_matrix_element(const struct Matrix *mat, size_t row, size_t col) {
    if (mat == NULL || mat->data == NULL) return 0.0;
    if (row >= mat->rows || col >= mat->cols) return 0.0;
    return mat->data[row * mat->cols + col];
}

struct Matrix matrix_multiply(const struct Matrix *a, const struct Matrix *b) {
    struct Matrix result = create_matrix(0, 0);
    if (a == NULL || b == NULL || a->data == NULL || b->data == NULL) return result;
    if (a->cols != b->rows) return result;
    
    result = create_matrix(a->rows, b->cols);
    if (result.data == NULL) return result;
    
    for (size_t i = 0; i < a->rows; i++) {
        for (size_t j = 0; j < b->cols; j++) {
            double sum = 0.0;
            for (size_t k = 0; k < a->cols; k++) {
                sum += get_matrix_element(a, i, k) * get_matrix_element(b, k, j);
            }
            set_matrix_element(&result, i, j, sum);
        }
    }
    return result;
}

struct Matrix matrix_transpose(const struct Matrix *mat) {
    struct Matrix result = create_matrix(0, 0);
    if (mat == NULL || mat->data == NULL) return result;
    
    result = create_matrix(mat->cols, mat->rows);
    if (result.data == NULL) return result;
    
    for (size_t i = 0; i < mat->rows; i++) {
        for (size_t j = 0; j < mat->cols; j++) {
            set_matrix_element(&result, j, i, get_matrix_element(mat, i, j));
        }
    }
    return result;
}

void print_matrix(const struct Matrix *mat) {
    if (mat == NULL || mat->data == NULL) return;
    for (size_t i = 0; i < mat->rows; i++) {
        for (size_t j = 0; j < mat->cols; j++) {
            printf("%8.2f", get_matrix_element(mat, i, j));
        }
        printf("\n");
    }
}

int read_positive_size(const char *prompt) {
    int value;
    printf("%s", prompt);
    if (scanf("%d", &value) != 1) return -1;
    if (value <= 0) return -1;
    return value;
}

int main(void) {
    int rows1, cols1, rows2, cols2;
    
    printf("Matrix Operations Program\n");
    
    rows1 = read_positive_size("Enter rows for matrix A: ");
    if (rows1 == -1) {
        printf("Invalid input for rows.\n");
        return 1;
    }
    
    cols1 = read_positive_size("Enter columns for matrix A: ");
    if (cols1 == -1) {
        printf("Invalid input for columns.\n");
        return 1;
    }
    
    struct Matrix matA = create_matrix(rows1, cols1);
    if (matA.data == NULL) {
        printf("Memory allocation failed for matrix A.\n");
        return 1;
    }
    
    printf("Enter %d elements for matrix A (%dx%d):\n", rows1 * cols1, rows1, cols1);
    for (size_t i = 0; i < matA.rows; i++) {
        for (size_t j = 0; j < matA.cols; j++) {
            double value;
            if (scanf("%lf", &value) != 1) {
                printf("Invalid input for matrix element.\n");
                destroy_matrix(&matA);
                return 1;
            }
            set_matrix_element(&matA, i, j, value);
        }
    }
    
    rows2 = read_positive_size("Enter rows for matrix B: ");
    if (rows2 == -1) {
        printf("Invalid input for rows.\n");
        destroy_matrix(&matA);
        return 1;
    }
    
    cols2 = read_positive_size("Enter columns for matrix B: ");
    if (cols2 == -1) {
        printf("Invalid input for columns.\n");
        destroy_matrix(&matA