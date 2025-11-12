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

int recursive_sum_row(Matrix m, int row, int col) {
    if (col < 0) return 0;
    return m.data[row][col] + recursive_sum_row(m, row, col - 1);
}

int recursive_sum_matrix(Matrix m, int row, int col) {
    if (row < 0) return 0;
    if (col < 0) return recursive_sum_matrix(m, row - 1, m.cols - 1);
    return m.data[row][col] + recursive_sum_matrix(m, row, col - 1);
}

Matrix recursive_transpose(Matrix src, Matrix dest, int row, int col) {
    if (row >= src.rows) return dest;
    if (col >= src.cols) return recursive_transpose(src, dest, row + 1, 0);
    dest.data[col][row] = src.data[row][col];
    return recursive_transpose(src, dest, row, col + 1);
}

int validate_dimensions(int rows, int cols) {
    if (rows <= 0 || cols <= 0) return 0;
    if (rows > MAX_SIZE || cols > MAX_SIZE) return 0;
    return 1;
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

    if (!validate_dimensions(rows, cols)) {
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

    printf("\nRow sums:\n");
    for (int i = 0; i < rows; i++) {
        int sum = recursive_sum_row(m, i, cols - 1);
        printf("Row %d: %d\n", i, sum);
    }

    int total_sum = recursive_sum_matrix(m, rows - 1, cols - 1);
    printf("\nTotal sum: %d\n", total_sum);

    if (rows == cols) {
        Matrix transposed = create_matrix(cols, rows);
        transposed = recursive_transpose(m, transposed, 0, 0);
        printf("\nTransposed matrix:\n");
        print_matrix(transposed);
    } else {
        printf("\nMatrix not square, skipping transpose\n");
    }

    return 0;
}