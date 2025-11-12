//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: matrix_ops
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

void matrix_multiply(const Matrix *a, const Matrix *b, Matrix *result) {
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
                    int64_t product = (int64_t)a->data[i][k] * (int64_t)b->data[k][j];
                    if (product > INT_MAX) {
                        result->data[i][j] = INT_MAX;
                        break;
                    } else if (product < INT_MIN) {
                        result->data[i][j] = INT_MIN;
                        break;
                    }
                }
                result->data[i][j] += a->data[i][k] * b->data[k][j];
            }
        }
    }
}

void print_matrix(const Matrix *m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%d", m->data[i][j]);
            if (j < m->cols - 1) printf(" ");
        }
        printf("\n");
    }
}

int read_matrix(Matrix *m) {
    int rows, cols;
    if (scanf("%d %d", &rows, &cols) != 2) return 0;
    if (!validate_matrix_dim(rows, cols)) return 0;
    
    m->rows = rows;
    m->cols = cols;
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (scanf("%d", &m->data[i][j]) != 1) return 0;
        }
    }
    return 1;
}

int main() {
    Matrix a, b, result;
    
    printf("Enter first matrix dimensions and elements (rows cols then matrix):\n");
    if (!read_matrix(&a)) {
        printf("Invalid first matrix input\n");
        return 1;
    }
    
    printf("Enter second matrix dimensions and elements (rows cols then matrix):\n");
    if (!read_matrix(&b)) {
        printf("Invalid second matrix input\n");
        return 1;
    }
    
    if (a.cols != b.rows) {
        printf("Matrix dimensions incompatible for multiplication\n");
        return 1;
    }
    
    if (!validate_matrix_dim(a.rows, b.cols)) {
        printf("Result matrix would exceed maximum dimensions\n");
        return 1;
    }
    
    result.rows = a.rows;
    result.cols = b.cols;
    
    matrix_multiply(&a, &b, &result);
    
    printf("Result matrix:\n");
    print_matrix(&result);
    
    return 0;
}