//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

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

int recursive_determinant(Matrix m) {
    if (m.rows != m.cols) return 0;
    if (m.rows == 1) return m.data[0][0];
    if (m.rows == 2) {
        long long prod1 = (long long)m.data[0][0] * m.data[1][1];
        long long prod2 = (long long)m.data[0][1] * m.data[1][0];
        long long result = prod1 - prod2;
        if (result > INT_MAX || result < INT_MIN) return 0;
        return (int)result;
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
                if (sub_j >= sub.cols) break;
                sub.data[sub_i][sub_j] = m.data[i][j];
                sub_j++;
            }
            sub_i++;
        }
        
        int sign = (col % 2 == 0) ? 1 : -1;
        int sub_det = recursive_determinant(sub);
        
        if (sub_det != 0) {
            if (m.data[0][col] > 0) {
                if (sub_det > 0 && sub_det > INT_MAX / m.data[0][col]) return 0;
                if (sub_det < 0 && sub_det < INT_MIN / m.data[0][col]) return 0;
            } else if (m.data[0][col] < 0) {
                if (sub_det > 0 && sub_det > INT_MIN / m.data[0][col]) return 0;
                if (sub_det < 0 && sub_det < INT_MAX / m.data[0][col]) return 0;
            }
        }
        
        long long term = (long long)sign * m.data[0][col] * sub_det;
        if (term > INT_MAX || term < INT_MIN) return 0;
        
        long long new_det = (long long)det + term;
        if (new_det > INT_MAX || new_det < INT_MIN) return 0;
        det = (int)new_det;
    }
    return det;
}

Matrix recursive_transpose(Matrix m) {
    Matrix result = create_matrix(m.cols, m.rows);
    if (result.rows == 0) return result;
    
    for (int i = 0; i < m.rows; i++) {
        for (int j = 0; j < m.cols; j++) {
            result.data[j][i] = m.data[i][j];
        }
    }
    return result;
}

int main() {
    printf("Matrix Operations with Recursion\n");
    printf("Enter matrix dimensions (rows cols, max %d): ", MAX_SIZE);
    
    int rows, cols;
    if (scanf("%d %d", &rows, &cols) != 2) {
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
    
    printf("\nTransposed matrix:\n");
    Matrix transposed = recursive_transpose(m);
    print