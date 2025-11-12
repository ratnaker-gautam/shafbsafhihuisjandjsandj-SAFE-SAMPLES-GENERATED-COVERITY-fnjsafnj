//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 10

typedef struct {
    int rows;
    int cols;
    int data[MAX_SIZE][MAX_SIZE];
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

void print_matrix(Matrix m) {
    for (int i = 0; i < m.rows; i++) {
        for (int j = 0; j < m.cols; j++) {
            printf("%d ", m.data[i][j]);
        }
        printf("\n");
    }
}

int read_int() {
    char buffer[32];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return -1;
    }
    int value;
    if (sscanf(buffer, "%d", &value) != 1) {
        return -1;
    }
    return value;
}

void recursive_fill(Matrix *m, int row, int col) {
    if (row >= m->rows) {
        return;
    }
    if (col >= m->cols) {
        recursive_fill(m, row + 1, 0);
        return;
    }
    printf("Enter value for [%d][%d]: ", row, col);
    int value = read_int();
    if (value == -1) {
        printf("Invalid input. Using 0.\n");
        value = 0;
    }
    m->data[row][col] = value;
    recursive_fill(m, row, col + 1);
}

Matrix recursive_multiply(Matrix a, Matrix b, int row, int col, int k) {
    Matrix result = create_matrix(a.rows, b.cols);
    if (row >= a.rows) {
        return result;
    }
    if (col >= b.cols) {
        Matrix next = recursive_multiply(a, b, row + 1, 0, 0);
        for (int i = 0; i < a.rows; i++) {
            for (int j = 0; j < b.cols; j++) {
                result.data[i][j] = next.data[i][j];
            }
        }
        return result;
    }
    if (k >= a.cols) {
        Matrix next = recursive_multiply(a, b, row, col + 1, 0);
        for (int i = 0; i < a.rows; i++) {
            for (int j = 0; j < b.cols; j++) {
                result.data[i][j] = next.data[i][j];
            }
        }
        result.data[row][col] = a.data[row][k - 1] * b.data[k - 1][col];
        return result;
    }
    Matrix partial = recursive_multiply(a, b, row, col, k + 1);
    for (int i = 0; i < a.rows; i++) {
        for (int j = 0; j < b.cols; j++) {
            result.data[i][j] = partial.data[i][j];
        }
    }
    result.data[row][col] += a.data[row][k] * b.data[k][col];
    return result;
}

int main() {
    printf("Matrix A dimensions (rows cols, max %d): ", MAX_SIZE);
    int rows_a = read_int();
    int cols_a = read_int();
    if (rows_a <= 0 || rows_a > MAX_SIZE || cols_a <= 0 || cols_a > MAX_SIZE) {
        printf("Invalid dimensions.\n");
        return 1;
    }

    printf("Matrix B dimensions (rows cols, max %d): ", MAX_SIZE);
    int rows_b = read_int();
    int cols_b = read_int();
    if (rows_b <= 0 || rows_b > MAX_SIZE || cols_b <= 0 || cols_b > MAX_SIZE) {
        printf("Invalid dimensions.\n");
        return 1;
    }

    if (cols_a != rows_b) {
        printf("Matrix multiplication not possible.\n");
        return 1;
    }

    Matrix a = create_matrix(rows_a, cols_a);
    Matrix b = create_matrix(rows_b, cols_b);

    printf("Enter Matrix A values:\n");
    recursive_fill(&a, 0, 0);

    printf("Enter Matrix B values:\n");
    recursive_fill(&b, 0, 0);

    printf("Matrix A:\n");
    print_matrix(a);
    printf("Matrix B:\n");
    print_matrix(b);

    Matrix result = recursive_multiply(a, b, 0, 0, 0);
    printf("Result A * B:\n");
    print_matrix(result);

    return 0;
}