//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 100

typedef struct {
    int rows;
    int cols;
    int data[MAX_DIM][MAX_DIM];
} Matrix;

int validate_matrix_dim(int rows, int cols) {
    if (rows <= 0 || cols <= 0) return 0;
    if (rows > MAX_DIM || cols > MAX_DIM) return 0;
    return 1;
}

int read_int_safe(const char* prompt, int* value) {
    char buffer[32];
    printf("%s", prompt);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return 0;
    char* endptr;
    long val = strtol(buffer, &endptr, 10);
    if (endptr == buffer || *endptr != '\n') return 0;
    if (val < INT_MIN || val > INT_MAX) return 0;
    *value = (int)val;
    return 1;
}

void matrix_init_zero(Matrix* m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            m->data[i][j] = 0;
        }
    }
}

int matrix_multiply(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->cols != b->rows) return 0;
    if (!validate_matrix_dim(a->rows, b->cols)) return 0;
    
    result->rows = a->rows;
    result->cols = b->cols;
    matrix_init_zero(result);
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            long sum = 0;
            for (int k = 0; k < a->cols; k++) {
                long product = (long)a->data[i][k] * (long)b->data[k][j];
                if (product > INT_MAX - sum || product < INT_MIN - sum) return 0;
                sum += product;
                if (sum > INT_MAX || sum < INT_MIN) return 0;
            }
            result->data[i][j] = (int)sum;
        }
    }
    return 1;
}

void matrix_print(const Matrix* m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%6d", m->data[i][j]);
        }
        printf("\n");
    }
}

int matrix_read(Matrix* m) {
    if (!read_int_safe("Enter number of rows: ", &m->rows)) return 0;
    if (!read_int_safe("Enter number of columns: ", &m->cols)) return 0;
    if (!validate_matrix_dim(m->rows, m->cols)) return 0;
    
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("Element [%d][%d]: ", i, j);
            if (!read_int_safe("", &m->data[i][j])) return 0;
        }
    }
    return 1;
}

int main(void) {
    Matrix a, b, result;
    
    printf("Matrix Multiplication Program\n");
    printf("Maximum dimension: %d\n\n", MAX_DIM);
    
    printf("First matrix:\n");
    if (!matrix_read(&a)) {
        printf("Error: Invalid first matrix input\n");
        return 1;
    }
    
    printf("\nSecond matrix:\n");
    if (!matrix_read(&b)) {
        printf("Error: Invalid second matrix input\n");
        return 1;
    }
    
    printf("\nMultiplying matrices...\n");
    if (!matrix_multiply(&a, &b, &result)) {
        printf("Error: Matrix multiplication failed - dimension mismatch or integer overflow\n");
        return 1;
    }
    
    printf("\nResult matrix:\n");
    matrix_print(&result);
    
    return 0;
}