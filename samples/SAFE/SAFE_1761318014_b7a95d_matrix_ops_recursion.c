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

Matrix recursive_transpose(Matrix src, Matrix dest, int row, int col) {
    if (row >= src.rows) {
        return dest;
    }
    if (col >= src.cols) {
        return recursive_transpose(src, dest, row + 1, 0);
    }
    dest.data[col][row] = src.data[row][col];
    return recursive_transpose(src, dest, row, col + 1);
}

Matrix transpose_matrix(Matrix m) {
    if (m.rows <= 0 || m.cols <= 0 || m.rows > MAX_SIZE || m.cols > MAX_SIZE) {
        return create_matrix(0, 0);
    }
    Matrix result = create_matrix(m.cols, m.rows);
    return recursive_transpose(m, result, 0, 0);
}

int recursive_trace(Matrix m, int row, int sum) {
    if (row >= m.rows || row >= m.cols) {
        return sum;
    }
    return recursive_trace(m, row + 1, sum + m.data[row][row]);
}

int matrix_trace(Matrix m) {
    if (m.rows <= 0 || m.cols <= 0 || m.rows != m.cols) {
        return 0;
    }
    return recursive_trace(m, 0, 0);
}

int main() {
    int rows, cols;
    printf("Enter matrix rows and columns (max %d): ", MAX_SIZE);
    if (scanf("%d %d", &rows, &cols) != 2) {
        printf("Invalid input\n");
        return 1;
    }
    if (rows <= 0 || cols <= 0 || rows > MAX_SIZE || cols > MAX_SIZE) {
        printf("Invalid dimensions\n");
        return 1;
    }
    
    Matrix m = create_matrix(rows, cols);
    printf("Enter %d matrix elements:\n", rows * cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (scanf("%d", &m.data[i][j]) != 1) {
                printf("Invalid input\n");
                return 1;
            }
        }
    }
    
    printf("\nOriginal matrix:\n");
    print_matrix(m);
    
    Matrix transposed = transpose_matrix(m);
    if (transposed.rows > 0 && transposed.cols > 0) {
        printf("\nTransposed matrix:\n");
        print_matrix(transposed);
    }
    
    int trace = matrix_trace(m);
    if (m.rows == m.cols) {
        printf("\nMatrix trace: %d\n", trace);
    } else {
        printf("\nMatrix is not square, no trace calculated\n");
    }
    
    return 0;
}