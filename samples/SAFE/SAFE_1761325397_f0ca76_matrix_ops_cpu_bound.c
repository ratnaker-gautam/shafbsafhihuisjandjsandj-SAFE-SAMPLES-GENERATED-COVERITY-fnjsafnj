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
        fprintf(stderr, "Invalid matrix dimensions\n");
        exit(1);
    }
    mat->rows = rows;
    mat->cols = cols;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            mat->data[i][j] = (double)(i * cols + j + 1);
        }
    }
}

void matrix_multiply(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a->cols != b->rows) {
        fprintf(stderr, "Matrix dimensions incompatible for multiplication\n");
        exit(1);
    }
    if (!validate_matrix_size(a->rows, b->cols)) {
        fprintf(stderr, "Result matrix would exceed maximum size\n");
        exit(1);
    }
    
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
}

void print_matrix(const Matrix *mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%8.2f", mat->data[i][j]);
        }
        printf("\n");
    }
}

int main() {
    Matrix mat1, mat2, result;
    
    int rows1, cols1, rows2, cols2;
    
    printf("Enter dimensions for first matrix (rows cols): ");
    if (scanf("%d %d", &rows1, &cols1) != 2) {
        fprintf(stderr, "Invalid input for matrix dimensions\n");
        return 1;
    }
    
    printf("Enter dimensions for second matrix (rows cols): ");
    if (scanf("%d %d", &rows2, &cols2) != 2) {
        fprintf(stderr, "Invalid input for matrix dimensions\n");
        return 1;
    }
    
    if (!validate_matrix_size(rows1, cols1) || !validate_matrix_size(rows2, cols2)) {
        fprintf(stderr, "Matrix dimensions out of valid range (1-%d)\n", MAX_SIZE);
        return 1;
    }
    
    if (cols1 != rows2) {
        fprintf(stderr, "Matrix dimensions incompatible for multiplication\n");
        return 1;
    }
    
    initialize_matrix(&mat1, rows1, cols1);
    initialize_matrix(&mat2, rows2, cols2);
    
    printf("\nFirst matrix:\n");
    print_matrix(&mat1);
    
    printf("\nSecond matrix:\n");
    print_matrix(&mat2);
    
    matrix_multiply(&mat1, &mat2, &result);
    
    printf("\nResult of multiplication:\n");
    print_matrix(&result);
    
    clock_t start = clock();
    
    for (int iter = 0; iter < 1000; iter++) {
        matrix_multiply(&mat1, &mat2, &result);
    }
    
    clock_t end = clock();
    double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    
    printf("\nCPU time for 1000 multiplications: %.3f seconds\n", cpu_time_used);
    
    return 0;
}