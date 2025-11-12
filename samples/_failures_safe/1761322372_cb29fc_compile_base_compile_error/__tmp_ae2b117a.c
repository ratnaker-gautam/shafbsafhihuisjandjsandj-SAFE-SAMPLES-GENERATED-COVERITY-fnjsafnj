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
    struct Matrix m;
    m.rows = rows;
    m.cols = cols;
    m.data = malloc(rows * cols * sizeof(double));
    if (m.data == NULL) {
        m.rows = 0;
        m.cols = 0;
    }
    return m;
}

void destroy_matrix(struct Matrix *m) {
    if (m != NULL && m->data != NULL) {
        free(m->data);
        m->data = NULL;
        m->rows = 0;
        m->cols = 0;
    }
}

int set_matrix_element(struct Matrix *m, size_t row, size_t col, double value) {
    if (m == NULL || m->data == NULL) return 0;
    if (row >= m->rows || col >= m->cols) return 0;
    m->data[row * m->cols + col] = value;
    return 1;
}

double get_matrix_element(const struct Matrix *m, size_t row, size_t col) {
    if (m == NULL || m->data == NULL) return 0.0;
    if (row >= m->rows || col >= m->cols) return 0.0;
    return m->data[row * m->cols + col];
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

struct Matrix matrix_transpose(const struct Matrix *m) {
    struct Matrix result = create_matrix(0, 0);
    if (m == NULL || m->data == NULL) return result;
    
    result = create_matrix(m->cols, m->rows);
    if (result.data == NULL) return result;
    
    for (size_t i = 0; i < m->rows; i++) {
        for (size_t j = 0; j < m->cols; j++) {
            set_matrix_element(&result, j, i, get_matrix_element(m, i, j));
        }
    }
    return result;
}

void print_matrix(const struct Matrix *m) {
    if (m == NULL || m->data == NULL) return;
    for (size_t i = 0; i < m->rows; i++) {
        for (size_t j = 0; j < m->cols; j++) {
            printf("%8.2f", get_matrix_element(m, i, j));
        }
        printf("\n");
    }
}

int read_matrix_from_input(struct Matrix *m) {
    if (m == NULL) return 0;
    
    printf("Enter number of rows: ");
    if (scanf("%zu", &m->rows) != 1 || m->rows == 0 || m->rows > 100) return 0;
    
    printf("Enter number of columns: ");
    if (scanf("%zu", &m->cols) != 1 || m->cols == 0 || m->cols > 100) return 0;
    
    destroy_matrix(m);
    *m = create_matrix(m->rows, m->cols);
    if (m->data == NULL) return 0;
    
    printf("Enter matrix elements row by row:\n");
    for (size_t i = 0; i < m->rows; i++) {
        for (size_t j = 0; j < m->cols; j++) {
            double value;
            if (scanf("%lf", &value) != 1) return 0;
            set_matrix_element(m, i, j, value);
        }
    }
    return 1;
}

int main(void) {
    struct Matrix a = create_matrix(0, 0);
    struct Matrix b = create_matrix(0, 0);
    
    printf("Matrix A:\n");
    if (!read_matrix_from_input(&a)) {
        printf("Invalid input for matrix A\n");
        destroy_matrix(&a);
        destroy_matrix(&b);
        return 1;
    }
    
    printf("Matrix B:\n");
    if (!read_matrix_from_input(&b)) {
        printf("Invalid input for matrix B\n");
        destroy_matrix(&a);
        destroy_matrix(&b);
        return 1;
    }
    
    printf("\nMatrix A:\n");
    print_matrix(&a);
    
    printf("\nMatrix B:\n");
    print_matrix(&b);
    
    printf("\nMatrix A Transpose:\n");
    struct Matrix a_transpose = matrix_transpose(&a);
    print_matrix(&a_transpose);
    
    printf("\nMatrix B Transpose:\n");