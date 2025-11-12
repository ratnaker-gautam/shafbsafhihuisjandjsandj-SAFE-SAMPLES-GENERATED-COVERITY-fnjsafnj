//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 100

typedef struct {
    int rows;
    int cols;
    int *data;
} Matrix;

int validate_dimension(int dim) {
    return dim > 0 && dim <= MAX_DIM;
}

int matrix_init(Matrix *m, int rows, int cols) {
    if (!validate_dimension(rows) || !validate_dimension(cols)) return 0;
    m->rows = rows;
    m->cols = cols;
    m->data = malloc(rows * cols * sizeof(int));
    return m->data != NULL;
}

void matrix_free(Matrix *m) {
    free(m->data);
    m->data = NULL;
}

int matrix_get(Matrix *m, int row, int col) {
    if (row < 0 || row >= m->rows || col < 0 || col >= m->cols) return 0;
    return *(m->data + row * m->cols + col);
}

void matrix_set(Matrix *m, int row, int col, int value) {
    if (row < 0 || row >= m->rows || col < 0 || col >= m->cols) return;
    *(m->data + row * m->cols + col) = value;
}

int matrix_multiply(Matrix *a, Matrix *b, Matrix *result) {
    if (a->cols != b->rows) return 0;
    if (!matrix_init(result, a->rows, b->cols)) return 0;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                int val1 = matrix_get(a, i, k);
                int val2 = matrix_get(b, k, j);
                if ((val1 > 0 && val2 > INT_MAX / val1) || 
                    (val1 < 0 && val2 < INT_MAX / val1)) {
                    matrix_free(result);
                    return 0;
                }
                int product = val1 * val2;
                if ((val1 > 0 && val2 > 0 && product < 0) || 
                    (val1 < 0 && val2 < 0 && product < 0)) {
                    matrix_free(result);
                    return 0;
                }
                if (sum > INT_MAX - product || sum < INT_MIN + product) {
                    matrix_free(result);
                    return 0;
                }
                sum += product;
            }
            matrix_set(result, i, j, sum);
        }
    }
    return 1;
}

void matrix_print(Matrix *m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%d ", matrix_get(m, i, j));
        }
        printf("\n");
    }
}

void matrix_fill_sequential(Matrix *m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            matrix_set(m, i, j, i * m->cols + j + 1);
        }
    }
}

int read_int(const char *prompt, int min, int max) {
    int value;
    char buffer[100];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) return -1;
        
        char *endptr;
        long val = strtol(buffer, &endptr, 10);
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        if (val < min || val > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        value = (int)val;
        break;
    }
    return value;
}

int main() {
    Matrix a, b, result;
    
    printf("Matrix Multiplication using Pointer Arithmetic\n");
    printf("=============================================\n");
    
    int rows1 = read_int("Enter rows for matrix A (1-100): ", 1, MAX_DIM);
    if (rows1 == -1) return 1;
    int cols1 = read_int("Enter columns for matrix A (1-100): ", 1, MAX_DIM);
    if (cols1 == -1) return 1;
    
    int rows2 = read_int("Enter rows for matrix B (1-100): ", 1, MAX_DIM);
    if (rows2 == -1) return 1;
    int cols2 = read_int("Enter columns for matrix B (1-100): ", 1, MAX_DIM);
    if (cols2 == -1) return 1;
    
    if (!matrix_init(&a, rows1, cols1)) {
        printf("Failed to initialize matrix A.\n");
        return 1;
    }
    
    if (!matrix_init(&b, rows2, cols2)) {
        printf("Failed to initialize matrix B.\n");
        matrix_free(&a);
        return 1;
    }
    
    matrix_fill_sequential(&a);
    matrix_fill_sequential(&b);