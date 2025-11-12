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
            printf("%4d", m.data[i][j]);
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

int recursive_determinant(Matrix m, int n) {
    if (n == 1) {
        return m.data[0][0];
    }
    
    int det = 0;
    int sign = 1;
    Matrix temp;
    temp.rows = n - 1;
    temp.cols = n - 1;
    
    for (int col = 0; col < n; col++) {
        int sub_i = 0;
        for (int i = 1; i < n; i++) {
            int sub_j = 0;
            for (int j = 0; j < n; j++) {
                if (j == col) continue;
                temp.data[sub_i][sub_j] = m.data[i][j];
                sub_j++;
            }
            sub_i++;
        }
        det += sign * m.data[0][col] * recursive_determinant(temp, n - 1);
        sign = -sign;
    }
    return det;
}

int calculate_determinant(Matrix m) {
    if (m.rows != m.cols) {
        return 0;
    }
    return recursive_determinant(m, m.rows);
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
    
    printf("\nMatrix:\n");
    print_matrix(m);
    
    if (rows == cols) {
        int det = calculate_determinant(m);
        printf("\nDeterminant: %d\n", det);
    } else {
        printf("\nMatrix is not square, cannot compute determinant\n");
    }
    
    return 0;
}