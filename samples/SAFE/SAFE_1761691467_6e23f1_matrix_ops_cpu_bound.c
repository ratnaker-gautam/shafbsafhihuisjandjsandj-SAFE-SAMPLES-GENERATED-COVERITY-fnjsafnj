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

int validate_dimension(int dim) {
    return dim > 0 && dim <= MAX_DIM;
}

int read_matrix(Matrix *mat) {
    if (scanf("%d %d", &mat->rows, &mat->cols) != 2) {
        return 0;
    }
    if (!validate_dimension(mat->rows) || !validate_dimension(mat->cols)) {
        return 0;
    }
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
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
            printf("%.2f ", mat->data[i][j]);
        }
        printf("\n");
    }
}

int multiply_matrices(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a->cols != b->rows) {
        return 0;
    }
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

int transpose_matrix(const Matrix *mat, Matrix *result) {
    result->rows = mat->cols;
    result->cols = mat->rows;
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            result->data[j][i] = mat->data[i][j];
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

int main() {
    Matrix a, b, result;
    int operation;
    
    if (scanf("%d", &operation) != 1) {
        fprintf(stderr, "Invalid operation\n");
        return 1;
    }
    
    if (operation < 1 || operation > 3) {
        fprintf(stderr, "Invalid operation\n");
        return 1;
    }
    
    if (!read_matrix(&a)) {
        fprintf(stderr, "Invalid matrix A\n");
        return 1;
    }
    
    if (operation == 1) {
        if (!read_matrix(&b)) {
            fprintf(stderr, "Invalid matrix B\n");
            return 1;
        }
        if (!multiply_matrices(&a, &b, &result)) {
            fprintf(stderr, "Matrix multiplication failed\n");
            return 1;
        }
        print_matrix(&result);
    } else if (operation == 2) {
        if (!transpose_matrix(&a, &result)) {
            fprintf(stderr, "Matrix transpose failed\n");
            return 1;
        }
        print_matrix(&result);
    } else if (operation == 3) {
        double det = compute_determinant(&a);
        printf("%.2f\n", det);
    }
    
    return 0;
}