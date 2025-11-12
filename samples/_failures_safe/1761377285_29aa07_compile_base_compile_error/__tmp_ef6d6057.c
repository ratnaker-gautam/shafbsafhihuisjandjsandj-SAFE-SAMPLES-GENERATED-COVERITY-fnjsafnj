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
    return (dim > 0 && dim <= MAX_DIM);
}

int read_int(const char *prompt, int min, int max) {
    char buffer[32];
    int value;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        
        char *endptr;
        long temp = strtol(buffer, &endptr, 10);
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (temp < min || temp > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        
        value = (int)temp;
        break;
    }
    
    return value;
}

struct Matrix create_matrix(int rows, int cols) {
    struct Matrix mat;
    mat.rows = rows;
    mat.cols = cols;
    
    mat.data = malloc(rows * sizeof(int *));
    if (mat.data == NULL) {
        mat.rows = 0;
        mat.cols = 0;
        return mat;
    }
    
    for (int i = 0; i < rows; i++) {
        mat.data[i] = malloc(cols * sizeof(int));
        if (mat.data[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(mat.data[j]);
            }
            free(mat.data);
            mat.rows = 0;
            mat.cols = 0;
            return mat;
        }
    }
    
    return mat;
}

void free_matrix(struct Matrix *mat) {
    if (mat->data != NULL) {
        for (int i = 0; i < mat->rows; i++) {
            free(mat->data[i]);
        }
        free(mat->data);
    }
    mat->rows = 0;
    mat->cols = 0;
    mat->data = NULL;
}

void fill_matrix(struct Matrix *mat) {
    printf("Enter matrix elements:\n");
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            char prompt[32];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            mat->data[i][j] = read_int(prompt, INT_MIN, INT_MAX);
        }
    }
}

void print_matrix(const struct Matrix *mat) {
    printf("Matrix (%dx%d):\n", mat->rows, mat->cols);
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%6d ", mat->data[i][j]);
        }
        printf("\n");
    }
}

struct Matrix multiply_matrices(const struct Matrix *a, const struct Matrix *b) {
    struct Matrix result = create_matrix(a->rows, b->cols);
    if (result.data == NULL) {
        return result;
    }
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            long sum = 0;
            for (int k = 0; k < a->cols; k++) {
                long product = (long)a->data[i][k] * (long)b->data[k][j];
                if (product > INT_MAX - sum || product < INT_MIN + sum) {
                    free_matrix(&result);
                    result.rows = 0;
                    result.cols = 0;
                    return result;
                }
                sum += product;
            }
            result.data[i][j] = (int)sum;
        }
    }
    
    return result;
}

int main(void) {
    printf("Matrix Operations Program\n");
    
    int rows1 = read_int("Enter rows for matrix 1 (1-100): ", 1, MAX_DIM);
    int cols1 = read_int("Enter columns for matrix 1 (1-100): ", 1, MAX_DIM);
    
    if (rows1 == -1 || cols1 == -1) {
        return 1;
    }
    
    struct Matrix mat1 = create_matrix(rows1, cols1);
    if (mat1.data == NULL) {
        printf("Memory allocation failed for matrix 1.\n");
        return 1;
    }
    
    fill_matrix(&mat1);
    
    int rows2 = read_int("Enter rows for matrix 2 (1-100): ", 1, MAX_DIM);
    int cols2 = read_int("Enter columns for matrix 2 (1-100): ", 1, MAX_DIM);
    
    if (rows2 == -1 || cols2 == -1) {
        free_matrix(&mat1);
        return 1;
    }
    
    if (cols1 != rows2) {
        printf("Matrix dimensions incompatible for multiplication.\n");
        free_matrix(&mat1);
        return 1;
    }
    
    struct Matrix mat2 = create_matrix(rows2, cols2);
    if (mat2.data == NULL)