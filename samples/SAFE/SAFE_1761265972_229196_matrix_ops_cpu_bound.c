//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_SIZE 100

typedef struct {
    int rows;
    int cols;
    double data[MAX_SIZE][MAX_SIZE];
} Matrix;

int validate_matrix_size(int rows, int cols) {
    return (rows > 0 && rows <= MAX_SIZE && cols > 0 && cols <= MAX_SIZE);
}

void initialize_matrix(Matrix *mat, int rows, int cols) {
    if (!validate_matrix_size(rows, cols)) {
        return;
    }
    mat->rows = rows;
    mat->cols = cols;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            mat->data[i][j] = (double)(i * cols + j + 1);
        }
    }
}

void print_matrix(const Matrix *mat) {
    if (mat == NULL) return;
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%8.2f", mat->data[i][j]);
        }
        printf("\n");
    }
}

int matrix_multiply(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a == NULL || b == NULL || result == NULL) return 0;
    if (a->cols != b->rows) return 0;
    if (!validate_matrix_size(a->rows, b->cols)) return 0;
    
    result->rows = a->rows;
    result->cols = b->cols;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            double sum = 0.0;
            for (int k = 0; k < a->cols; k++) {
                sum += a->data[i][k] * b->data[k][j];
            }
            result->data[i][j] = sum;
        }
    }
    return 1;
}

double matrix_trace(const Matrix *mat) {
    if (mat == NULL) return 0.0;
    if (mat->rows != mat->cols) return 0.0;
    
    double trace = 0.0;
    for (int i = 0; i < mat->rows; i++) {
        trace += mat->data[i][i];
    }
    return trace;
}

int main() {
    Matrix mat1, mat2, result;
    int size;
    
    printf("Enter matrix size (1-%d): ", MAX_SIZE);
    if (scanf("%d", &size) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (!validate_matrix_size(size, size)) {
        printf("Invalid matrix size\n");
        return 1;
    }
    
    initialize_matrix(&mat1, size, size);
    initialize_matrix(&mat2, size, size);
    
    printf("Matrix 1:\n");
    print_matrix(&mat1);
    printf("\nMatrix 2:\n");
    print_matrix(&mat2);
    
    if (matrix_multiply(&mat1, &mat2, &result)) {
        printf("\nMatrix multiplication result:\n");
        print_matrix(&result);
        
        double trace = matrix_trace(&result);
        printf("\nTrace of result matrix: %.2f\n", trace);
    } else {
        printf("Matrix multiplication failed\n");
    }
    
    return 0;
}