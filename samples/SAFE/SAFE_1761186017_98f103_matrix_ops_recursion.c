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

int recursive_determinant(Matrix m) {
    if (m.rows != m.cols) return 0;
    if (m.rows == 1) return m.data[0][0];
    if (m.rows == 2) {
        return m.data[0][0] * m.data[1][1] - m.data[0][1] * m.data[1][0];
    }
    
    int det = 0;
    for (int col = 0; col < m.cols; col++) {
        Matrix sub;
        sub.rows = m.rows - 1;
        sub.cols = m.cols - 1;
        
        int sub_i = 0;
        for (int i = 1; i < m.rows; i++) {
            int sub_j = 0;
            for (int j = 0; j < m.cols; j++) {
                if (j == col) continue;
                sub.data[sub_i][sub_j] = m.data[i][j];
                sub_j++;
            }
            sub_i++;
        }
        
        int sign = (col % 2 == 0) ? 1 : -1;
        int sub_det = recursive_determinant(sub);
        if (__builtin_add_overflow_p(det, sign * m.data[0][col] * sub_det, det)) {
            printf("Determinant overflow\n");
            exit(1);
        }
        det += sign * m.data[0][col] * sub_det;
    }
    return det;
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
    
    Matrix m = create_matrix(rows, cols);
    read_matrix_input(&m);
    
    printf("\nMatrix:\n");
    print_matrix(m);
    
    if (rows == cols) {
        int det = recursive_determinant(m);
        printf("\nDeterminant: %d\n", det);
    } else {
        printf("\nMatrix is not square, cannot compute determinant\n");
    }
    
    return 0;
}