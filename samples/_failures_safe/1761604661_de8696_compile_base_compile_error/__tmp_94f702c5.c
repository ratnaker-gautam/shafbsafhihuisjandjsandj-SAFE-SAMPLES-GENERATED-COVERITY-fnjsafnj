//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 100

typedef struct {
    int rows;
    int cols;
    int **data;
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

Matrix* matrix_create(int rows, int cols) {
    if (rows <= 0 || cols <= 0 || rows > MAX_DIM || cols > MAX_DIM) {
        return NULL;
    }
    
    Matrix *m = malloc(sizeof(Matrix));
    if (!m) return NULL;
    
    m->rows = rows;
    m->cols = cols;
    m->data = malloc(rows * sizeof(int*));
    if (!m->data) {
        free(m);
        return NULL;
    }
    
    for (int i = 0; i < rows; i++) {
        m->data[i] = calloc(cols, sizeof(int));
        if (!m->data[i]) {
            for (int j = 0; j < i; j++) {
                free(m->data[j]);
            }
            free(m->data);
            free(m);
            return NULL;
        }
    }
    return m;
}

void matrix_free(Matrix *m) {
    if (m) {
        for (int i = 0; i < m->rows; i++) {
            free(m->data[i]);
        }
        free(m->data);
        free(m);
    }
}

int matrix_multiply(const Matrix *a, const Matrix *b, Matrix *result) {
    if (!a || !b || !result) return 0;
    if (a->cols != b->rows) return 0;
    if (result->rows != a->rows || result->cols != b->cols) return 0;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                int product = safe_multiply(a->data[i][k], b->data[k][j]);
                if (!product && a->data[i][k] != 0 && b->data[k][j] != 0) {
                    return 0;
                }
                sum = safe_add(sum, product);
                if (!sum && product != 0) {
                    return 0;
                }
            }
            result->data[i][j] = sum;
        }
    }
    return 1;
}

void matrix_print(const Matrix *m) {
    if (!m) return;
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%d ", m->data[i][j]);
        }
        printf("\n");
    }
}

int main(void) {
    int rows1, cols1, rows2, cols2;
    
    printf("Enter dimensions for first matrix (rows cols): ");
    if (scanf("%d %d", &rows1, &cols1) != 2) {
        printf("Invalid input\n");
        return 1;
    }
    
    printf("Enter dimensions for second matrix (rows cols): ");
    if (scanf("%d %d", &rows2, &cols2) != 2) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (cols1 != rows2) {
        printf("Matrix dimensions incompatible for multiplication\n");
        return 1;
    }
    
    Matrix *a = matrix_create(rows1, cols1);
    Matrix *b = matrix_create(rows2, cols2);
    Matrix *result = matrix_create(rows1, cols2);
    
    if (!a || !b || !result) {
        printf("Memory allocation failed\n");
        matrix_free(a);
        matrix_free(b);
        matrix_free(result);
        return 1;
    }
    
    printf("Enter elements for first matrix (%d x %d):\n", rows1, cols1);
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols1; j++) {
            if (scanf("%d", &a->data[i][j]) != 1) {
                printf("Invalid input\n");
                matrix_free(a);
                matrix_free(b);
                matrix_free(result);
                return 1;
            }
        }
    }
    
    printf("Enter elements for second matrix (%d x %d):\n", rows2, cols2);
    for (int i = 0; i < rows2; i++) {
        for (int j = 0; j < cols2; j++) {
            if