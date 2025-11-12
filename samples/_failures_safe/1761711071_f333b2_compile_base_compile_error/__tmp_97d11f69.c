//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

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

void matrix_multiply(const Matrix *a, const Matrix *b, Matrix *result) {
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            result->data[i][j] = 0.0;
            for (int k = 0; k < a->cols; k++) {
                result->data[i][j] += a->data[i][k] * b->data[k][j];
            }
        }
    }
}

void matrix_transpose(const Matrix *src, Matrix *dest) {
    for (int i = 0; i < src->rows; i++) {
        for (int j = 0; j < src->cols; j++) {
            dest->data[j][i] = src->data[i][j];
        }
    }
}

double matrix_determinant(const Matrix *m) {
    if (m->rows != m->cols) return 0.0;
    if (m->rows == 1) return m->data[0][0];
    if (m->rows == 2) {
        return m->data[0][0] * m->data[1][1] - m->data[0][1] * m->data[1][0];
    }
    
    double det = 0.0;
    for (int col = 0; col < m->cols; col++) {
        Matrix sub;
        sub.rows = m->rows - 1;
        sub.cols = m->cols - 1;
        
        int sub_i = 0;
        for (int i = 1; i < m->rows; i++) {
            int sub_j = 0;
            for (int j = 0; j < m->cols; j++) {
                if (j == col) continue;
                sub.data[sub_i][sub_j] = m->data[i][j];
                sub_j++;
            }
            sub_i++;
        }
        
        double sub_det = matrix_determinant(&sub);
        double sign = (col % 2 == 0) ? 1.0 : -1.0;
        det += sign * m->data[0][col] * sub_det;
    }
    return det;
}

void print_matrix(const Matrix *m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%8.2f ", m->data[i][j]);
        }
        printf("\n");
    }
}

void initialize_random_matrix(Matrix *m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            m->data[i][j] = (rand() % 1000) / 100.0;
        }
    }
}

int main() {
    srand(42);
    
    int rows1, cols1, rows2, cols2;
    
    printf("Enter dimensions for first matrix (rows cols): ");
    if (scanf("%d %d", &rows1, &cols1) != 2) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (!validate_matrix_dimensions(rows1, cols1)) {
        printf("Invalid matrix dimensions\n");
        return 1;
    }
    
    printf("Enter dimensions for second matrix (rows cols): ");
    if (scanf("%d %d", &rows2, &cols2) != 2) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (!validate_matrix_dimensions(rows2, cols2)) {
        printf("Invalid matrix dimensions\n");
        return 1;
    }
    
    Matrix mat1, mat2;
    mat1.rows = rows1;
    mat1.cols = cols1;
    mat2.rows = rows2;
    mat2.cols = cols2;
    
    initialize_random_matrix(&mat1);
    initialize_random_matrix(&mat2);
    
    printf("\nMatrix 1:\n");
    print_matrix(&mat1);
    
    printf("\nMatrix 2:\n");
    print_matrix(&mat2);
    
    if (mat1.cols == mat2.rows) {
        Matrix result;
        result.rows = mat1.rows;
        result.cols = mat2.cols;
        matrix_multiply(&mat1, &mat2, &result);
        printf("\nMatrix multiplication result:\n");
        print_matrix(&result);
    } else {
        printf("\nMatrices cannot be multiplied (dimension mismatch)\n");
    }
    
    if (mat1.rows == mat1.cols) {
        double det = matrix_determinant(&mat1);
        printf("\nDeterminant of matrix 1: %.2f\n", det);
    } else {
        printf("\nMatrix 1 is not square, cannot compute determinant\n");
    }