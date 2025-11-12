//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
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

int matrix_multiply(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a == NULL || b == NULL || result == NULL) {
        return 0;
    }
    
    if (a->cols != b->rows) {
        return 0;
    }
    
    if (result->rows != a->rows || result->cols != b->cols) {
        return 0;
    }
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                if ((a->data[i][k] > 0 && b->data[k][j] > INT_MAX / a->data[i][k]) ||
                    (a->data[i][k] < 0 && b->data[k][j] < INT_MIN / a->data[i][k])) {
                    return 0;
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

int main() {
    int rows1, cols1, rows2, cols2;
    
    printf("Enter dimensions for first matrix (rows cols): ");
    if (scanf("%d %d", &rows1, &cols1) != 2) {
        printf("Invalid input\n");
        return 1;
    }
    
    Matrix* mat1 = create_matrix(rows1, cols1);
    if (mat1 == NULL) {
        printf("Failed to create first matrix\n");
        return 1;
    }
    
    if (!read_matrix(mat1)) {
        printf("Failed to read first matrix\n");
        destroy_matrix(mat1);
        return 1;
    }
    
    printf("Enter dimensions for second matrix (rows cols): ");
    if (scanf("%d %d", &rows2, &cols2) != 2) {
        printf("Invalid input\n");
        destroy_matrix(mat1);
        return 1;
    }
    
    Matrix* mat2 = create_matrix(rows2, cols2);
    if (mat2 == NULL) {
        printf("Failed to create second matrix\n");
        destroy_matrix(mat1);
        return 1;
    }
    
    if (!read_matrix(mat2)) {
        printf("Failed to read second matrix\n");
        destroy_matrix(mat1);
        destroy_matrix(mat2);
        return 1;
    }
    
    if (cols1 != rows2) {
        printf("Matrix dimensions incompatible for multiplication\n");
        destroy_matrix(mat1);
        destroy_matrix(mat2);
        return 1;
    }
    
    Matrix* result = create_matrix(rows1, cols2);
    if (result == NULL) {
        printf("Failed to create result matrix\n");
        destroy_matrix(mat1);
        destroy_matrix(mat2);
        return 1;
    }
    
    printf("\nFirst matrix:\n");
    print_matrix(mat1);
    
    printf("\nSecond matrix:\n");
    print_matrix(mat2);
    
    if (matrix_multiply(mat1, mat2, result)) {
        printf("\nResult matrix:\n");
        print_matrix(result);
    } else {
        printf("Matrix multiplication failed\n");
    }
    
    destroy_matrix(mat1);
    destroy_matrix(mat