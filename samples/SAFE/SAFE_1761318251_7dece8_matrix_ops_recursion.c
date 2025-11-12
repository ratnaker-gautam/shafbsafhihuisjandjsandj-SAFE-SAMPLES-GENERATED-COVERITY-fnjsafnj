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
    printf("Enter matrix dimensions (rows cols, max %dx%d): ", MAX_SIZE, MAX_SIZE);
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

void print_matrix(const Matrix *m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%d ", m->data[i][j]);
        }
        printf("\n");
    }
}

int recursive_trace(const Matrix *m, int row, int col, int sum) {
    if (row >= m->rows || col >= m->cols) {
        return sum;
    }
    
    if (m->rows != m->cols) {
        return -1;
    }
    
    int new_sum;
    if (sum > INT_MAX - m->data[row][col]) {
        return -1;
    }
    new_sum = sum + m->data[row][col];
    
    return recursive_trace(m, row + 1, col + 1, new_sum);
}

int matrix_trace(const Matrix *m) {
    if (m->rows != m->cols) {
        return -1;
    }
    return recursive_trace(m, 0, 0, 0);
}

int recursive_determinant(const Matrix *m, int size, int temp[MAX_SIZE][MAX_SIZE]) {
    if (size == 1) {
        return temp[0][0];
    }
    
    if (size == 2) {
        int64_t det = (int64_t)temp[0][0] * temp[1][1] - (int64_t)temp[0][1] * temp[1][0];
        if (det > INT_MAX || det < INT_MIN) {
            return 0;
        }
        return (int)det;
    }
    
    int sign = 1;
    int result = 0;
    
    for (int f = 0; f < size; f++) {
        int submatrix[MAX_SIZE][MAX_SIZE];
        int sub_i = 0;
        
        for (int i = 1; i < size; i++) {
            int sub_j = 0;
            for (int j = 0; j < size; j++) {
                if (j == f) continue;
                submatrix[sub_i][sub_j] = temp[i][j];
                sub_j++;
            }
            sub_i++;
        }
        
        int64_t term = (int64_t)sign * temp[0][f] * recursive_determinant(m, size - 1, submatrix);
        if (term > INT_MAX || term < INT_MIN) {
            return 0;
        }
        
        int64_t new_result = (int64_t)result + term;
        if (new_result > INT_MAX || new_result < INT_MIN) {
            return 0;
        }
        result = (int)new_result;
        sign = -sign;
    }
    
    return result;
}

int matrix_determinant(const Matrix *m) {
    if (m->rows != m->cols) {
        return 0;
    }
    
    int temp[MAX_SIZE][MAX_SIZE];
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->rows; j++) {
            temp[i][j] = m->data[i][j];
        }
    }
    
    return recursive_determinant(m, m->rows, temp);
}

int main() {
    Matrix mat;
    
    printf("Matrix Operations Program\n");
    read_matrix(&mat);
    
    printf("\nMatrix:\n");
    print_matrix(&mat);
    
    int trace = matrix_trace(&mat);
    if (trace >= 0) {
        printf("Trace: %d\n", trace);
    } else {
        printf("Trace: Not defined for non-square matrix\n");
    }
    
    if (mat.rows == mat.cols) {
        int det = matrix_determinant(&mat);
        printf("Determinant: %d\n", det);
    } else {
        printf("Determinant: Not defined for non-square matrix\n");
    }
    
    return 0;
}