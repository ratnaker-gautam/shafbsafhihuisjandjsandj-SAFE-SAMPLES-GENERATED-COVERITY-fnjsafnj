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

int validate_matrix_params(int rows, int cols) {
    return validate_dimension(rows) && validate_dimension(cols);
}

int allocate_matrix(struct Matrix *mat, int rows, int cols) {
    if (!validate_matrix_params(rows, cols)) {
        return 0;
    }
    
    mat->rows = rows;
    mat->cols = cols;
    
    mat->data = malloc(rows * sizeof(int*));
    if (mat->data == NULL) {
        return 0;
    }
    
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
    }
}

void initialize_matrix(struct Matrix *mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            mat->data[i][j] = i * mat->cols + j + 1;
        }
    }
}

void print_matrix(struct Matrix *mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%4d", mat->data[i][j]);
        }
        printf("\n");
    }
}

int multiply_matrices(struct Matrix *a, struct Matrix *b, struct Matrix *result) {
    if (a->cols != b->rows) {
        return 0;
    }
    
    if (!allocate_matrix(result, a->rows, b->cols)) {
        return 0;
    }
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                sum += a->data[i][k] * b->data[k][j];
            }
            result->data[i][j] = sum;
        }
    }
    return 1;
}

int get_valid_int(const char *prompt, int min, int max) {
    int value;
    char buffer[100];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            continue;
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

int main(void) {
    struct Matrix mat_a, mat_b, result;
    
    printf("Matrix Multiplication Program\n");
    printf("=============================\n\n");
    
    printf("Matrix A dimensions:\n");
    int rows_a = get_valid_int("Rows (1-100): ", 1, MAX_DIM);
    int cols_a = get_valid_int("Columns (1-100): ", 1, MAX_DIM);
    
    printf("\nMatrix B dimensions:\n");
    int rows_b = get_valid_int("Rows (1-100): ", 1, MAX_DIM);
    int cols_b = get_valid_int("Columns (1-100): ", 1, MAX_DIM);
    
    if (cols_a != rows_b) {
        printf("Error: Matrix A columns (%d) must equal Matrix B rows (%d) for multiplication.\n", cols_a, rows_b);
        return 1;
    }
    
    if (!allocate_matrix(&mat_a, rows_a, cols_a)) {
        printf("Error: Failed to allocate memory for matrix A.\n");
        return 1;
    }
    
    if (!allocate_matrix(&mat_b, rows_b, cols_b)) {
        printf("Error: Failed to allocate memory for matrix B.\n");
        free_matrix(&mat_a);
        return 1;
    }
    
    initialize_matrix(&mat_a);
    initialize_matrix(&mat_b);
    
    printf("\nMatrix A:\n");
    print_matrix(&mat_a);
    
    printf("\nMatrix B:\n");
    print_matrix(&mat_b);
    
    if (multiply_matrices(&mat_a, &mat_b, &result)) {
        printf("\nResult (A × B):\n");
        print_matrix(&result);
        free_matrix(&result);
    } else {
        printf("Error: Matrix multiplication failed.\n");
    }
    
    free_matrix(&mat_a);
    free_matrix(&mat_b);
    
    return 0;
}