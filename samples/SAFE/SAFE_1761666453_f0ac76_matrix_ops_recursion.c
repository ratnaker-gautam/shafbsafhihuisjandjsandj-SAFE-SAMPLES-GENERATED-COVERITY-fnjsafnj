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

int validate_matrix_size(int rows, int cols) {
    return (rows > 0 && rows <= MAX_SIZE && cols > 0 && cols <= MAX_SIZE);
}

void print_matrix(Matrix m) {
    for (int i = 0; i < m.rows; i++) {
        for (int j = 0; j < m.cols; j++) {
            printf("%d ", m.data[i][j]);
        }
        printf("\n");
    }
}

void read_matrix_input(Matrix *m) {
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            if (scanf("%d", &m->data[i][j]) != 1) {
                printf("Invalid input\n");
                exit(1);
            }
        }
    }
}

int recursive_trace(Matrix m, int row, int col, int sum) {
    if (row >= m.rows || col >= m.cols) {
        return sum;
    }
    if (row == col) {
        sum += m.data[row][col];
    }
    if (col < m.cols - 1) {
        return recursive_trace(m, row, col + 1, sum);
    } else {
        return recursive_trace(m, row + 1, 0, sum);
    }
}

int calculate_trace(Matrix m) {
    if (m.rows != m.cols) {
        return -1;
    }
    return recursive_trace(m, 0, 0, 0);
}

Matrix recursive_transpose(Matrix m, Matrix result, int row, int col) {
    if (row >= m.rows) {
        return result;
    }
    if (col >= m.cols) {
        return recursive_transpose(m, result, row + 1, 0);
    }
    result.data[col][row] = m.data[row][col];
    return recursive_transpose(m, result, row, col + 1);
}

Matrix transpose_matrix(Matrix m) {
    Matrix result = create_matrix(m.cols, m.rows);
    return recursive_transpose(m, result, 0, 0);
}

int main() {
    int rows, cols;
    
    printf("Enter number of rows (1-%d): ", MAX_SIZE);
    if (scanf("%d", &rows) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    printf("Enter number of columns (1-%d): ", MAX_SIZE);
    if (scanf("%d", &cols) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (!validate_matrix_size(rows, cols)) {
        printf("Invalid matrix size\n");
        return 1;
    }
    
    Matrix m = create_matrix(rows, cols);
    read_matrix_input(&m);
    
    printf("\nOriginal matrix:\n");
    print_matrix(m);
    
    int trace = calculate_trace(m);
    if (trace != -1) {
        printf("\nMatrix trace: %d\n", trace);
    } else {
        printf("\nMatrix is not square, cannot calculate trace\n");
    }
    
    Matrix transposed = transpose_matrix(m);
    printf("\nTransposed matrix:\n");
    print_matrix(transposed);
    
    return 0;
}