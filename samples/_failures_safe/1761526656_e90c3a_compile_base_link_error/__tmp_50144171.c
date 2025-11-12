//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_SIZE 100

struct Matrix {
    double *data;
    int rows;
    int cols;
};

int validate_matrix_params(int rows, int cols) {
    if (rows <= 0 || cols <= 0) return 0;
    if (rows > MAX_SIZE || cols > MAX_SIZE) return 0;
    return 1;
}

struct Matrix* create_matrix(int rows, int cols) {
    if (!validate_matrix_params(rows, cols)) return NULL;
    
    struct Matrix *mat = malloc(sizeof(struct Matrix));
    if (!mat) return NULL;
    
    mat->rows = rows;
    mat->cols = cols;
    mat->data = malloc(rows * cols * sizeof(double));
    if (!mat->data) {
        free(mat);
        return NULL;
    }
    
    return mat;
}

void destroy_matrix(struct Matrix *mat) {
    if (mat) {
        free(mat->data);
        free(mat);
    }
}

int fill_matrix_identity(struct Matrix *mat) {
    if (!mat || !mat->data) return 0;
    
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            if (i == j) {
                mat->data[i * mat->cols + j] = 1.0;
            } else {
                mat->data[i * mat->cols + j] = 0.0;
            }
        }
    }
    return 1;
}

double compute_frobenius_norm(const struct Matrix *mat) {
    if (!mat || !mat->data) return -1.0;
    
    double sum = 0.0;
    for (int i = 0; i < mat->rows * mat->cols; i++) {
        sum += mat->data[i] * mat->data[i];
    }
    return sqrt(sum);
}

int read_matrix_size(int *rows, int *cols) {
    printf("Enter matrix dimensions (rows cols, max %d each): ", MAX_SIZE);
    if (scanf("%d %d", rows, cols) != 2) return 0;
    if (!validate_matrix_params(*rows, *cols)) return 0;
    return 1;
}

int main(void) {
    int rows, cols;
    
    if (!read_matrix_size(&rows, &cols)) {
        printf("Invalid matrix dimensions\n");
        return 1;
    }
    
    struct Matrix *mat = create_matrix(rows, cols);
    if (!mat) {
        printf("Failed to create matrix\n");
        return 1;
    }
    
    if (!fill_matrix_identity(mat)) {
        printf("Failed to fill matrix\n");
        destroy_matrix(mat);
        return 1;
    }
    
    double stack_buffer[MAX_SIZE];
    int valid_size = (rows < MAX_SIZE && cols < MAX_SIZE) ? rows * cols : 0;
    
    if (valid_size > 0) {
        for (int i = 0; i < valid_size; i++) {
            stack_buffer[i] = mat->data[i];
        }
    }
    
    double norm = compute_frobenius_norm(mat);
    if (norm < 0) {
        printf("Failed to compute norm\n");
        destroy_matrix(mat);
        return 1;
    }
    
    printf("Frobenius norm of %dx%d identity matrix: %.6f\n", rows, cols, norm);
    
    if (valid_size > 0) {
        double stack_sum = 0.0;
        for (int i = 0; i < valid_size; i++) {
            stack_sum += stack_buffer[i];
        }
        printf("Sum of first %d elements (from stack): %.6f\n", valid_size, stack_sum);
    }
    
    destroy_matrix(mat);
    return 0;
}