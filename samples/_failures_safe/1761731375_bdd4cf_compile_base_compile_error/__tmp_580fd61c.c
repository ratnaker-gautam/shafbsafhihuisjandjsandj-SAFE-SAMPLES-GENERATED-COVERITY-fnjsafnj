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

int read_matrix_input(Matrix* mat) {
    if (mat == NULL) {
        return 0;
    }
    
    printf("Enter %d x %d matrix elements:\n", mat->rows, mat->cols);
    
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("Element [%d][%d]: ", i, j);
            
            char buffer[32];
            if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
                return 0;
            }
            
            char* endptr;
            long value = strtol(buffer, &endptr, 10);
            if (endptr == buffer || *endptr != '\n') {
                return 0;
            }
            
            if (value < INT_MIN || value > INT_MAX) {
                return 0;
            }
            
            mat->data[i][j] = (int)value;
        }
    }
    
    return 1;
}

void print_matrix(const Matrix* mat) {
    if (mat == NULL) {
        return;
    }
    
    printf("Matrix %d x %d:\n", mat->rows, mat->cols);
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%6d", mat->data[i][j]);
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
                
                long product = (long)a->data[i][k] * (long)b->data[k][j];
                if (sum > 0 && product > INT_MAX - sum) {
                    destroy_matrix(result);
                    return NULL;
                }
                if (sum < 0 && product < INT_MIN - sum) {
                    destroy_matrix(result);
                    return NULL;
                }
                
                sum += (int)product;
            }
            result->data[i][j] = sum;
        }
    }
    
    return result;
}

int main(void) {
    int rows1, cols1, rows2, cols2;
    char buffer[32];
    
    printf("Enter dimensions for first matrix (rows cols): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    
    if (sscanf(buffer, "%d %d", &rows1, &cols1) != 2) {
        printf("Invalid dimensions\n");
        return 1;
    }
    
    Matrix* mat1 = create_matrix(rows1, cols1);
    if (mat1 == NULL) {
        printf("Failed to create first matrix\n");
        return 1;
    }
    
    if (!read_matrix_input(mat1)) {
        printf("Failed to read first matrix data\n");
        destroy_matrix(mat1);
        return 1;
    }
    
    printf("Enter dimensions for second matrix (rows cols): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        printf