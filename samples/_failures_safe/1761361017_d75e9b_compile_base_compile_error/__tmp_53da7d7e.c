//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 10
#define MATRIX_ELEMENT_MAX 1000
#define MATRIX_ELEMENT_MIN -1000

#define MATRIX_OP_ADD 1
#define MATRIX_OP_SUBTRACT 2
#define MATRIX_OP_MULTIPLY 3

#define VALIDATE_SIZE(s) ((s) > 0 && (s) <= MAX_SIZE)
#define VALIDATE_ELEMENT(e) ((e) >= MATRIX_ELEMENT_MIN && (e) <= MATRIX_ELEMENT_MAX)

typedef struct {
    int rows;
    int cols;
    int data[MAX_SIZE][MAX_SIZE];
} Matrix;

int read_matrix(Matrix *m) {
    if (!m) return 0;
    
    printf("Enter number of rows (1-%d): ", MAX_SIZE);
    if (scanf("%d", &m->rows) != 1) return 0;
    if (!VALIDATE_SIZE(m->rows)) return 0;
    
    printf("Enter number of columns (1-%d): ", MAX_SIZE);
    if (scanf("%d", &m->cols) != 1) return 0;
    if (!VALIDATE_SIZE(m->cols)) return 0;
    
    printf("Enter matrix elements (%d to %d):\n", MATRIX_ELEMENT_MIN, MATRIX_ELEMENT_MAX);
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            if (scanf("%d", &m->data[i][j]) != 1) return 0;
            if (!VALIDATE_ELEMENT(m->data[i][j])) return 0;
        }
    }
    return 1;
}

void print_matrix(const Matrix *m) {
    if (!m) return;
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%6d", m->data[i][j]);
        }
        printf("\n");
    }
}

int matrix_add(const Matrix *a, const Matrix *b, Matrix *result) {
    if (!a || !b || !result) return 0;
    if (a->rows != b->rows || a->cols != b->cols) return 0;
    
    result->rows = a->rows;
    result->cols = a->cols;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            long long sum = (long long)a->data[i][j] + b->data[i][j];
            if (sum > MATRIX_ELEMENT_MAX || sum < MATRIX_ELEMENT_MIN) return 0;
            result->data[i][j] = (int)sum;
        }
    }
    return 1;
}

int matrix_subtract(const Matrix *a, const Matrix *b, Matrix *result) {
    if (!a || !b || !result) return 0;
    if (a->rows != b->rows || a->cols != b->cols) return 0;
    
    result->rows = a->rows;
    result->cols = a->cols;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            long long diff = (long long)a->data[i][j] - b->data[i][j];
            if (diff > MATRIX_ELEMENT_MAX || diff < MATRIX_ELEMENT_MIN) return 0;
            result->data[i][j] = (int)diff;
        }
    }
    return 1;
}

int matrix_multiply(const Matrix *a, const Matrix *b, Matrix *result) {
    if (!a || !b || !result) return 0;
    if (a->cols != b->rows) return 0;
    
    result->rows = a->rows;
    result->cols = b->cols;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            long long sum = 0;
            for (int k = 0; k < a->cols; k++) {
                long long product = (long long)a->data[i][k] * b->data[k][j];
                if (product > INT_MAX || product < INT_MIN) return 0;
                sum += product;
                if (sum > INT_MAX || sum < INT_MIN) return 0;
            }
            if (sum > MATRIX_ELEMENT_MAX || sum < MATRIX_ELEMENT_MIN) return 0;
            result->data[i][j] = (int)sum;
        }
    }
    return 1;
}

int main() {
    Matrix a, b, result;
    int operation;
    
    printf("Matrix Operations Program\n");
    printf("Available operations:\n");
    printf("1. Addition\n");
    printf("2. Subtraction\n");
    printf("3. Multiplication\n");
    
    printf("Select operation (1-3): ");
    if (scanf("%d", &operation) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (operation < MATRIX_OP_