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
    if (m == NULL || m->data == NULL || row >= m->rows || col >= m->cols) {
        return 0;
    }
    m->data[row * m->cols + col] = value;
    return 1;
}

double get_matrix_element(const struct Matrix *m, size_t row, size_t col) {
    if (m == NULL || m->data == NULL || row >= m->rows || col >= m->cols) {
        return 0.0;
    }
    return m->data[row * m->cols + col];
}

struct Matrix matrix_multiply(const struct Matrix *a, const struct Matrix *b) {
    struct Matrix result = create_matrix(0, 0);
    if (a == NULL || b == NULL || a->data == NULL || b->data == NULL || a->cols != b->rows) {
        return result;
    }
    
    result = create_matrix(a->rows, b->cols);
    if (result.data == NULL) {
        return result;
    }
    
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
    if (m == NULL || m->data == NULL) {
        return result;
    }
    
    result = create_matrix(m->cols, m->rows);
    if (result.data == NULL) {
        return result;
    }
    
    for (size_t i = 0; i < m->rows; i++) {
        for (size_t j = 0; j < m->cols; j++) {
            set_matrix_element(&result, j, i, get_matrix_element(m, i, j));
        }
    }
    return result;
}

void print_matrix(const struct Matrix *m) {
    if (m == NULL || m->data == NULL) {
        printf("Invalid matrix\n");
        return;
    }
    
    for (size_t i = 0; i < m->rows; i++) {
        for (size_t j = 0; j < m->cols; j++) {
            printf("%8.2f", get_matrix_element(m, i, j));
        }
        printf("\n");
    }
}

int read_matrix_from_input(struct Matrix *m) {
    if (m == NULL || m->data == NULL) {
        return 0;
    }
    
    printf("Enter %zu x %zu matrix elements:\n", m->rows, m->cols);
    for (size_t i = 0; i < m->rows; i++) {
        for (size_t j = 0; j < m->cols; j++) {
            double value;
            if (scanf("%lf", &value) != 1) {
                return 0;
            }
            set_matrix_element(m, i, j, value);
        }
    }
    return 1;
}

int main(void) {
    size_t rows1, cols1, rows2, cols2;
    
    printf("Enter dimensions of first matrix (rows cols): ");
    if (scanf("%zu %zu", &rows1, &cols1) != 2 || rows1 == 0 || cols1 == 0) {
        printf("Invalid dimensions\n");
        return 1;
    }
    
    printf("Enter dimensions of second matrix (rows cols): ");
    if (scanf("%zu %zu", &rows2, &cols2) != 2 || rows2 == 0 || cols2 == 0) {
        printf("Invalid dimensions\n");
        return 1;
    }
    
    struct Matrix mat1 = create_matrix(rows1, cols1);
    struct Matrix mat2 = create_matrix(rows2, cols2);
    
    if (mat1.data == NULL || mat2.data == NULL) {
        printf("Memory allocation failed\n");
        destroy_matrix(&mat1);
        destroy_matrix(&mat2);
        return 1;
    }
    
    if (!read_matrix_from_input(&mat1)) {
        printf("Failed to read first matrix\n");
        destroy_matrix(&mat1);
        destroy_matrix(&mat2);
        return 1;
    }
    
    if (!read_matrix_from_input(&mat2)) {
        printf("Failed to read second matrix\n");
        destroy