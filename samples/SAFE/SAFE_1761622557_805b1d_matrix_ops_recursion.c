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
    if (rows <= 0 || rows > MAX_SIZE || cols <= 0 || cols > MAX_SIZE) {
        m.rows = 0;
        m.cols = 0;
        return m;
    }
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
    if (m.rows <= 0 || m.cols <= 0) {
        printf("Invalid matrix\n");
        return;
    }
    for (int i = 0; i < m.rows; i++) {
        for (int j = 0; j < m.cols; j++) {
            printf("%3d ", m.data[i][j]);
        }
        printf("\n");
    }
}

Matrix recursive_transpose(Matrix src, Matrix dest, int i, int j) {
    if (i >= src.rows) {
        return dest;
    }
    if (j >= src.cols) {
        return recursive_transpose(src, dest, i + 1, 0);
    }
    if (i < dest.cols && j < dest.rows) {
        dest.data[j][i] = src.data[i][j];
    }
    return recursive_transpose(src, dest, i, j + 1);
}

Matrix transpose_matrix(Matrix m) {
    if (m.rows <= 0 || m.cols <= 0 || m.rows > MAX_SIZE || m.cols > MAX_SIZE) {
        return create_matrix(0, 0);
    }
    Matrix result = create_matrix(m.cols, m.rows);
    if (result.rows == 0 || result.cols == 0) {
        return result;
    }
    return recursive_transpose(m, result, 0, 0);
}

int recursive_trace(Matrix m, int i, int sum) {
    if (i >= m.rows || i >= m.cols) {
        return sum;
    }
    return recursive_trace(m, i + 1, sum + m.data[i][i]);
}

int matrix_trace(Matrix m) {
    if (m.rows <= 0 || m.cols <= 0 || m.rows != m.cols) {
        return 0;
    }
    return recursive_trace(m, 0, 0);
}

int main() {
    int rows, cols;
    printf("Enter matrix rows (1-%d): ", MAX_SIZE);
    if (scanf("%d", &rows) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    printf("Enter matrix cols (1-%d): ", MAX_SIZE);
    if (scanf("%d", &cols) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (rows <= 0 || rows > MAX_SIZE || cols <= 0 || cols > MAX_SIZE) {
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
    printf("\nTransposed matrix:\n");
    print_matrix(transposed);
    
    if (rows == cols) {
        int trace = matrix_trace(m);
        printf("\nMatrix trace: %d\n", trace);
    } else {
        printf("\nMatrix is not square, no trace calculated\n");
    }
    
    return 0;
}