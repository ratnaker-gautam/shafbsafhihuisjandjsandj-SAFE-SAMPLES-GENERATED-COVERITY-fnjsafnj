//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_DIM 100

typedef struct {
    int rows;
    int cols;
    double data[MAX_DIM][MAX_DIM];
} Matrix;

int validate_matrix_dimensions(int rows, int cols) {
    if (rows <= 0 || cols <= 0 || rows > MAX_DIM || cols > MAX_DIM) {
        return 0;
    }
    return 1;
}

int read_matrix(Matrix *mat) {
    int valid = 0;
    do {
        printf("Enter matrix dimensions (rows cols, max %d): ", MAX_DIM);
        if (scanf("%d %d", &mat->rows, &mat->cols) != 2) {
            while (getchar() != '\n');
            printf("Invalid input format.\n");
            continue;
        }
        if (!validate_matrix_dimensions(mat->rows, mat->cols)) {
            printf("Invalid dimensions.\n");
            continue;
        }
        valid = 1;
    } while (!valid);

    printf("Enter %d x %d matrix elements:\n", mat->rows, mat->cols);
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            if (scanf("%lf", &mat->data[i][j]) != 1) {
                printf("Invalid matrix element.\n");
                return 0;
            }
        }
    }
    return 1;
}

void print_matrix(const Matrix *mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%8.2f", mat->data[i][j]);
        }
        printf("\n");
    }
}

int multiply_matrices(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a->cols != b->rows) {
        printf("Matrix dimensions incompatible for multiplication.\n");
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

double matrix_determinant(const Matrix *mat) {
    if (mat->rows != mat->cols) {
        return NAN;
    }
    int n = mat->rows;
    if (n == 1) {
        return mat->data[0][0];
    }
    if (n == 2) {
        return mat->data[0][0] * mat->data[1][1] - mat->data[0][1] * mat->data[1][0];
    }
    double det = 0.0;
    for (int col = 0; col < n; col++) {
        Matrix sub;
        sub.rows = n - 1;
        sub.cols = n - 1;
        for (int i = 1; i < n; i++) {
            int subcol = 0;
            for (int j = 0; j < n; j++) {
                if (j == col) continue;
                sub.data[i-1][subcol] = mat->data[i][j];
                subcol++;
            }
        }
        double subdet = matrix_determinant(&sub);
        double sign = (col % 2 == 0) ? 1.0 : -1.0;
        det += sign * mat->data[0][col] * subdet;
    }
    return det;
}

int main() {
    Matrix a, b, result;
    printf("Matrix Operations Program\n");
    printf("First matrix:\n");
    if (!read_matrix(&a)) {
        printf("Failed to read first matrix.\n");
        return 1;
    }
    printf("Second matrix:\n");
    if (!read_matrix(&b)) {
        printf("Failed to read second matrix.\n");
        return 1;
    }
    printf("\nFirst matrix:\n");
    print_matrix(&a);
    printf("\nSecond matrix:\n");
    print_matrix(&b);
    printf("\nMatrix multiplication result:\n");
    if (multiply_matrices(&a, &b, &result)) {
        print_matrix(&result);
    }
    printf("\nDeterminant of first matrix: ");
    double det = matrix_determinant(&a);
    if (!isnan(det)) {
        printf("%.2f\n", det);
    } else {
        printf("Not a square matrix.\n");
    }
    printf("Determinant of second matrix: ");
    det = matrix_determinant(&b);
    if (!isnan(det)) {
        printf("%.2f\n", det);
    } else {
        printf("Not a square matrix.\n");
    }
    return 0;
}