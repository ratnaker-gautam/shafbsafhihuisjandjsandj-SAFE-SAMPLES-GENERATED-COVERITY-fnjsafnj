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
    int **data;
};

int validate_dimension(int dim) {
    return dim > 0 && dim <= MAX_DIM;
}

int allocate_matrix(struct Matrix *mat, int rows, int cols) {
    if (!validate_dimension(rows) || !validate_dimension(cols)) {
        return 0;
    }
    
    mat->rows = rows;
    mat->cols = cols;
    mat->data = malloc(rows * sizeof(int*));
    if (mat->data == NULL) return 0;
    
    for (int i = 0; i < rows; i++) {
        mat->data[i] = malloc(cols * sizeof(int));
        if (mat->data[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(mat->data[j]);
            }
            free(mat->data);
            return 0;
        }
    }
    return 1;
}

void free_matrix(struct Matrix *mat) {
    if (mat->data != NULL) {
        for (int i = 0; i < mat->rows; i++) {
            free(mat->data[i]);
        }
        free(mat->data);
        mat->data = NULL;
    }
}

int read_matrix(struct Matrix *mat) {
    printf("Enter matrix elements (%d x %d):\n", mat->rows, mat->cols);
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            if (scanf("%d", &mat->data[i][j]) != 1) {
                return 0;
            }
        }
    }
    return 1;
}

void print_matrix(const struct Matrix *mat) {
    printf("Matrix (%d x %d):\n", mat->rows, mat->cols);
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%d ", mat->data[i][j]);
        }
        printf("\n");
    }
}

int multiply_matrices(const struct Matrix *a, const struct Matrix *b, struct Matrix *result) {
    if (a->cols != b->rows) return 0;
    
    if (!allocate_matrix(result, a->rows, b->cols)) {
        return 0;
    }
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                if ((a->data[i][k] > 0 && b->data[k][j] > INT_MAX / a->data[i][k]) ||
                    (a->data[i][k] < 0 && b->data[k][j] < INT_MIN / a->data[i][k])) {
                    free_matrix(result);
                    return 0;
                }
                sum += a->data[i][k] * b->data[k][j];
                if ((a->data[i][k] > 0 && sum < 0) || (a->data[i][k] < 0 && sum > 0)) {
                    free_matrix(result);
                    return 0;
                }
            }
            result->data[i][j] = sum;
        }
    }
    return 1;
}

int main(void) {
    struct Matrix mat1, mat2, result;
    int rows1, cols1, rows2, cols2;
    
    printf("Enter dimensions for first matrix (rows cols): ");
    if (scanf("%d %d", &rows1, &cols1) != 2) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (!allocate_matrix(&mat1, rows1, cols1)) {
        printf("Failed to allocate matrix 1\n");
        return 1;
    }
    
    if (!read_matrix(&mat1)) {
        printf("Failed to read matrix 1\n");
        free_matrix(&mat1);
        return 1;
    }
    
    printf("Enter dimensions for second matrix (rows cols): ");
    if (scanf("%d %d", &rows2, &cols2) != 2) {
        printf("Invalid input\n");
        free_matrix(&mat1);
        return 1;
    }
    
    if (!allocate_matrix(&mat2, rows2, cols2)) {
        printf("Failed to allocate matrix 2\n");
        free_matrix(&mat1);
        return 1;
    }
    
    if (!read_matrix(&mat2)) {
        printf("Failed to read matrix 2\n");
        free_matrix(&mat1);
        free_matrix(&mat2);
        return 1;
    }
    
    printf("\nFirst matrix:\n");
    print_matrix(&mat1);
    printf("\nSecond matrix:\n");
    print_matrix(&mat2);
    
    if (multiply_matrices(&mat1, &mat2, &result)) {
        printf("\nMatrix multiplication result:\n");
        print_matrix(&result);
        free_matrix(&result);
    } else {
        printf("\nMatrix multiplication failed: dimension mismatch or integer overflow\n");
    }
    
    free_matrix(&mat1);
    free_matrix(&mat2);
    
    return 0;
}