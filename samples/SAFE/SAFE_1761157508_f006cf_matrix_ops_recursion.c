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
    if (m.rows == 0 || m.cols == 0) {
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

int recursive_sum_row(Matrix m, int row, int col) {
    if (row < 0 || row >= m.rows || col < 0 || col >= m.cols) {
        return 0;
    }
    if (col == m.cols - 1) {
        return m.data[row][col];
    }
    return m.data[row][col] + recursive_sum_row(m, row, col + 1);
}

int recursive_sum_all(Matrix m, int row) {
    if (row < 0 || row >= m.rows) {
        return 0;
    }
    if (row == m.rows - 1) {
        return recursive_sum_row(m, row, 0);
    }
    return recursive_sum_row(m, row, 0) + recursive_sum_all(m, row + 1);
}

Matrix recursive_transpose(Matrix m, int i, int j) {
    Matrix result = create_matrix(m.cols, m.rows);
    if (i < 0 || i >= m.rows || j < 0 || j >= m.cols) {
        return result;
    }
    if (i == m.rows - 1 && j == m.cols - 1) {
        result.data[j][i] = m.data[i][j];
        return result;
    }
    if (j == m.cols - 1) {
        result = recursive_transpose(m, i + 1, 0);
        result.data[j][i] = m.data[i][j];
        return result;
    }
    result = recursive_transpose(m, i, j + 1);
    result.data[j][i] = m.data[i][j];
    return result;
}

int main() {
    int rows, cols;
    printf("Enter matrix rows (1-%d): ", MAX_SIZE);
    if (scanf("%d", &rows) != 1 || rows <= 0 || rows > MAX_SIZE) {
        printf("Invalid input\n");
        return 1;
    }
    printf("Enter matrix cols (1-%d): ", MAX_SIZE);
    if (scanf("%d", &cols) != 1 || cols <= 0 || cols > MAX_SIZE) {
        printf("Invalid input\n");
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

    printf("\nRow sums:\n");
    for (int i = 0; i < rows; i++) {
        int sum = recursive_sum_row(m, i, 0);
        printf("Row %d: %d\n", i, sum);
    }

    int total_sum = recursive_sum_all(m, 0);
    printf("\nTotal sum of all elements: %d\n", total_sum);

    Matrix transposed = recursive_transpose(m, 0, 0);
    printf("\nTransposed matrix:\n");
    print_matrix(transposed);

    return 0;
}