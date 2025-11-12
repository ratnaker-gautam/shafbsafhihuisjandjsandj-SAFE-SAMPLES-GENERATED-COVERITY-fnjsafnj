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
    if (rows <= 0 || cols <= 0) return 0;
    if (rows > MAX_SIZE || cols > MAX_SIZE) return 0;
    return 1;
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
    printf("Enter matrix dimensions (rows cols): ");
    int rows, cols;
    if (scanf("%d %d", &rows, &cols) != 2) {
        printf("Invalid input\n");
        exit(1);
    }
    
    if (!validate_matrix_size(rows, cols)) {
        printf("Invalid matrix size\n");
        exit(1);
    }
    
    m->rows = rows;
    m->cols = cols;
    
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (scanf("%d", &m->data[i][j]) != 1) {
                printf("Invalid input\n");
                exit(1);
            }
        }
    }
}

Matrix recursive_transpose(Matrix m, int row, int col) {
    Matrix result = create_matrix(m.cols, m.rows);
    
    void transpose_helper(int r, int c) {
        if (r >= m.rows) return;
        if (c >= m.cols) {
            transpose_helper(r + 1, 0);
            return;
        }
        result.data[c][r] = m.data[r][c];
        transpose_helper(r, c + 1);
    }
    
    transpose_helper(row, col);
    return result;
}

int recursive_trace(Matrix m, int diag, int sum) {
    if (diag >= m.rows || diag >= m.cols) return sum;
    return recursive_trace(m, diag + 1, sum + m.data[diag][diag]);
}

Matrix recursive_add(Matrix a, Matrix b, int row, int col) {
    Matrix result = create_matrix(a.rows, a.cols);
    
    void add_helper(int r, int c) {
        if (r >= a.rows) return;
        if (c >= a.cols) {
            add_helper(r + 1, 0);
            return;
        }
        result.data[r][c] = a.data[r][c] + b.data[r][c];
        add_helper(r, c + 1);
    }
    
    add_helper(row, col);
    return result;
}

int main() {
    Matrix A, B;
    
    printf("Matrix A:\n");
    read_matrix_input(&A);
    
    printf("Matrix B:\n");
    read_matrix_input(&B);
    
    if (A.rows != B.rows || A.cols != B.cols) {
        printf("Matrices must have same dimensions for addition\n");
        return 1;
    }
    
    printf("\nMatrix A:\n");
    print_matrix(A);
    
    printf("\nMatrix B:\n");
    print_matrix(B);
    
    printf("\nMatrix A + B:\n");
    Matrix sum = recursive_add(A, B, 0, 0);
    print_matrix(sum);
    
    printf("\nTranspose of A:\n");
    Matrix transA = recursive_transpose(A, 0, 0);
    print_matrix(transA);
    
    if (A.rows == A.cols) {
        int trace = recursive_trace(A, 0, 0);
        printf("\nTrace of A: %d\n", trace);
    } else {
        printf("\nTrace undefined for non-square matrix A\n");
    }
    
    if (B.rows == B.cols) {
        int trace = recursive_trace(B, 0, 0);
        printf("Trace of B: %d\n", trace);
    } else {
        printf("Trace undefined for non-square matrix B\n");
    }
    
    return 0;
}