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
            double value = sin((double)(i + j) * 0.1) * 10.0;
            mat->data[i * mat->cols + j] = value;
        }
    }
    return 1;
}

double matrix_determinant_2x2(struct Matrix *mat) {
    if (mat == NULL || mat->data == NULL || mat->rows != 2 || mat->cols != 2) {
        return 0.0;
    }
    
    return mat->data[0] * mat->data[3] - mat->data[1] * mat->data[2];
}

void print_matrix(struct Matrix *mat) {
    if (mat == NULL || mat->data == NULL) {
        return;
    }
    
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%8.3f ", mat->data[i * mat->cols + j]);
        }
        printf("\n");
    }
}

int main() {
    int rows, cols;
    
    printf("Enter matrix rows (1-%d): ", MAX_SIZE);
    if (scanf("%d", &rows) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    printf("Enter matrix columns (1-%d): ", MAX_SIZE);
    if (scanf("%d", &cols) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (!validate_matrix_size(rows, cols)) {
        printf("Invalid matrix size\n");
        return 1;
    }
    
    struct Matrix *mat = create_matrix(rows, cols);
    if (mat == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }
    
    if (!initialize_matrix(mat)) {
        printf("Matrix initialization failed\n");
        destroy_matrix(mat);
        return 1;
    }
    
    printf("Generated matrix:\n");
    print_matrix(mat);
    
    if (rows == 2 && cols == 2) {
        double det = matrix_determinant_2x2(mat);
        printf("2x2 matrix determinant: %.3f\n", det);
    } else {
        printf("Matrix is not 2x2, skipping determinant calculation\n");
    }
    
    double stack_array[MAX_SIZE];
    int valid_size = (rows * cols <= MAX_SIZE) ? rows * cols : MAX_SIZE;
    
    for (int i = 0; i < valid_size; i++) {
        stack_array[i] = mat->data[i] * 2.0;
    }
    
    printf("First %d elements doubled (stack):\n", valid_size);
    for (int i = 0; i < valid_size; i++) {
        printf("%8.3f ", stack_array[i]);
        if ((i + 1) % cols == 0) {
            printf("\n");
        }
    }
    
    destroy_matrix(mat);
    return 0;
}