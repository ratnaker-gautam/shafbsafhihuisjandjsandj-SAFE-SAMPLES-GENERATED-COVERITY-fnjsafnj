//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: matrix_ops
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
    if (rows <= 0 || cols <= 0 || rows > MAX_DIM || cols > MAX_DIM) {
        return 0;
    }
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

void matrix_transpose(const Matrix *src, Matrix *dst) {
    for (int i = 0; i < src->rows; i++) {
        for (int j = 0; j < src->cols; j++) {
            dst->data[j][i] = src->data[i][j];
        }
    }
}

double matrix_determinant(const Matrix *m) {
    if (m->rows != m->cols || m->rows > 3) {
        return 0.0;
    }
    
    if (m->rows == 1) {
        return m->data[0][0];
    }
    
    if (m->rows == 2) {
        return m->data[0][0] * m->data[1][1] - m->data[0][1] * m->data[1][0];
    }
    
    if (m->rows == 3) {
        return m->data[0][0] * (m->data[1][1] * m->data[2][2] - m->data[1][2] * m->data[2][1]) -
               m->data[0][1] * (m->data[1][0] * m->data[2][2] - m->data[1][2] * m->data[2][0]) +
               m->data[0][2] * (m->data[1][0] * m->data[2][1] - m->data[1][1] * m->data[2][0]);
    }
    
    return 0.0;
}

void print_matrix(const Matrix *m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%8.2f ", m->data[i][j]);
        }
        printf("\n");
    }
}

int read_matrix(Matrix *m) {
    int rows, cols;
    
    printf("Enter number of rows (1-%d): ", MAX_DIM);
    if (scanf("%d", &rows) != 1) {
        return 0;
    }
    
    printf("Enter number of columns (1-%d): ", MAX_DIM);
    if (scanf("%d", &cols) != 1) {
        return 0;
    }
    
    if (!validate_matrix_dimensions(rows, cols)) {
        return 0;
    }
    
    m->rows = rows;
    m->cols = cols;
    
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (scanf("%lf", &m->data[i][j]) != 1) {
                return 0;
            }
        }
    }
    
    return 1;
}

int main() {
    Matrix a, b, result;
    int choice;
    
    printf("Matrix Operations Program\n");
    printf("1. Matrix Multiplication\n");
    printf("2. Matrix Transpose\n");
    printf("3. Matrix Determinant (1x1, 2x2, 3x3 only)\n");
    printf("Enter choice (1-3): ");
    
    if (scanf("%d", &choice) != 1 || choice < 1 || choice > 3) {
        printf("Invalid choice\n");
        return 1;
    }
    
    switch (choice) {
        case 1:
            printf("Enter first matrix:\n");
            if (!read_matrix(&a)) {
                printf("Invalid first matrix input\n");
                return 1;
            }
            
            printf("Enter second matrix:\n");
            if (!read_matrix(&b)) {
                printf("Invalid second matrix input\n");
                return 1;
            }
            
            if (a.cols != b.rows) {
                printf("Matrix dimensions incompatible for multiplication\n");
                return 1;
            }
            
            result.rows = a.rows;
            result.cols = b.cols;
            matrix_multiply(&a, &b, &result);
            
            printf("Result of multiplication:\n");
            print_matrix(&result);
            break;
            
        case 2:
            printf("Enter matrix:\n");
            if (!read_matrix(&a)) {
                printf("Invalid matrix input\n");
                return 1;
            }
            
            result.rows = a.cols;
            result.cols = a.rows