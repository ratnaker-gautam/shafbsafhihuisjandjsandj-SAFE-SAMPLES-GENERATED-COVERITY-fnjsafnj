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

void matrix_multiply(const Matrix* a, const Matrix* b, Matrix* result) {
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            result->data[i][j] = 0;
            for (int k = 0; k < a->cols; k++) {
                if (a->data[i][k] > 0 && b->data[k][j] > 0) {
                    if (a->data[i][k] > INT_MAX / b->data[k][j]) {
                        result->data[i][j] = INT_MAX;
                        break;
                    }
                } else if (a->data[i][k] < 0 && b->data[k][j] < 0) {
                    if (a->data[i][k] < INT_MIN / b->data[k][j]) {
                        result->data[i][j] = INT_MIN;
                        break;
                    }
                } else if (a->data[i][k] != 0 && b->data[k][j] != 0) {
                    if ((a->data[i][k] > 0 && b->data[k][j] < 0 && a->data[i][k] > INT_MAX / b->data[k][j]) ||
                        (a->data[i][k] < 0 && b->data[k][j] > 0 && a->data[i][k] < INT_MIN / b->data[k][j])) {
                        result->data[i][j] = (a->data[i][k] > 0) ? INT_MAX : INT_MIN;
                        break;
                    }
                }
                result->data[i][j] += a->data[i][k] * b->data[k][j];
            }
        }
    }
}

void print_matrix(const Matrix* m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%8d", m->data[i][j]);
        }
        printf("\n");
    }
}

int main() {
    Matrix a, b, result;
    
    printf("Matrix Multiplication Calculator\n");
    
    if (!read_int_safe("Enter rows for matrix A: ", &a.rows)) {
        printf("Invalid input for rows A\n");
        return 1;
    }
    if (!read_int_safe("Enter columns for matrix A: ", &a.cols)) {
        printf("Invalid input for columns A\n");
        return 1;
    }
    if (!validate_matrix_dim(a.rows, a.cols)) {
        printf("Invalid matrix A dimensions\n");
        return 1;
    }
    
    if (!read_int_safe("Enter rows for matrix B: ", &b.rows)) {
        printf("Invalid input for rows B\n");
        return 1;
    }
    if (!read_int_safe("Enter columns for matrix B: ", &b.cols)) {
        printf("Invalid input for columns B\n");
        return 1;
    }
    if (!validate_matrix_dim(b.rows, b.cols)) {
        printf("Invalid matrix B dimensions\n");
        return 1;
    }
    
    if (a.cols != b.rows) {
        printf("Matrix dimensions incompatible for multiplication\n");
        return 1;
    }
    
    result.rows = a.rows;
    result.cols = b.cols;
    
    printf("Enter elements for matrix A (%dx%d):\n", a.rows, a.cols);
    for (int i = 0; i < a.rows; i++) {
        for (int j = 0; j < a.cols; j++) {
            if (!read_int_safe("", &a.data[i][j])) {
                printf("Invalid matrix element\n");
                return 1;
            }
        }
    }
    
    printf("Enter elements for matrix B (%dx%d):\n", b.rows, b.cols);
    for (int i = 0; i < b.rows; i++) {
        for (int j = 0; j < b.cols; j++) {
            if (!read_int_safe("", &b.data[i][j])) {
                printf("Invalid matrix element\n");
                return 1;
            }
        }
    }
    
    printf("\nMatrix A:\n");
    print_matrix(&a);
    printf("\nMatrix B:\n");