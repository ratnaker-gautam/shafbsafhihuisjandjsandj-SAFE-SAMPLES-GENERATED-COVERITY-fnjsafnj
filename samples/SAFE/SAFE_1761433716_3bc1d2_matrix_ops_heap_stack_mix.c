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
    return dim > 0 && dim <= MAX_DIM;
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
    printf("=============================\n\n");
    
    printf("First Matrix:\n");
    mat1.rows = read_int("Enter rows (1-100): ", 1, MAX_DIM);
    mat1.cols = read_int("Enter columns (1-100): ", 1, MAX_DIM);
    
    if (!validate_dimension(mat1.rows) || !validate_dimension(mat1.cols)) {
        printf("Invalid matrix dimensions.\n");
        return 1;
    }
    
    printf("\nSecond Matrix:\n");
    mat2.rows = read_int("Enter rows (1-100): ", 1, MAX_DIM);
    mat2.cols = read_int("Enter columns (1-100): ", 1, MAX_DIM);
    
    if (!validate_dimension(mat2.rows) || !validate_dimension(mat2.cols)) {
        printf("Invalid matrix dimensions.\n");
        return 1;
    }
    
    if (mat1.cols != mat2.rows) {
        printf("Matrix multiplication not possible: columns of first matrix must equal rows of second matrix.\n");
        return 1;
    }
    
    initialize_matrix(&mat1);
    initialize_matrix(&mat2);
    
    printf("\nFirst Matrix:\n");
    print_matrix(&mat1);
    
    printf("\nSecond Matrix:\n");
    print_matrix(&mat2);
    
    struct Matrix* result = multiply_matrices(&mat1, &mat2);
    if (result == NULL) {
        printf("Error: Failed to multiply matrices.\n");
        return 1;
    }
    
    printf("\nResult Matrix:\n");
    print_matrix(result);
    
    free(result);
    return 0;
}