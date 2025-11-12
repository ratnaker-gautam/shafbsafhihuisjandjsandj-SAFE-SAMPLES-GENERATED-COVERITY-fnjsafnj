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

int read_int(const char* prompt, int min, int max) {
    int value;
    char buffer[100];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        
        if (sscanf(buffer, "%d", &value) == 1) {
            if (value >= min && value <= max) {
                return value;
            }
        }
        printf("Invalid input. Enter a number between %d and %d.\n", min, max);
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
    printf("Enter values for matrix %s (%dx%d):\n", name, mat->rows, mat->cols);
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            char prompt[50];
            snprintf(prompt, sizeof(prompt), "  [%d][%d]: ", i, j);
            mat->data[i][j] = read_int(prompt, INT_MIN, INT_MAX);
        }
    }
}

void print_matrix(const struct Matrix* mat, const char* name) {
    printf("Matrix %s (%dx%d):\n", name, mat->rows, mat->cols);
    for (int i = 0; i < mat->rows; i++) {
        printf("  ");
        for (int j = 0; j < mat->cols; j++) {
            printf("%6d ", mat->data[i][j]);
        }
        printf("\n");
    }
}

int multiply_matrices(const struct Matrix* a, const struct Matrix* b, struct Matrix* result) {
    if (a->cols != b->rows) {
        return 0;
    }
    
    result->rows = a->rows;
    result->cols = b->cols;
    initialize_matrix(result);
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            long long sum = 0;
            for (int k = 0; k < a->cols; k++) {
                long long product = (long long)a->data[i][k] * (long long)b->data[k][j];
                if (product > 0 && sum > LLONG_MAX - product) {
                    return -1;
                }
                if (product < 0 && sum < LLONG_MIN - product) {
                    return -1;
                }
                sum += product;
            }
            if (sum > INT_MAX || sum < INT_MIN) {
                return -1;
            }
            result->data[i][j] = (int)sum;
        }
    }
    return 1;
}

struct Matrix* create_matrix_on_heap(int rows, int cols) {
    if (!validate_dimension(rows) || !validate_dimension(cols)) {
        return NULL;
    }
    
    struct Matrix* mat = malloc(sizeof(struct Matrix));
    if (mat == NULL) {
        return NULL;
    }
    
    mat->rows = rows;
    mat->cols = cols;
    initialize_matrix(mat);
    return mat;
}

void free_matrix(struct Matrix* mat) {
    free(mat);
}

int main(void) {
    struct Matrix stack_matrix;
    struct Matrix* heap_matrix = NULL;
    struct Matrix result_matrix;
    
    printf("Matrix Multiplication Calculator\n");
    printf("================================\n");
    
    int rows1 = read_int("Enter rows for first matrix (1-100): ", 1, MAX_DIM);
    int cols1 = read_int("Enter columns for first matrix (1-100): ", 1, MAX_DIM);
    
    stack_matrix.rows = rows1;
    stack_matrix.cols = cols1;
    input_matrix(&stack_matrix, "A");
    
    int rows2 = read_int("Enter rows for second matrix (1-100): ", 1, MAX_DIM);
    int cols2 = read_int("Enter columns for second matrix (1-100): ", 1, MAX_DIM);
    
    heap_matrix = create_matrix_on_heap(rows2, cols2);
    if (heap_matrix == NULL) {
        printf("Error: Failed to allocate matrix on heap.\n");
        return 1;
    }
    
    input_matrix(heap_matrix, "B");
    
    printf("\n");
    print_matrix(&stack_matrix, "A");
    print_matrix(heap_matrix, "B");
    
    int multiply_result = multiply_matrices(&stack_matrix, heap_matrix, &result_matrix);
    
    if (multiply_result == 1) {
        printf("\nMatrix multiplication successful!\n");
        print_matrix(&result_matrix, "A x B");
    } else if (multiply_result == -1) {
        printf("\nError: Integer overflow during multiplication.\n");
    }