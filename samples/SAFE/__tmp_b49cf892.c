//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_DIM 100

typedef struct {
    int rows;
    int cols;
    double data[MAX_DIM][MAX_DIM];
} Matrix;

int validate_matrix_dimensions(int rows, int cols) {
    return (rows > 0 && rows <= MAX_DIM && cols > 0 && cols <= MAX_DIM);
}

void initialize_matrix(Matrix *mat, int rows, int cols) {
    if (!validate_matrix_dimensions(rows, cols)) {
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

int multiply_matrices(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a == NULL || b == NULL || result == NULL) return 0;
    if (a->cols != b->rows) return 0;
    if (!validate_matrix_dimensions(a->rows, b->cols)) return 0;
    
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

double compute_matrix_determinant(const Matrix *mat) {
    if (mat == NULL) return 0.0;
    if (mat->rows != mat->cols) return 0.0;
    if (mat->rows == 1) return mat->data[0][0];
    if (mat->rows == 2) {
        return mat->data[0][0] * mat->data[1][1] - mat->data[0][1] * mat->data[1][0];
    }
    
    double det = 0.0;
    for (int col = 0; col < mat->cols; col++) {
        Matrix submat;
        submat.rows = mat->rows - 1;
        submat.cols = mat->cols - 1;
        
        int sub_i = 0;
        for (int i = 1; i < mat->rows; i++) {
            int sub_j = 0;
            for (int j = 0; j < mat->cols; j++) {
                if (j == col) continue;
                submat.data[sub_i][sub_j] = mat->data[i][j];
                sub_j++;
            }
            sub_i++;
        }
        
        double sub_det = compute_matrix_determinant(&submat);
        double sign = (col % 2 == 0) ? 1.0 : -1.0;
        det += sign * mat->data[0][col] * sub_det;
    }
    return det;
}

int main() {
    srand(42);
    
    Matrix a, b, result;
    int size = 3;
    
    if (!validate_matrix_dimensions(size, size)) {
        printf("Invalid matrix dimensions\n");
        return 1;
    }
    
    initialize_matrix(&a, size, size);
    initialize_matrix(&b, size, size);
    
    printf("Matrix A:\n");
    print_matrix(&a);
    printf("\nMatrix B:\n");
    print_matrix(&b);
    
    if (multiply_matrices(&a, &b, &result)) {
        printf("\nMatrix A * B:\n");
        print_matrix(&result);
    } else {
        printf("\nMatrix multiplication failed\n");
    }
    
    double det_a = compute_matrix_determinant(&a);
    double det_b = compute_matrix_determinant(&b);
    
    printf("\nDeterminant of A: %.2f\n", det_a);
    printf("Determinant of B: %.2f\n", det_b);
    
    return 0;
}