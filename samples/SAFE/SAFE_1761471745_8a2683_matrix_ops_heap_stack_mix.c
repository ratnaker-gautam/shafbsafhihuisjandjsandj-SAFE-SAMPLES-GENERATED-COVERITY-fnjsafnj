//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ROWS 100
#define MAX_COLS 100

typedef struct {
    int rows;
    int cols;
    int data[MAX_ROWS][MAX_COLS];
} Matrix;

Matrix* create_matrix(int rows, int cols) {
    if (rows <= 0 || rows > MAX_ROWS || cols <= 0 || cols > MAX_COLS) {
        return NULL;
    }
    Matrix* mat = malloc(sizeof(Matrix));
    if (mat == NULL) {
        return NULL;
    }
    mat->rows = rows;
    mat->cols = cols;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            mat->data[i][j] = 0;
        }
    }
    return mat;
}

void destroy_matrix(Matrix* mat) {
    if (mat != NULL) {
        free(mat);
    }
}

int read_matrix(Matrix* mat) {
    if (mat == NULL) {
        return 0;
    }
    printf("Enter %d x %d matrix elements:\n", mat->rows, mat->cols);
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            if (scanf("%d", &mat->data[i][j]) != 1) {
                return 0;
            }
        }
    }
    return 1;
}

void print_matrix(const Matrix* mat) {
    if (mat == NULL) {
        return;
    }
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%d ", mat->data[i][j]);
        }
        printf("\n");
    }
}

Matrix* multiply_matrices(const Matrix* a, const Matrix* b) {
    if (a == NULL || b == NULL || a->cols != b->rows) {
        return NULL;
    }
    Matrix* result = create_matrix(a->rows, b->cols);
    if (result == NULL) {
        return NULL;
    }
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                if ((a->data[i][k] > 0 && b->data[k][j] > 0 && a->data[i][k] > INT_MAX / b->data[k][j]) ||
                    (a->data[i][k] < 0 && b->data[k][j] < 0 && a->data[i][k] < INT_MAX / b->data[k][j]) ||
                    (a->data[i][k] > 0 && b->data[k][j] < 0 && a->data[i][k] > INT_MIN / b->data[k][j]) ||
                    (a->data[i][k] < 0 && b->data[k][j] > 0 && a->data[i][k] < INT_MIN / b->data[k][j])) {
                    destroy_matrix(result);
                    return NULL;
                }
                int product = a->data[i][k] * b->data[k][j];
                if ((product > 0 && sum > INT_MAX - product) || (product < 0 && sum < INT_MIN - product)) {
                    destroy_matrix(result);
                    return NULL;
                }
                sum += product;
            }
            result->data[i][j] = sum;
        }
    }
    return result;
}

int main() {
    int rows1, cols1, rows2, cols2;
    
    printf("Enter dimensions of first matrix (rows cols): ");
    if (scanf("%d %d", &rows1, &cols1) != 2) {
        printf("Invalid input\n");
        return 1;
    }
    
    Matrix* mat1 = create_matrix(rows1, cols1);
    if (mat1 == NULL) {
        printf("Failed to create matrix 1\n");
        return 1;
    }
    
    if (!read_matrix(mat1)) {
        printf("Failed to read matrix 1\n");
        destroy_matrix(mat1);
        return 1;
    }
    
    printf("Enter dimensions of second matrix (rows cols): ");
    if (scanf("%d %d", &rows2, &cols2) != 2) {
        printf("Invalid input\n");
        destroy_matrix(mat1);
        return 1;
    }
    
    Matrix* mat2 = create_matrix(rows2, cols2);
    if (mat2 == NULL) {
        printf("Failed to create matrix 2\n");
        destroy_matrix(mat1);
        return 1;
    }
    
    if (!read_matrix(mat2)) {
        printf("Failed to read matrix 2\n");
        destroy_matrix(mat1);
        destroy_matrix(mat2);
        return 1;
    }
    
    Matrix* result = multiply_matrices(mat1, mat2);
    if (result == NULL) {
        printf("Matrix multiplication failed\n");
    } else {
        printf("Result matrix:\n");
        print_matrix(result);
        destroy_matrix(result);
    }
    
    destroy_matrix(mat1);
    destroy_matrix(mat2);
    return 0;
}