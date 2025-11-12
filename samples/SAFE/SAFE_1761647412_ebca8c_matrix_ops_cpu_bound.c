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
    double data[MAX_DIM][MAX_DIM];
} Matrix;

int validate_matrix_dimensions(int rows, int cols) {
    if (rows <= 0 || cols <= 0) return 0;
    if (rows > MAX_DIM || cols > MAX_DIM) return 0;
    return 1;
}

int read_matrix(Matrix *mat) {
    char input[256];
    printf("Enter number of rows: ");
    if (fgets(input, sizeof(input), stdin) == NULL) return 0;
    if (sscanf(input, "%d", &mat->rows) != 1) return 0;
    
    printf("Enter number of columns: ");
    if (fgets(input, sizeof(input), stdin) == NULL) return 0;
    if (sscanf(input, "%d", &mat->cols) != 1) return 0;
    
    if (!validate_matrix_dimensions(mat->rows, mat->cols)) return 0;
    
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("Element [%d][%d]: ", i, j);
            if (fgets(input, sizeof(input), stdin) == NULL) return 0;
            if (sscanf(input, "%lf", &mat->data[i][j]) != 1) return 0;
        }
    }
    return 1;
}

void print_matrix(const Matrix *mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%8.2f ", mat->data[i][j]);
        }
        printf("\n");
    }
}

int matrix_multiply(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a->cols != b->rows) return 0;
    if (!validate_matrix_dimensions(a->rows, b->cols)) return 0;
    
    result->rows = a->rows;
    result->cols = b->cols;
    
    for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->cols; j++) {
            result->data[i][j] = 0.0;
            for (int k = 0; k < a->cols; k++) {
                result->data[i][j] += a->data[i][k] * b->data[k][j];
            }
        }
    }
    return 1;
}

int matrix_transpose(const Matrix *src, Matrix *dst) {
    if (!validate_matrix_dimensions(src->cols, src->rows)) return 0;
    
    dst->rows = src->cols;
    dst->cols = src->rows;
    
    for (int i = 0; i < src->rows; i++) {
        for (int j = 0; j < src->cols; j++) {
            dst->data[j][i] = src->data[i][j];
        }
    }
    return 1;
}

double matrix_trace(const Matrix *mat) {
    if (mat->rows != mat->cols) return 0.0;
    
    double trace = 0.0;
    for (int i = 0; i < mat->rows; i++) {
        trace += mat->data[i][i];
    }
    return trace;
}

int main() {
    Matrix a, b, result;
    
    printf("Matrix Operations Program\n");
    printf("Enter first matrix:\n");
    if (!read_matrix(&a)) {
        printf("Error: Invalid first matrix input\n");
        return 1;
    }
    
    printf("Enter second matrix:\n");
    if (!read_matrix(&b)) {
        printf("Error: Invalid second matrix input\n");
        return 1;
    }
    
    printf("\nFirst matrix:\n");
    print_matrix(&a);
    
    printf("\nSecond matrix:\n");
    print_matrix(&b);
    
    printf("\nMatrix multiplication:\n");
    if (matrix_multiply(&a, &b, &result)) {
        print_matrix(&result);
    } else {
        printf("Multiplication not possible\n");
    }
    
    printf("\nTranspose of first matrix:\n");
    if (matrix_transpose(&a, &result)) {
        print_matrix(&result);
    } else {
        printf("Transpose not possible\n");
    }
    
    printf("\nTrace of first matrix: ");
    double trace = matrix_trace(&a);
    if (trace != 0.0 || a.rows == a.cols) {
        printf("%.2f\n", trace);
    } else {
        printf("Not a square matrix\n");
    }
    
    printf("\nTrace of second matrix: ");
    trace = matrix_trace(&b);
    if (trace != 0.0 || b.rows == b.cols) {
        printf("%.2f\n", trace);
    } else {
        printf("Not a square matrix\n");
    }
    
    return 0;
}