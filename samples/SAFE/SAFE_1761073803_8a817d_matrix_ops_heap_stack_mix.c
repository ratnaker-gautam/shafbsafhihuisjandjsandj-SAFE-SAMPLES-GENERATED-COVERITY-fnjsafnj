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
            mat->data[i][j] = 0;
        }
    }
}

void print_matrix(const struct Matrix* mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%6d", mat->data[i][j]);
        }
        printf("\n");
    }
}

void input_matrix(struct Matrix* mat) {
    printf("Enter matrix elements:\n");
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            char prompt[50];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            mat->data[i][j] = read_int(prompt, INT_MIN, INT_MAX);
        }
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
                if (product > INT_MAX - sum || product < INT_MIN - sum) {
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
    struct Matrix mat1, mat2;
    
    printf("Matrix Multiplication Program\n");
    printf("Maximum dimension: %d\n\n", MAX_DIM);
    
    mat1.rows = read_int("Enter rows for matrix 1: ", 1, MAX_DIM);
    mat1.cols = read_int("Enter columns for matrix 1: ", 1, MAX_DIM);
    
    printf("\n");
    mat2.rows = read_int("Enter rows for matrix 2: ", 1, MAX_DIM);
    mat2.cols = read_int("Enter columns for matrix 2: ", 1, MAX_DIM);
    
    if (mat1.cols != mat2.rows) {
        printf("Error: Matrix 1 columns must equal matrix 2 rows for multiplication.\n");
        return 1;
    }
    
    printf("\n");
    input_matrix(&mat1);
    
    printf("\n");
    input_matrix(&mat2);
    
    printf("\nMatrix 1:\n");
    print_matrix(&mat1);
    
    printf("\nMatrix 2:\n");
    print_matrix(&mat2);
    
    struct Matrix* result = multiply_matrices(&mat1, &mat2);
    if (result == NULL) {
        printf("\nError: Matrix multiplication failed (overflow or memory error).\n");
        return 1;
    }
    
    printf("\nResult of multiplication:\n");
    print_matrix(result);
    
    free(result);
    return 0;
}