//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>

struct Matrix {
    size_t rows;
    size_t cols;
    double *data;
};

struct Matrix create_matrix(size_t rows, size_t cols) {
    struct Matrix mat;
    mat.rows = rows;
    mat.cols = cols;
    if (rows == 0 || cols == 0) {
        mat.data = NULL;
        return mat;
    }
    if (rows > SIZE_MAX / cols / sizeof(double)) {
        mat.data = NULL;
        mat.rows = 0;
        mat.cols = 0;
        return mat;
    }
    mat.data = malloc(rows * cols * sizeof(double));
    if (mat.data == NULL) {
        mat.rows = 0;
        mat.cols = 0;
    } else {
        for (size_t i = 0; i < rows * cols; i++) {
            mat.data[i] = 0.0;
        }
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

int set_element(struct Matrix *mat, size_t row, size_t col, double value) {
    if (mat == NULL || mat->data == NULL) return 0;
    if (row >= mat->rows || col >= mat->cols) return 0;
    mat->data[row * mat->cols + col] = value;
    return 1;
}

double get_element(const struct Matrix *mat, size_t row, size_t col) {
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
                sum += get_element(a, i, k) * get_element(b, k, j);
            }
            set_element(&result, i, j, sum);
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
            set_element(&result, j, i, get_element(mat, i, j));
        }
    }
    return result;
}

double matrix_determinant(const struct Matrix *mat) {
    if (mat == NULL || mat->data == NULL) return 0.0;
    if (mat->rows != mat->cols) return 0.0;
    if (mat->rows == 1) return get_element(mat, 0, 0);
    if (mat->rows == 2) {
        return get_element(mat, 0, 0) * get_element(mat, 1, 1) - 
               get_element(mat, 0, 1) * get_element(mat, 1, 0);
    }
    
    double det = 0.0;
    for (size_t col = 0; col < mat->cols; col++) {
        struct Matrix submat = create_matrix(mat->rows - 1, mat->cols - 1);
        if (submat.data == NULL) {
            destroy_matrix(&submat);
            return 0.0;
        }
        
        for (size_t i = 1; i < mat->rows; i++) {
            size_t subcol = 0;
            for (size_t j = 0; j < mat->cols; j++) {
                if (j == col) continue;
                set_element(&submat, i-1, subcol, get_element(mat, i, j));
                subcol++;
            }
        }
        
        double subdet = matrix_determinant(&submat);
        double sign = (col % 2 == 0) ? 1.0 : -1.0;
        det += sign * get_element(mat, 0, col) * subdet;
        destroy_matrix(&submat);
    }
    return det;
}

void print_matrix(const struct Matrix *mat) {
    if (mat == NULL || mat->data == NULL) return;
    for (size_t i = 0; i < mat->rows; i++) {
        for (size_t j = 0; j < mat->cols; j++) {
            printf("%8