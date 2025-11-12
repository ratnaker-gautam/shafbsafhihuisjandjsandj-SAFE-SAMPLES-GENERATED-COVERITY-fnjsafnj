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
        memset(m.data, 0, sizeof(m.data));
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
        printf("Invalid matrix dimensions\n");
        return;
    }
    for (int i = 0; i < m.rows; i++) {
        for (int j = 0; j < m.cols; j++) {
            printf("%3d ", m.data[i][j]);
        }
        printf("\n");
    }
}

Matrix recursive_transpose(Matrix m, int row, int col) {
    Matrix result = create_matrix(m.cols, m.rows);
    if (m.rows <= 0 || m.cols <= 0) {
        return result;
    }
    if (row >= m.rows) {
        return result;
    }
    if (col >= m.cols) {
        return recursive_transpose(m, row + 1, 0);
    }
    result = recursive_transpose(m, row, col + 1);
    result.data[col][row] = m.data[row][col];
    return result;
}

Matrix transpose_matrix(Matrix m) {
    return recursive_transpose(m, 0, 0);
}

int read_int_safe() {
    char buffer[32];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return -1;
    }
    char *endptr;
    long val = strtol(buffer, &endptr, 10);
    if (endptr == buffer || *endptr != '\n') {
        return -1;
    }
    if (val < 0 || val > MAX_SIZE) {
        return -1;
    }
    return (int)val;
}

int main() {
    printf("Enter matrix rows (1-%d): ", MAX_SIZE);
    int rows = read_int_safe();
    if (rows <= 0) {
        printf("Invalid rows\n");
        return 1;
    }
    printf("Enter matrix cols (1-%d): ", MAX_SIZE);
    int cols = read_int_safe();
    if (cols <= 0) {
        printf("Invalid cols\n");
        return 1;
    }
    Matrix m = create_matrix(rows, cols);
    printf("Enter %d matrix elements:\n", rows * cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("Element [%d][%d]: ", i, j);
            int val = read_int_safe();
            if (val < 0) {
                printf("Invalid input\n");
                return 1;
            }
            m.data[i][j] = val;
        }
    }
    printf("\nOriginal matrix:\n");
    print_matrix(m);
    Matrix transposed = transpose_matrix(m);
    printf("\nTransposed matrix:\n");
    print_matrix(transposed);
    return 0;
}