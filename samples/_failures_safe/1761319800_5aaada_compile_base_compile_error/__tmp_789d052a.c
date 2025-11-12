//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: matrix_ops
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
    while (1) {
        printf("%s", prompt);
        if (scanf("%d", &value) != 1) {
            while (getchar() != '\n');
            printf("Invalid input. Please enter a positive integer.\n");
            continue;
        }
        if (value <= 0) {
            printf("Value must be positive. Please try again.\n");
            continue;
        }
        break;
    }
    return value;
}

void read_matrix_data(struct Matrix *mat) {
    if (mat == NULL || mat->data == NULL) return;
    
    printf("Enter matrix elements row by row:\n");
    for (size_t i = 0; i < mat->rows; i++) {
        for (size_t j = 0; j < mat->cols; j++) {
            double value;
            printf("Element [%zu][%zu]: ", i, j);
            while (scanf("%lf", &value) != 1) {
                while (getchar() != '\n');
                printf("Invalid input. Please enter a number: ");
            }
            set_matrix_element(mat, i, j, value);
        }
    }
}

int main(void) {
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    printf("Matrix A:\n");
    int rows_a = read_positive_size("Enter number of rows: ");
    int cols_a = read_positive_size("Enter number of columns: ");
    
    struct Matrix a = create_matrix(rows_a, cols_a);
    if (a.data == NULL) {
        printf("Error: Failed to allocate memory for matrix A.\n");
        return 1;
    }
    
    read_matrix_data(&a);
    
    printf("Matrix B:\n");
    int rows_b = read_positive_size("Enter number of rows: ");
    int cols_b = read_positive_size("Enter number of columns: ");
    
    struct Matrix b = create_matrix(rows_b, cols_b);
    if (b.data == NULL) {