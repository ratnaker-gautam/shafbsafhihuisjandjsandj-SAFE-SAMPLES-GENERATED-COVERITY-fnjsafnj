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
    int rows, cols;
    
    if (scanf("%d %d", &rows, &cols) != 2) {
        return 0;
    }
    
    if (!validate_matrix_dimensions(rows, cols)) {
        return 0;
    }
    
    mat->rows = rows;
    mat->cols = cols;
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (scanf("%lf", &mat->data[i][j]) != 1) {
                return 0;
            }
        }
    }
    return 1;
}

void print_matrix(const Matrix *mat) {
    printf("%d %d\n", mat->rows, mat->cols);
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%.2f", mat->data[i][j]);
            if (j < mat->cols - 1) {
                printf(" ");
            }
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
            double sum = 0.0;
            for (int k = 0; k < a->cols; k++) {
                sum += a->data[i][k] * b->data[k][j];
            }
            result->data[i][j] = sum;
        }
    }
    return 1;
}

int transpose_matrix(const Matrix *input, Matrix *result) {
    if (!validate_matrix_dimensions(input->cols, input->rows)) {
        return 0;
    }
    
    result->rows = input->cols;
    result->cols = input->rows;
    
    for (int i = 0; i < input->rows; i++) {
        for (int j = 0; j < input->cols; j++) {
            result->data[j][i] = input->data[i][j];
        }
    }
    return 1;
}

double compute_determinant(const Matrix *mat) {
    if (mat->rows != mat->cols) {
        return 0.0;
    }
    
    int n = mat->rows;
    if (n == 1) {
        return mat->data[0][0];
    }
    
    if (n == 2) {
        return mat->data[0][0] * mat->data[1][1] - mat->data[0][1] * mat->data[1][0];
    }
    
    double det = 0.0;
    Matrix submat;
    submat.rows = n - 1;
    submat.cols = n - 1;
    
    for (int x = 0; x < n; x++) {
        int subi = 0;
        for (int i = 1; i < n; i++) {
            int subj = 0;
            for (int j = 0; j < n; j++) {
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

int main(void) {
    Matrix mat1, mat2, result;
    
    printf("Enter first matrix (rows cols then elements):\n");
    if (!read_matrix(&mat1)) {
        printf("Invalid first matrix input\n");
        return 1;
    }
    
    printf("Enter second matrix (rows cols then elements):\n");
    if (!read_matrix(&mat2)) {
        printf("Invalid second matrix input\n");
        return 1;
    }
    
    printf("\nMatrix multiplication:\n");
    if (multiply_matrices(&mat1, &mat2, &result)) {
        print_matrix(&result);
    } else {
        printf("Cannot multiply matrices\n");
    }
    
    printf("\nFirst matrix transpose:\n");
    if (transpose_matrix(&mat1, &result)) {
        print_matrix(&result);
    } else {
        printf("Cannot transpose matrix\n");
    }
    
    printf("\nSecond matrix transpose:\n");
    if (transpose_matrix(&mat2,