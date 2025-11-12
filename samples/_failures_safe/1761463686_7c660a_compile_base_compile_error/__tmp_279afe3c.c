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
    if (a->cols != b->rows) return 0;
    
    if (!allocate_matrix(result, a->rows, b->cols)) {
        return 0;
    }
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                if (a->data[i][k] > 0 && b->data[k][j] > 0) {
                    if (a->data[i][k] > INT_MAX / b->data[k][j]) {
                        free_matrix(result);
                        return 0;
                    }
                    int product = a->data[i][k] * b->data[k][j];
                    if (sum > INT_MAX - product) {
                        free_matrix(result);
                        return 0;
                    }
                    sum += product;
                } else if (a->data[i][k] < 0 && b->data[k][j] < 0) {
                    if (a->data[i][k] < INT_MIN / b->data[k][j]) {
                        free_matrix(result);
                        return 0;
                    }
                    int product = a->data[i][k] * b->data[k][j];
                    if (sum < INT_MIN - product) {
                        free_matrix(result);
                        return 0;
                    }
                    sum += product;
                } else {
                    int product = a->data[i][k] * b->data[k][j];
                    if ((product > 0 && sum > INT_MAX - product) ||
                        (product < 0 && sum < INT_MIN - product)) {
                        free_matrix(result);
                        return 0;
                    }
                    sum += product;
                }
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
            return -1;
        }
        
        char *endptr;
        long val = strtol(buffer, &endptr, 10);
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (val < min || val > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        
        value = (int)val;
        break;
    }
    return value;
}

int main(void) {
    struct Matrix mat1, mat2, result;
    int rows1, cols1, rows2, cols2;
    
    printf("Matrix Multiplication Program\n");
    printf("=============================\n\n");
    
    printf("Enter dimensions for first matrix:\n");
    rows1 = get_valid_int("Rows (1-100): ", 1, MAX_DIM);
    cols1 = get_valid_int("Columns (1-100): ", 1, MAX_DIM);
    
    printf("\nEnter dimensions for second matrix:\n");
    rows2 = get_valid_int("Rows (1-100): ", 1, MAX_DIM);
    cols2 = get_valid_int("Columns (