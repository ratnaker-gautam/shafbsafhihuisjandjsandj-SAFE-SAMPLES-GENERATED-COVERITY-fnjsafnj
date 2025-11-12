//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 100

struct Matrix {
    int rows;
    int cols;
    int data[MAX_DIM][MAX_DIM];
};

int validate_dimension(int dim) {
    return (dim > 0 && dim <= MAX_DIM);
}

int read_matrix(struct Matrix *m) {
    if (m == NULL) return 0;
    
    printf("Enter number of rows: ");
    if (scanf("%d", &m->rows) != 1) return 0;
    if (!validate_dimension(m->rows)) return 0;
    
    printf("Enter number of columns: ");
    if (scanf("%d", &m->cols) != 1) return 0;
    if (!validate_dimension(m->cols)) return 0;
    
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            if (scanf("%d", &m->data[i][j]) != 1) return 0;
        }
    }
    return 1;
}

void print_matrix(const struct Matrix *m) {
    if (m == NULL) return;
    
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%d ", m->data[i][j]);
        }
        printf("\n");
    }
}

struct Matrix *matrix_multiply(const struct Matrix *a, const struct Matrix *b) {
    if (a == NULL || b == NULL) return NULL;
    if (a->cols != b->rows) return NULL;
    
    struct Matrix *result = malloc(sizeof(struct Matrix));
    if (result == NULL) return NULL;
    
    result->rows = a->rows;
    result->cols = b->cols;
    
    for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                if (a->data[i][k] > 0 && b->data[k][j] > 0) {
                    if (a->data[i][k] > INT_MAX / b->data[k][j]) {
                        free(result);
                        return NULL;
                    }
                } else if (a->data[i][k] < 0 && b->data[k][j] < 0) {
                    if (a->data[i][k] < INT_MIN / b->data[k][j]) {
                        free(result);
                        return NULL;
                    }
                }
                sum += a->data[i][k] * b->data[k][j];
            }
            result->data[i][j] = sum;
        }
    }
    
    return result;
}

int main(void) {
    struct Matrix mat1, mat2;
    
    printf("Matrix Multiplication Program\n");
    printf("Maximum dimension: %d\n\n", MAX_DIM);
    
    printf("First matrix:\n");
    if (!read_matrix(&mat1)) {
        printf("Error reading first matrix\n");
        return 1;
    }
    
    printf("\nSecond matrix:\n");
    if (!read_matrix(&mat2)) {
        printf("Error reading second matrix\n");
        return 1;
    }
    
    if (mat1.cols != mat2.rows) {
        printf("Matrix dimensions incompatible for multiplication\n");
        return 1;
    }
    
    struct Matrix *result = matrix_multiply(&mat1, &mat2);
    if (result == NULL) {
        printf("Error during matrix multiplication\n");
        return 1;
    }
    
    printf("\nFirst matrix:\n");
    print_matrix(&mat1);
    
    printf("\nSecond matrix:\n");
    print_matrix(&mat2);
    
    printf("\nResult matrix:\n");
    print_matrix(result);
    
    free(result);
    return 0;
}