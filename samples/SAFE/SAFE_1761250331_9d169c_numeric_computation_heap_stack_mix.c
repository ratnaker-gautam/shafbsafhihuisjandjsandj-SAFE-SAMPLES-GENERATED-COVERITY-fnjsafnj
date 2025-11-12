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

int validate_matrix_size(int rows, int cols) {
    return (rows > 0 && rows <= MAX_SIZE && cols > 0 && cols <= MAX_SIZE);
}

struct Matrix* create_matrix(int rows, int cols) {
    if (!validate_matrix_size(rows, cols)) {
        return NULL;
    }
    
    struct Matrix *mat = malloc(sizeof(struct Matrix));
    if (mat == NULL) {
        return NULL;
    }
    
    mat->data = malloc(rows * cols * sizeof(double));
    if (mat->data == NULL) {
        free(mat);
        return NULL;
    }
    
    mat->rows = rows;
    mat->cols = cols;
    return mat;
}

void destroy_matrix(struct Matrix *mat) {
    if (mat != NULL) {
        free(mat->data);
        free(mat);
    }
}

int fill_matrix(struct Matrix *mat) {
    if (mat == NULL || mat->data == NULL) {
        return 0;
    }
    
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            mat->data[i * mat->cols + j] = (i + 1) * (j + 1) * 0.5;
        }
    }
    return 1;
}

double compute_determinant_2x2(struct Matrix *mat) {
    if (mat == NULL || mat->data == NULL || mat->rows != 2 || mat->cols != 2) {
        return 0.0;
    }
    
    return mat->data[0] * mat->data[3] - mat->data[1] * mat->data[2];
}

double compute_matrix_trace(struct Matrix *mat) {
    if (mat == NULL || mat->data == NULL || mat->rows != mat->cols) {
        return 0.0;
    }
    
    double trace = 0.0;
    for (int i = 0; i < mat->rows; i++) {
        trace += mat->data[i * mat->cols + i];
    }
    return trace;
}

void process_matrices(void) {
    int sizes[][2] = {{2, 2}, {3, 3}, {4, 4}};
    int num_matrices = sizeof(sizes) / sizeof(sizes[0]);
    
    struct Matrix *matrices[3];
    
    for (int i = 0; i < num_matrices; i++) {
        matrices[i] = create_matrix(sizes[i][0], sizes[i][1]);
        if (matrices[i] == NULL) {
            for (int j = 0; j < i; j++) {
                destroy_matrix(matrices[j]);
            }
            return;
        }
        
        if (!fill_matrix(matrices[i])) {
            for (int j = 0; j <= i; j++) {
                destroy_matrix(matrices[j]);
            }
            return;
        }
    }
    
    double stack_results[3];
    
    for (int i = 0; i < num_matrices; i++) {
        if (matrices[i]->rows == 2 && matrices[i]->cols == 2) {
            stack_results[i] = compute_determinant_2x2(matrices[i]);
        } else {
            stack_results[i] = compute_matrix_trace(matrices[i]);
        }
    }
    
    double final_result = 0.0;
    for (int i = 0; i < num_matrices; i++) {
        final_result += stack_results[i];
        printf("Matrix %d result: %.2f\n", i + 1, stack_results[i]);
    }
    
    printf("Final combined result: %.2f\n", final_result);
    
    for (int i = 0; i < num_matrices; i++) {
        destroy_matrix(matrices[i]);
    }
}

int main(void) {
    printf("Matrix Computation Program\n");
    printf("Creating and processing matrices...\n");
    
    process_matrices();
    
    printf("Program completed successfully.\n");
    return 0;
}