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
    if (rows <= 0 || cols <= 0) {
        return 0;
    }
    if (rows > MAX_DIM || cols > MAX_DIM) {
        return 0;
    }
    return 1;
}

int read_matrix(Matrix *mat) {
    char input[256];
    int rows, cols;
    
    printf("Enter number of rows: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 0;
    }
    if (sscanf(input, "%d", &rows) != 1) {
        return 0;
    }
    
    printf("Enter number of columns: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 0;
    }
    if (sscanf(input, "%d", &cols) != 1) {
        return 0;
    }
    
    if (!validate_matrix_dimensions(rows, cols)) {
        return 0;
    }
    
    mat->rows = rows;
    mat->cols = cols;
    
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("Element [%d][%d]: ", i, j);
            if (fgets(input, sizeof(input), stdin) == NULL) {
                return 0;
            }
            if (sscanf(input, "%lf", &mat->data[i][j]) != 1) {
                return 0;
            }
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
    if (a->cols != b->rows) {
        return 0;
    }
    if (!validate_matrix_dimensions(a->rows, b->cols)) {
        return 0;
    }
    
    result->rows = a->rows;
    result->cols = b->cols;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            result->data[i][j] = 0.0;
            for (int k = 0; k < a->cols; k++) {
                result->data[i][j] += a->data[i][k] * b->data[k][j];
            }
        }
    }
    return 1;
}

void transpose_matrix(const Matrix *input, Matrix *result) {
    result->rows = input->cols;
    result->cols = input->rows;
    
    for (int i = 0; i < input->rows; i++) {
        for (int j = 0; j < input->cols; j++) {
            result->data[j][i] = input->data[i][j];
        }
    }
}

double compute_determinant(const Matrix *mat) {
    if (mat->rows != mat->cols) {
        return 0.0;
    }
    if (mat->rows == 1) {
        return mat->data[0][0];
    }
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
    Matrix mat1, mat2, result;
    
    printf("Matrix Operations Program\n");
    printf("Enter first matrix:\n");
    if (!read_matrix(&mat1)) {
        printf("Error: Invalid input for first matrix\n");
        return 1;
    }
    
    printf("Enter second matrix:\n");
    if (!read_matrix(&mat2)) {
        printf("Error: Invalid input for second matrix\n");
        return 1;
    }
    
    printf("\nFirst matrix:\n");
    print_matrix(&mat1);
    
    printf("\nSecond matrix