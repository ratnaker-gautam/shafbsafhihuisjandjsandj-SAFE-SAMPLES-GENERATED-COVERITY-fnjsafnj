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

int initialize_matrix(struct Matrix *mat) {
    if (mat == NULL || mat->data == NULL) {
        return 0;
    }
    
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            mat->data[i * mat->cols + j] = sin(i + j) * cos(i - j);
        }
    }
    return 1;
}

double compute_matrix_norm(const struct Matrix *mat) {
    if (mat == NULL || mat->data == NULL) {
        return 0.0;
    }
    
    double sum = 0.0;
    for (int i = 0; i < mat->rows * mat->cols; i++) {
        sum += mat->data[i] * mat->data[i];
    }
    return sqrt(sum);
}

void process_matrices_stack(int count) {
    if (count <= 0 || count > 5) {
        printf("Invalid count\n");
        return;
    }
    
    struct Matrix stack_mats[5];
    double norms[5];
    
    for (int i = 0; i < count; i++) {
        int size = (i + 2) * 2;
        stack_mats[i].rows = size;
        stack_mats[i].cols = size;
        double local_data[MAX_SIZE * MAX_SIZE];
        stack_mats[i].data = local_data;
        
        for (int j = 0; j < size * size; j++) {
            local_data[j] = (i + j) * 0.1;
        }
        
        norms[i] = compute_matrix_norm(&stack_mats[i]);
        printf("Stack matrix %d norm: %.6f\n", i, norms[i]);
    }
}

int main() {
    int rows, cols;
    
    printf("Enter matrix rows (1-%d): ", MAX_SIZE);
    if (scanf("%d", &rows) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    printf("Enter matrix cols (1-%d): ", MAX_SIZE);
    if (scanf("%d", &cols) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    struct Matrix *heap_mat = create_matrix(rows, cols);
    if (heap_mat == NULL) {
        printf("Failed to create matrix\n");
        return 1;
    }
    
    if (!initialize_matrix(heap_mat)) {
        printf("Failed to initialize matrix\n");
        destroy_matrix(heap_mat);
        return 1;
    }
    
    double norm = compute_matrix_norm(heap_mat);
    printf("Heap matrix norm: %.6f\n", norm);
    
    printf("\nProcessing stack matrices:\n");
    process_matrices_stack(3);
    
    destroy_matrix(heap_mat);
    return 0;
}