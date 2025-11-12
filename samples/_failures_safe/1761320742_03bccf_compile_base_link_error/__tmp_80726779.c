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

int fill_matrix(struct Matrix *mat) {
    if (!mat || !mat->data) return 0;
    
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            double value = sin(i + j) * cos(i - j);
            if (isnan(value) || isinf(value)) return 0;
            mat->data[i * mat->cols + j] = value;
        }
    }
    return 1;
}

double compute_determinant_2x2(struct Matrix *mat) {
    if (!mat || mat->rows != 2 || mat->cols != 2) return NAN;
    return mat->data[0] * mat->data[3] - mat->data[1] * mat->data[2];
}

void process_matrices(void) {
    int sizes[][2] = {{2, 2}, {3, 3}, {4, 4}};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);
    
    for (int i = 0; i < num_sizes; i++) {
        int rows = sizes[i][0];
        int cols = sizes[i][1];
        
        struct Matrix *mat = create_matrix(rows, cols);
        if (!mat) {
            printf("Failed to create %dx%d matrix\n", rows, cols);
            continue;
        }
        
        if (!fill_matrix(mat)) {
            printf("Failed to fill %dx%d matrix\n", rows, cols);
            destroy_matrix(mat);
            continue;
        }
        
        if (rows == 2 && cols == 2) {
            double det = compute_determinant_2x2(mat);
            if (!isnan(det)) {
                printf("2x2 matrix determinant: %.6f\n", det);
            }
        }
        
        double trace = 0.0;
        int min_dim = (rows < cols) ? rows : cols;
        for (int j = 0; j < min_dim; j++) {
            trace += mat->data[j * cols + j];
        }
        printf("%dx%d matrix trace: %.6f\n", rows, cols, trace);
        
        destroy_matrix(mat);
    }
}

int main(void) {
    printf("Matrix Computation Results:\n");
    process_matrices();
    
    double stack_array[MAX_SIZE];
    for (int i = 0; i < MAX_SIZE; i++) {
        stack_array[i] = sqrt(i + 1);
        if (isnan(stack_array[i]) || isinf(stack_array[i])) {
            printf("Invalid computation at index %d\n", i);
            return 1;
        }
    }
    
    double *heap_array = malloc(MAX_SIZE * sizeof(double));
    if (!heap_array) {
        printf("Memory allocation failed\n");
        return 1;
    }
    
    for (int i = 0; i < MAX_SIZE; i++) {
        heap_array[i] = stack_array[i] * 2.0;
        if (isnan(heap_array[i]) || isinf(heap_array[i])) {
            printf("Invalid heap computation at index %d\n", i);
            free(heap_array);
            return 1;
        }
    }
    
    double sum = 0.0;
    for (int i = 0; i < MAX_SIZE; i++) {
        sum += heap_array[i];
        if (isnan(sum) || isinf(sum)) {
            printf("Sum overflow at index %d\n", i);
            free(heap_array);
            return 1;
        }
    }
    
    printf("Final computed sum: %.6f\n", sum);
    free(heap_array);
    
    return 0;
}