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
            printf("Invalid input. Please enter an integer.\n");
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
            mat->data[i][j] = 0;
        }
    }
}

void input_matrix(struct Matrix* mat, const char* name) {
    printf("Enter values for matrix %s (%d x %d):\n", name, mat->rows, mat->cols);
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            char prompt[50];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            mat->data[i][j] = read_int(prompt, INT_MIN, INT_MAX);
        }
    }
}

void print_matrix(const struct Matrix* mat, const char* name) {
    printf("Matrix %s (%d x %d):\n", name, mat->rows, mat->cols);
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%6d ", mat->data[i][j]);
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
    initialize_matrix(result);
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            long long sum = 0;
            for (int k = 0; k < a->cols; k++) {
                long long product = (long long)a->data[i][k] * (long long)b->data[k][j];
                if (product > INT_MAX - sum || product < INT_MIN + sum) {
                    free(result);
                    return NULL;
                }
                sum += product;
            }
            if (sum > INT_MAX || sum < INT_MIN) {
                free(result);
                return NULL;
            }
            result->data[i][j] = (int)sum;
        }
    }
    
    return result;
}

int main(void) {
    struct Matrix mat_a, mat_b;
    
    printf("Matrix Multiplication Program\n");
    printf("Maximum dimension: %d\n\n", MAX_DIM);
    
    mat_a.rows = read_int("Enter rows for matrix A: ", 1, MAX_DIM);
    mat_a.cols = read_int("Enter columns for matrix A: ", 1, MAX_DIM);
    
    mat_b.rows = read_int("Enter rows for matrix B: ", 1, MAX_DIM);
    mat_b.cols = read_int("Enter columns for matrix B: ", 1, MAX_DIM);
    
    if (!validate_dimension(mat_a.rows) || !validate_dimension(mat_a.cols) ||
        !validate_dimension(mat_b.rows) || !validate_dimension(mat_b.cols)) {
        printf("Invalid matrix dimensions.\n");
        return 1;
    }
    
    input_matrix(&mat_a, "A");
    input_matrix(&mat_b, "B");
    
    printf("\n");
    print_matrix(&mat_a, "A");
    printf("\n");
    print_matrix(&mat_b, "B");
    printf("\n");
    
    struct Matrix* result = multiply_matrices(&mat_a, &mat_b);
    if (result == NULL) {
        printf("Matrix multiplication failed. Check dimensions and integer overflow.\n");
        return 1;
    }
    
    print_matrix(result, "A x B");
    
    free(result);
    return 0;
}