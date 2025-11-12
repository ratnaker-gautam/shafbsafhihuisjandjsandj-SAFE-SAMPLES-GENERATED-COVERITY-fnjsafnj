//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: matrix_ops
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
    return (rows > 0 && rows <= MAX_DIM && cols > 0 && cols <= MAX_DIM);
}

void initialize_matrix(Matrix *m, int rows, int cols) {
    if (!validate_matrix_dimensions(rows, cols)) {
        fprintf(stderr, "Invalid matrix dimensions\n");
        exit(1);
    }
    m->rows = rows;
    m->cols = cols;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            m->data[i][j] = 0.0;
        }
    }
}

void read_matrix(Matrix *m) {
    int rows, cols;
    printf("Enter number of rows (1-%d): ", MAX_DIM);
    if (scanf("%d", &rows) != 1) {
        fprintf(stderr, "Invalid input for rows\n");
        exit(1);
    }
    printf("Enter number of columns (1-%d): ", MAX_DIM);
    if (scanf("%d", &cols) != 1) {
        fprintf(stderr, "Invalid input for columns\n");
        exit(1);
    }
    
    if (!validate_matrix_dimensions(rows, cols)) {
        fprintf(stderr, "Invalid matrix dimensions\n");
        exit(1);
    }
    
    initialize_matrix(m, rows, cols);
    
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (scanf("%lf", &m->data[i][j]) != 1) {
                fprintf(stderr, "Invalid matrix element\n");
                exit(1);
            }
        }
    }
}

void print_matrix(const Matrix *m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%8.2f ", m->data[i][j]);
        }
        printf("\n");
    }
}

int can_multiply(const Matrix *a, const Matrix *b) {
    return (a->cols == b->rows);
}

void matrix_multiply(const Matrix *a, const Matrix *b, Matrix *result) {
    if (!can_multiply(a, b)) {
        fprintf(stderr, "Matrices cannot be multiplied\n");
        exit(1);
    }
    
    initialize_matrix(result, a->rows, b->cols);
    
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

double matrix_determinant(const Matrix *m) {
    if (m->rows != m->cols) {
        fprintf(stderr, "Determinant requires square matrix\n");
        exit(1);
    }
    
    if (m->rows == 1) {
        return m->data[0][0];
    }
    
    if (m->rows == 2) {
        return m->data[0][0] * m->data[1][1] - m->data[0][1] * m->data[1][0];
    }
    
    double det = 0.0;
    for (int col = 0; col < m->cols; col++) {
        Matrix sub;
        initialize_matrix(&sub, m->rows - 1, m->cols - 1);
        
        for (int i = 1; i < m->rows; i++) {
            int sub_col = 0;
            for (int j = 0; j < m->cols; j++) {
                if (j != col) {
                    sub.data[i-1][sub_col] = m->data[i][j];
                    sub_col++;
                }
            }
        }
        
        double sign = (col % 2 == 0) ? 1.0 : -1.0;
        det += sign * m->data[0][col] * matrix_determinant(&sub);
    }
    
    return det;
}

int main() {
    Matrix a, b, result;
    
    printf("Matrix Operations Program\n");
    printf("Enter first matrix:\n");
    read_matrix(&a);
    
    printf("Enter second matrix:\n");
    read_matrix(&b);
    
    printf("\nFirst matrix:\n");
    print_matrix(&a);
    
    printf("\nSecond matrix:\n");
    print_matrix(&b);
    
    printf("\nMatrix multiplication result:\n");
    matrix_multiply(&a, &b, &result);
    print_matrix(&result);
    
    if (a.rows == a.cols) {
        printf("\nDeterminant of first matrix: %.2f\n", matrix_determinant(&a));
    }
    
    if (b.rows == b.cols) {
        printf("Determinant of second matrix: %.2f\n", matrix_d