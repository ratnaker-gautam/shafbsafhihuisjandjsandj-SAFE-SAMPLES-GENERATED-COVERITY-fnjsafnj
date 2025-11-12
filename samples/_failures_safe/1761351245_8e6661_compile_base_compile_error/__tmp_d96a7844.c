//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ROWS 10
#define MAX_COLS 10
#define MIN_DIM 1
#define MAX_ELEM 1000
#define MIN_ELEM -1000

typedef struct {
    int data[MAX_ROWS][MAX_COLS];
    int rows;
    int cols;
} Matrix;

int safe_add(int a, int b) {
    if ((b > 0 && a > INT_MAX - b) || (b < 0 && a < INT_MIN - b)) {
        return 0;
    }
    return a + b;
}

int safe_multiply(int a, int b) {
    if (a > 0) {
        if (b > 0) {
            if (a > INT_MAX / b) return 0;
        } else {
            if (b < INT_MIN / a) return 0;
        }
    } else {
        if (b > 0) {
            if (a < INT_MIN / b) return 0;
        } else {
            if (a != 0 && b < INT_MAX / a) return 0;
        }
    }
    return a * b;
}

void init_matrix(Matrix *m) {
    m->rows = 0;
    m->cols = 0;
    memset(m->data, 0, sizeof(m->data));
}

int read_int(const char *prompt, int min_val, int max_val) {
    char buffer[32];
    long value;
    char *endptr;
    
    printf("%s", prompt);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return -1;
    }
    
    value = strtol(buffer, &endptr, 10);
    if (endptr == buffer || *endptr != '\n') {
        return -1;
    }
    if (value < min_val || value > max_val) {
        return -1;
    }
    return (int)value;
}

int read_matrix(Matrix *m) {
    int rows, cols;
    
    rows = read_int("Enter number of rows (1-10): ", MIN_DIM, MAX_ROWS);
    if (rows == -1) {
        printf("Invalid rows input\n");
        return 0;
    }
    
    cols = read_int("Enter number of columns (1-10): ", MIN_DIM, MAX_COLS);
    if (cols == -1) {
        printf("Invalid columns input\n");
        return 0;
    }
    
    m->rows = rows;
    m->cols = cols;
    
    printf("Enter matrix elements (%d x %d):\n", rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int elem = read_int("", MIN_ELEM, MAX_ELEM);
            if (elem == -1) {
                printf("Invalid element input\n");
                return 0;
            }
            m->data[i][j] = elem;
        }
    }
    return 1;
}

void print_matrix(const Matrix *m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%6d", m->data[i][j]);
        }
        printf("\n");
    }
}

int add_matrices(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a->rows != b->rows || a->cols != b->cols) {
        return 0;
    }
    
    result->rows = a->rows;
    result->cols = a->cols;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            int sum = safe_add(a->data[i][j], b->data[i][j]);
            if (sum == 0 && (a->data[i][j] + b->data[i][j]) != 0) {
                return 0;
            }
            result->data[i][j] = sum;
        }
    }
    return 1;
}

int multiply_matrices(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a->cols != b->rows) {
        return 0;
    }
    
    result->rows = a->rows;
    result->cols = b->cols;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                int product = safe_multiply(a->data[i][k], b->data[k][j]);
                if (product == 0 && (a->data[i][k] * b->data[k][j]) != 0) {
                    return 0;
                }
                int new_sum = safe_add(sum, product);
                if (new_sum == 0 && (sum + product) != 0) {
                    return 0;
                }
                sum = new_sum;
            }
            result->data[i][j] = sum;
        }
    }
    return 1;
}

int main() {
    Matrix mat1, mat2, result;
    int choice;
    
    init_matrix(&mat1);
    init