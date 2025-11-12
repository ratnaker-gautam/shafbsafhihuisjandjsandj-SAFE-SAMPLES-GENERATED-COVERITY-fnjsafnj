//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: heap_stack_mix ; Variation: numeric_computation
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

int initialize_matrix(struct Matrix *mat) {
    if (!mat || !mat->data) return 0;
    
    for (int i = 0; i < mat->rows * mat->cols; i++) {
        mat->data[i] = (double)(i % 10) + 1.0;
    }
    return 1;
}

double compute_determinant_2x2(struct Matrix *mat) {
    if (!mat || mat->rows != 2 || mat->cols != 2) return 0.0;
    return mat->data[0] * mat->data[3] - mat->data[1] * mat->data[2];
}

double compute_matrix_trace(struct Matrix *mat) {
    if (!mat || mat->rows != mat->cols) return 0.0;
    
    double trace = 0.0;
    for (int i = 0; i < mat->rows; i++) {
        trace += mat->data[i * mat->cols + i];
    }
    return trace;
}

void print_matrix(struct Matrix *mat) {
    if (!mat || !mat->data) return;
    
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%8.2f", mat->data[i * mat->cols + j]);
        }
        printf("\n");
    }
}

int main() {
    int choice;
    printf("Matrix Operations:\n");
    printf("1. 2x2 Matrix Determinant\n");
    printf("2. Square Matrix Trace\n");
    printf("Enter choice (1-2): ");
    
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (choice < 1 || choice > 2) {
        printf("Invalid choice\n");
        return 1;
    }
    
    if (choice == 1) {
        struct Matrix *mat2x2 = create_matrix(2, 2);
        if (!mat2x2) {
            printf("Matrix creation failed\n");
            return 1;
        }
        
        if (!initialize_matrix(mat2x2)) {
            printf("Matrix initialization failed\n");
            destroy_matrix(mat2x2);
            return 1;
        }
        
        printf("2x2 Matrix:\n");
        print_matrix(mat2x2);
        
        double det = compute_determinant_2x2(mat2x2);
        printf("Determinant: %.2f\n", det);
        
        destroy_matrix(mat2x2);
    } else {
        int size;
        printf("Enter square matrix size (1-%d): ", MAX_SIZE);
        if (scanf("%d", &size) != 1) {
            printf("Invalid input\n");
            return 1;
        }
        
        if (size <= 0 || size > MAX_SIZE) {
            printf("Invalid size\n");
            return 1;
        }
        
        struct Matrix *mat = create_matrix(size, size);
        if (!mat) {
            printf("Matrix creation failed\n");
            return 1;
        }
        
        if (!initialize_matrix(mat)) {
            printf("Matrix initialization failed\n");
            destroy_matrix(mat);
            return 1;
        }
        
        printf("%dx%d Matrix:\n", size, size);
        print_matrix(mat);
        
        double trace = compute_matrix_trace(mat);
        printf("Trace: %.2f\n", trace);
        
        destroy_matrix(mat);
    }
    
    return 0;
}