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
    printf("Enter matrix dimensions (rows cols): ");
    if (scanf("%d %d", &mat->rows, &mat->cols) != 2) return 0;
    if (!validate_matrix_dimensions(mat->rows, mat->cols)) return 0;
    
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            if (scanf("%lf", &mat->data[i][j]) != 1) return 0;
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

int multiply_matrices(const Matrix *a, const Matrix *b, Matrix *result) {
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

int transpose_matrix(const Matrix *input, Matrix *result) {
    if (!validate_matrix_dimensions(input->cols, input->rows)) return 0;
    
    result->rows = input->cols;
    result->cols = input->rows;
    
    for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->cols; j++) {
            result->data[i][j] = input->data[j][i];
        }
    }
    return 1;
}

double compute_determinant(const Matrix *mat) {
    if (mat->rows != mat->cols) return 0.0;
    if (mat->rows == 1) return mat->data[0][0];
    if (mat->rows == 2) {
        return mat->data[0][0] * mat->data[1][1] - mat->data[0][1] * mat->data[1][0];
    }
    
    double det = 0.0;
    Matrix submat;
    submat.rows = mat->rows - 1;
    submat.cols = mat->cols - 1;
    
    for (int x = 0; x < mat->cols; x++) {
        int subi = 0;
        for (int i = 1; i < mat->rows; i++) {
            int subj = 0;
            for (int j = 0; j < mat->cols; j++) {
                if (j == x) continue;
                submat.data[subi][subj] = mat->data[i][j];
                subj++;
            }
            subi++;
        }
        double sign = (x % 2 == 0) ? 1.0 : -1.0;
        det += sign * mat->data[0][x] * compute_determinant(&submat);
    }
    return det;
}

int main() {
    Matrix a, b, result;
    
    printf("Matrix Operations Program\n");
    printf("Maximum matrix dimension: %d\n", MAX_DIM);
    
    printf("\nEnter first matrix:\n");
    if (!read_matrix(&a)) {
        printf("Error: Invalid first matrix input\n");
        return 1;
    }
    
    printf("\nEnter second matrix:\n");
    if (!read_matrix(&b)) {
        printf("Error: Invalid second matrix input\n");
        return 1;
    }
    
    printf("\nFirst matrix:\n");
    print_matrix(&a);
    
    printf("\nSecond matrix:\n");
    print_matrix(&b);
    
    printf("\nMatrix multiplication:\n");
    if (multiply_matrices(&a, &b, &result)) {
        print_matrix(&result);
    } else {
        printf("Error: Cannot multiply matrices\n");
    }
    
    printf("\nTranspose of first matrix:\n");
    if (transpose_matrix(&a, &result)) {
        print_matrix(&result);
    } else {
        printf("Error: Cannot transpose matrix\n");
    }
    
    printf("\nDeterminant of first matrix: ");
    if (a.rows == a.cols) {
        double det = compute_determinant