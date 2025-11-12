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

Matrix recursive_transpose(Matrix m, int row, int col) {
    Matrix result = create_matrix(m.cols, m.rows);
    
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

int recursive_trace(Matrix m, int diag, int sum) {
    if (diag >= m.rows || diag >= m.cols) {
        return sum;
    }
    return recursive_trace(m, diag + 1, sum + m.data[diag][diag]);
}

int main() {
    int rows, cols;
    
    printf("Enter matrix dimensions (rows cols, max %dx%d): ", MAX_SIZE, MAX_SIZE);
    if (scanf("%d %d", &rows, &cols) != 2) {
        printf("Invalid dimensions\n");
        return 1;
    }
    
    if (!validate_matrix_size(rows, cols)) {
        printf("Invalid matrix size\n");
        return 1;
    }
    
    Matrix original = create_matrix(rows, cols);
    read_matrix_input(&original);
    
    printf("\nOriginal matrix:\n");
    print_matrix(original);
    
    if (rows == cols) {
        int trace = recursive_trace(original, 0, 0);
        printf("\nMatrix trace: %d\n", trace);
    } else {
        printf("\nMatrix is not square, cannot compute trace\n");
    }
    
    Matrix transposed = recursive_transpose(original, 0, 0);
    printf("\nTransposed matrix:\n");
    print_matrix(transposed);
    
    return 0;
}