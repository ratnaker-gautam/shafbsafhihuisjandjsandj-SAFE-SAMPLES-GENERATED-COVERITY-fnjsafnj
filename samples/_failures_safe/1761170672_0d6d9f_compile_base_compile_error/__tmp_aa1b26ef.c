//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 10

typedef struct {
    int rows;
    int cols;
    int data[MAX_DIM][MAX_DIM];
} Matrix;

int validate_dimension(int dim) {
    return dim > 0 && dim <= MAX_DIM;
}

int read_matrix(Matrix *m) {
    if (scanf("%d %d", &m->rows, &m->cols) != 2) {
        return 0;
    }
    if (!validate_dimension(m->rows) || !validate_dimension(m->cols)) {
        return 0;
    }
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            if (scanf("%d", &m->data[i][j]) != 1) {
                return 0;
            }
        }
    }
    return 1;
}

void print_matrix(const Matrix *m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%d ", m->data[i][j]);
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
            if ((a->data[i][j] > 0 && b->data[i][j] > INT_MAX - a->data[i][j]) ||
                (a->data[i][j] < 0 && b->data[i][j] < INT_MIN - a->data[i][j])) {
                return 0;
            }
            result->data[i][j] = a->data[i][j] + b->data[i][j];
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
                if (a->data[i][k] != 0 && b->data[k][j] != 0) {
                    if ((a->data[i][k] > 0 && b->data[k][j] > INT_MAX / a->data[i][k]) ||
                        (a->data[i][k] < 0 && b->data[k][j] < INT_MAX / a->data[i][k])) {
                        return 0;
                    }
                }
                int product = a->data[i][k] * b->data[k][j];
                if ((product > 0 && sum > INT_MAX - product) ||
                    (product < 0 && sum < INT_MIN - product)) {
                    return 0;
                }
                sum += product;
            }
            result->data[i][j] = sum;
        }
    }
    return 1;
}

int transpose_matrix(const Matrix *m, Matrix *result) {
    result->rows = m->cols;
    result->cols = m->rows;
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            result->data[j][i] = m->data[i][j];
        }
    }
    return 1;
}

int main() {
    Matrix a, b, result;
    int operation;
    
    printf("Enter first matrix (rows cols elements):\n");
    if (!read_matrix(&a)) {
        printf("Invalid first matrix input\n");
        return 1;
    }
    
    printf("Enter second matrix (rows cols elements):\n");
    if (!read_matrix(&b)) {
        printf("Invalid second matrix input\n");
        return 1;
    }
    
    printf("Choose operation (1=add, 2=multiply, 3=transpose first): ");
    if (scanf("%d", &operation) != 1) {
        printf("Invalid operation input\n");
        return 1;
    }
    
    switch (operation) {
        case 1:
            if (add_matrices(&a, &b, &result)) {
                printf("Matrix sum:\n");
                print_matrix(&result);
            } else {
                printf("Matrix addition failed: dimension mismatch or overflow\n");
            }
            break;
        case 2:
            if (multiply_matrices(&a, &b, &result)) {
                printf("Matrix product:\n");
                print_matrix(&result);
            } else {
                printf("Matrix multiplication failed: dimension mismatch or overflow\n");
            }
            break;
        case 3:
            if (transpose_matrix(&a, &result)) {
                printf("Transpose of first matrix:\n");
                print_matrix(&result);
            } else {
                printf("Matrix transpose