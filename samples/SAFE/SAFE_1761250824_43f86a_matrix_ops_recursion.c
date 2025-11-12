//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 8

typedef struct {
    int rows;
    int cols;
    int data[MAX_SIZE][MAX_SIZE];
} Matrix;

int validate_matrix_size(int rows, int cols) {
    return (rows > 0 && rows <= MAX_SIZE && cols > 0 && cols <= MAX_SIZE);
}

void read_matrix(Matrix *m) {
    printf("Enter matrix dimensions (rows cols): ");
    if (scanf("%d %d", &m->rows, &m->cols) != 2) {
        printf("Invalid input\n");
        exit(1);
    }
    
    if (!validate_matrix_size(m->rows, m->cols)) {
        printf("Invalid matrix size\n");
        exit(1);
    }
    
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

void print_matrix(Matrix *m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%d ", m->data[i][j]);
        }
        printf("\n");
    }
}

int recursive_trace(Matrix *m, int row, int col, int sum) {
    if (row >= m->rows || col >= m->cols) {
        return sum;
    }
    
    if (sum > INT_MAX - m->data[row][col]) {
        printf("Integer overflow detected\n");
        exit(1);
    }
    
    sum += m->data[row][col];
    
    if (row == m->rows - 1 && col == m->cols - 1) {
        return sum;
    }
    
    if (row < m->rows - 1) {
        return recursive_trace(m, row + 1, col, sum);
    } else {
        return recursive_trace(m, row, col + 1, sum);
    }
}

int recursive_determinant(Matrix *m, int size, int *temp) {
    if (size == 1) {
        return m->data[0][0];
    }
    
    if (size == 2) {
        int64_t det = (int64_t)m->data[0][0] * m->data[1][1] - 
                     (int64_t)m->data[0][1] * m->data[1][0];
        if (det > INT_MAX || det < INT_MIN) {
            printf("Determinant overflow\n");
            exit(1);
        }
        return (int)det;
    }
    
    int det = 0;
    int sign = 1;
    
    for (int i = 0; i < size; i++) {
        int sub_i = 0;
        for (int row = 1; row < size; row++) {
            int sub_j = 0;
            for (int col = 0; col < size; col++) {
                if (col != i) {
                    temp[sub_i * (size - 1) + sub_j] = m->data[row][col];
                    sub_j++;
                }
            }
            sub_i++;
        }
        
        Matrix sub_mat = {size - 1, size - 1};
        for (int k = 0; k < (size - 1) * (size - 1); k++) {
            sub_mat.data[k / (size - 1)][k % (size - 1)] = temp[k];
        }
        
        int64_t term = (int64_t)sign * m->data[0][i] * 
                      recursive_determinant(&sub_mat, size - 1, temp);
        
        if (term > INT_MAX || term < INT_MIN) {
            printf("Determinant overflow\n");
            exit(1);
        }
        
        det += (int)term;
        sign = -sign;
    }
    
    return det;
}

int main() {
    Matrix mat;
    read_matrix(&mat);
    
    printf("\nOriginal matrix:\n");
    print_matrix(&mat);
    
    if (mat.rows == mat.cols) {
        int *temp_buffer = malloc(mat.rows * mat.rows * sizeof(int));
        if (!temp_buffer) {
            printf("Memory allocation failed\n");
            return 1;
        }
        
        int det = recursive_determinant(&mat, mat.rows, temp_buffer);
        printf("\nDeterminant: %d\n", det);
        free(temp_buffer);
    } else {
        printf("\nMatrix is not square, skipping determinant\n");
    }
    
    int trace_sum = recursive_trace(&mat, 0, 0, 0);
    printf("Diagonal trace sum: %d\n", trace_sum);
    
    return 0;
}