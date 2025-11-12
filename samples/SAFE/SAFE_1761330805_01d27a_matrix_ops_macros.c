//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ROWS 10
#define MAX_COLS 10
#define MATRIX_SIZE (MAX_ROWS * MAX_COLS)

typedef struct {
    int data[MAX_ROWS][MAX_COLS];
    int rows;
    int cols;
} Matrix;

Matrix create_matrix(int rows, int cols) {
    Matrix m;
    m.rows = rows;
    m.cols = cols;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            m.data[i][j] = 0;
        }
    }
    return m;
}

int validate_matrix_size(int rows, int cols) {
    if (rows <= 0 || cols <= 0) return 0;
    if (rows > MAX_ROWS || cols > MAX_COLS) return 0;
    return 1;
}

void input_matrix(Matrix *m) {
    printf("Enter matrix elements (%d x %d):\n", m->rows, m->cols);
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("Element [%d][%d]: ", i, j);
            if (scanf("%d", &m->data[i][j]) != 1) {
                printf("Invalid input\n");
                exit(1);
            }
        }
    }
}

void print_matrix(const Matrix *m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%6d", m->data[i][j]);
        }
        printf("\n");
    }
}

Matrix add_matrices(const Matrix *a, const Matrix *b) {
    Matrix result = create_matrix(a->rows, a->cols);
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            if ((a->data[i][j] > 0 && b->data[i][j] > INT_MAX - a->data[i][j]) ||
                (a->data[i][j] < 0 && b->data[i][j] < INT_MIN - a->data[i][j])) {
                printf("Integer overflow detected\n");
                exit(1);
            }
            result.data[i][j] = a->data[i][j] + b->data[i][j];
        }
    }
    return result;
}

Matrix multiply_matrices(const Matrix *a, const Matrix *b) {
    Matrix result = create_matrix(a->rows, b->cols);
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            long long sum = 0;
            for (int k = 0; k < a->cols; k++) {
                long long product = (long long)a->data[i][k] * b->data[k][j];
                if (product > INT_MAX - sum || product < INT_MIN - sum) {
                    printf("Integer overflow detected\n");
                    exit(1);
                }
                sum += product;
            }
            if (sum > INT_MAX || sum < INT_MIN) {
                printf("Integer overflow detected\n");
                exit(1);
            }
            result.data[i][j] = (int)sum;
        }
    }
    return result;
}

int main() {
    int rows1, cols1, rows2, cols2;
    
    printf("Enter dimensions for first matrix (rows cols): ");
    if (scanf("%d %d", &rows1, &cols1) != 2) {
        printf("Invalid input\n");
        return 1;
    }
    if (!validate_matrix_size(rows1, cols1)) {
        printf("Invalid matrix dimensions\n");
        return 1;
    }
    
    printf("Enter dimensions for second matrix (rows cols): ");
    if (scanf("%d %d", &rows2, &cols2) != 2) {
        printf("Invalid input\n");
        return 1;
    }
    if (!validate_matrix_size(rows2, cols2)) {
        printf("Invalid matrix dimensions\n");
        return 1;
    }
    
    Matrix m1 = create_matrix(rows1, cols1);
    Matrix m2 = create_matrix(rows2, cols2);
    
    input_matrix(&m1);
    input_matrix(&m2);
    
    printf("\nMatrix 1:\n");
    print_matrix(&m1);
    printf("\nMatrix 2:\n");
    print_matrix(&m2);
    
    if (m1.rows == m2.rows && m1.cols == m2.cols) {
        Matrix sum = add_matrices(&m1, &m2);
        printf("\nMatrix Sum:\n");
        print_matrix(&sum);
    } else {
        printf("\nMatrices cannot be added (dimensions differ)\n");
    }
    
    if (m1.cols == m2.rows) {
        Matrix product = multiply_matrices(&m1, &m2);
        printf("\nMatrix Product:\n");
        print_matrix(&product);
    } else {
        printf("\nMatrices cannot be multiplied (incompatible dimensions)\n");
    }
    
    return 0;
}