//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ROWS 10
#define MAX_COLS 10
#define MATRIX_SIZE (MAX_ROWS * MAX_COLS)

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

void matrix_init(Matrix *m) {
    memset(m->data, 0, sizeof(m->data));
    m->rows = 0;
    m->cols = 0;
}

int matrix_input(Matrix *m) {
    int rows, cols;
    printf("Enter matrix dimensions (rows cols, max %dx%d): ", MAX_ROWS, MAX_COLS);
    if (scanf("%d %d", &rows, &cols) != 2) return 0;
    if (rows <= 0 || rows > MAX_ROWS || cols <= 0 || cols > MAX_COLS) return 0;
    
    m->rows = rows;
    m->cols = cols;
    
    printf("Enter %d elements row by row:\n", rows * cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (scanf("%d", &m->data[i][j]) != 1) return 0;
        }
    }
    return 1;
}

void matrix_print(const Matrix *m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%6d", m->data[i][j]);
        }
        printf("\n");
    }
}

int matrix_add(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a->rows != b->rows || a->cols != b->cols) return 0;
    
    result->rows = a->rows;
    result->cols = a->cols;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            int sum = safe_add(a->data[i][j], b->data[i][j]);
            if (sum == 0 && (a->data[i][j] != 0 || b->data[i][j] != 0)) return 0;
            result->data[i][j] = sum;
        }
    }
    return 1;
}

int matrix_multiply(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a->cols != b->rows) return 0;
    
    result->rows = a->rows;
    result->cols = b->cols;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                int product = safe_multiply(a->data[i][k], b->data[k][j]);
                if (product == 0 && (a->data[i][k] != 0 || b->data[k][j] != 0)) return 0;
                int new_sum = safe_add(sum, product);
                if (new_sum == 0 && (sum != 0 || product != 0)) return 0;
                sum = new_sum;
            }
            result->data[i][j] = sum;
        }
    }
    return 1;
}

int matrix_transpose(const Matrix *m, Matrix *result) {
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
    int choice;
    
    matrix_init(&a);
    matrix_init(&b);
    matrix_init(&result);
    
    printf("Matrix Operations Program\n");
    printf("1. Input Matrix A\n");
    printf("2. Input Matrix B\n");
    printf("3. Add Matrices\n");
    printf("4. Multiply Matrices\n");
    printf("5. Transpose Matrix A\n");
    printf("6. Exit\n");
    
    while (1) {
        printf("\nChoose operation (1-6): ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input\n