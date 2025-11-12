//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: heap_stack_mix ; Variation: matrix_ops
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

int read_int(const char* prompt, int min, int max) {
    int value;
    char buffer[100];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        
        if (sscanf(buffer, "%d", &value) != 1) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if (value >= min && value <= max) {
            return value;
        }
        printf("Value must be between %d and %d.\n", min, max);
    }
}

void initialize_matrix(struct Matrix* mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            mat->data[i][j] = i * mat->cols + j + 1;
        }
    }
}

void print_matrix(const struct Matrix* mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%4d", mat->data[i][j]);
        }
        printf("\n");
    }
}

struct Matrix* multiply_matrices(const struct Matrix* a, const struct Matrix* b) {
    if (a->cols != b->rows) {
        return NULL;
    }
    
    struct Matrix* result = malloc(sizeof(struct Matrix));
    if (result == NULL) {
        return NULL;
    }
    
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
                    int product = a->data[i][k] * b->data[k][j];
                    if (sum > INT_MAX - product) {
                        free(result);
                        return NULL;
                    }
                    sum += product;
                } else {
                    int product = a->data[i][k] * b->data[k][j];
                    if ((a->data[i][k] < 0 && b->data[k][j] < 0 && product < 0) ||
                        (a->data[i][k] > 0 && b->data[k][j] < 0 && product > 0) ||
                        (a->data[i][k] < 0 && b->data[k][j] > 0 && product > 0)) {
                        free(result);
                        return NULL;
                    }
                    if (sum > 0 && product > INT_MAX - sum) {
                        free(result);
                        return NULL;
                    }
                    if (sum < 0 && product < INT_MIN - sum) {
                        free(result);
                        return NULL;
                    }
                    sum += product;
                }
            }
            result->data[i][j] = sum;
        }
    }
    
    return result;
}

int main(void) {
    struct Matrix mat1, mat2;
    
    printf("Matrix Multiplication Program\n");
    printf("=============================\n\n");
    
    mat1.rows = read_int("Enter rows for matrix 1 (1-100): ", 1, MAX_DIM);
    if (mat1.rows == -1) {
        return 1;
    }
    
    mat1.cols = read_int("Enter columns for matrix 1 (1-100): ", 1, MAX_DIM);
    if (mat1.cols == -1) {
        return 1;
    }
    
    mat2.rows = read_int("Enter rows for matrix 2 (1-100): ", 1, MAX_DIM);
    if (mat2.rows == -1) {
        return 1;
    }
    
    mat2.cols = read_int("Enter columns for matrix 2 (1-100): ", 1, MAX_DIM);
    if (mat2.cols == -1) {
        return 1;
    }
    
    if (mat1.cols != mat2.rows) {
        printf("Error: Matrix 1 columns must equal matrix 2 rows for multiplication.\n");
        return 1;
    }
    
    initialize_matrix(&mat1);
    initialize_matrix(&mat2);
    
    printf("\nMatrix 1:\n");
    print_matrix(&mat1);
    
    printf("\nMatrix 2:\n");
    print_matrix(&mat2);
    
    struct Matrix* result = multiply_matrices(&mat1, &mat2);
    if (result == NULL) {
        printf("Error: Matrix multiplication failed (dimension mismatch or integer overflow).\n");
        return 1;
    }
    
    printf("\nResult of multiplication:\n");
    print_matrix(result);
    
    free