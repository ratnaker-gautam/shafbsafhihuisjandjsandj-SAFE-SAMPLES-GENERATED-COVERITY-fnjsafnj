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
    if (m != NULL) {
        free(m->data);
        m->data = NULL;
        m->rows = 0;
        m->cols = 0;
    }
}

int set_element(struct Matrix *m, size_t row, size_t col, double value) {
    if (m == NULL || m->data == NULL) return 0;
    if (row >= m->rows || col >= m->cols) return 0;
    m->data[row * m->cols + col] = value;
    return 1;
}

double get_element(const struct Matrix *m, size_t row, size_t col) {
    if (m == NULL || m->data == NULL) return 0.0;
    if (row >= m->rows || col >= m->cols) return 0.0;
    return m->data[row * m->cols + col];
}

struct Matrix add_matrices(const struct Matrix *a, const struct Matrix *b) {
    struct Matrix result = create_matrix(0, 0);
    if (a == NULL || b == NULL || a->data == NULL || b->data == NULL) return result;
    if (a->rows != b->rows || a->cols != b->cols) return result;
    
    result = create_matrix(a->rows, a->cols);
    if (result.data == NULL) return result;
    
    for (size_t i = 0; i < a->rows; i++) {
        for (size_t j = 0; j < a->cols; j++) {
            double sum = get_element(a, i, j) + get_element(b, i, j);
            set_element(&result, i, j, sum);
        }
    }
    return result;
}

struct Matrix multiply_matrices(const struct Matrix *a, const struct Matrix *b) {
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

void print_matrix(const struct Matrix *m) {
    if (m == NULL || m->data == NULL) return;
    for (size_t i = 0; i < m->rows; i++) {
        for (size_t j = 0; j < m->cols; j++) {
            printf("%8.2f", get_element(m, i, j));
        }
        printf("\n");
    }
}

int read_double(const char *prompt, double *value) {
    if (prompt == NULL || value == NULL) return 0;
    printf("%s", prompt);
    char buffer[100];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return 0;
    char *endptr;
    *value = strtod(buffer, &endptr);
    return (endptr != buffer);
}

int read_size_t(const char *prompt, size_t *value) {
    if (prompt == NULL || value == NULL) return 0;
    printf("%s", prompt);
    char buffer[100];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return 0;
    char *endptr;
    long long temp = strtoll(buffer, &endptr, 10);
    if (endptr == buffer || temp < 0 || temp > 100) return 0;
    *value = (size_t)temp;
    return 1;
}

int main(void) {
    struct Matrix a = create_matrix(0, 0);
    struct Matrix b = create_matrix(0, 0);
    
    size_t rows, cols;
    
    printf("Matrix Operations Demo\n");
    
    if (!read_size_t("Enter rows for matrix A: ", &rows) || rows == 0) {
        printf("Invalid input for rows.\n");
        destroy_matrix(&a);
        destroy_matrix(&b);
        return 1;
    }
    
    if (!read_size_t("Enter columns for matrix A: ", &cols) || cols == 0) {
        printf("Invalid input for columns.\n");
        destroy_matrix(&a);
        destroy_matrix(&b);
        return 1;
    }
    
    a = create_matrix(rows, cols);
    if (a.data == NULL) {
        printf("Failed to create matrix A.\n");
        destroy_matrix(&