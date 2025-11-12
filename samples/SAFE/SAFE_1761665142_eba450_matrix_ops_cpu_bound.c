//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 100

typedef struct {
    int rows;
    int cols;
    int data[MAX_DIM][MAX_DIM];
} Matrix;

int validate_matrix_dim(int dim) {
    return dim > 0 && dim <= MAX_DIM;
}

int read_int(const char* prompt, int min, int max) {
    char buffer[32];
    int value;
    
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

void init_matrix(Matrix* mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            mat->data[i][j] = rand() % 100;
        }
    }
}

void print_matrix(const Matrix* mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%6d", mat->data[i][j]);
        }
        printf("\n");
    }
}

void multiply_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                sum += a->data[i][k] * b->data[k][j];
            }
            result->data[i][j] = sum;
        }
    }
}

void transpose_matrix(const Matrix* src, Matrix* dst) {
    for (int i = 0; i < src->rows; i++) {
        for (int j = 0; j < src->cols; j++) {
            dst->data[j][i] = src->data[i][j];
        }
    }
}

int main() {
    Matrix a, b, result, transposed;
    
    printf("Matrix Operations Demo\n");
    printf("=====================\n\n");
    
    printf("Matrix A dimensions:\n");
    a.rows = read_int("Rows (1-100): ", 1, MAX_DIM);
    a.cols = read_int("Columns (1-100): ", 1, MAX_DIM);
    
    printf("\nMatrix B dimensions:\n");
    b.rows = read_int("Rows (1-100): ", 1, MAX_DIM);
    b.cols = read_int("Columns (1-100): ", 1, MAX_DIM);
    
    if (a.rows == -1 || a.cols == -1 || b.rows == -1 || b.cols == -1) {
        printf("Input error.\n");
        return 1;
    }
    
    srand(42);
    init_matrix(&a);
    init_matrix(&b);
    
    printf("\nMatrix A:\n");
    print_matrix(&a);
    
    printf("\nMatrix B:\n");
    print_matrix(&b);
    
    if (a.cols == b.rows) {
        result.rows = a.rows;
        result.cols = b.cols;
        multiply_matrices(&a, &b, &result);
        printf("\nMatrix A * B:\n");
        print_matrix(&result);
    } else {
        printf("\nCannot multiply: A columns (%d) != B rows (%d)\n", a.cols, b.rows);
    }
    
    if (a.rows <= MAX_DIM && a.cols <= MAX_DIM) {
        transposed.rows = a.cols;
        transposed.cols = a.rows;
        transpose_matrix(&a, &transposed);
        printf("\nTranspose of A:\n");
        print_matrix(&transposed);
    }
    
    if (b.rows <= MAX_DIM && b.cols <= MAX_DIM) {
        transposed.rows = b.cols;
        transposed.cols = b.rows;
        transpose_matrix(&b, &transposed);
        printf("\nTranspose of B:\n");
        print_matrix(&transposed);
    }
    
    return 0;
}