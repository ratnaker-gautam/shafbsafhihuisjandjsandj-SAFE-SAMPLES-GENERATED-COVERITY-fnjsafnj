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

int recursive_determinant(Matrix m, int size) {
    if (size == 1) {
        return m.data[0][0];
    }
    if (size == 2) {
        return m.data[0][0] * m.data[1][1] - m.data[0][1] * m.data[1][0];
    }
    
    int det = 0;
    int sign = 1;
    
    for (int col = 0; col < size; col++) {
        Matrix submatrix;
        submatrix.rows = size - 1;
        submatrix.cols = size - 1;
        
        int sub_i = 0;
        for (int i = 1; i < size; i++) {
            int sub_j = 0;
            for (int j = 0; j < size; j++) {
                if (j == col) continue;
                if (sub_i < submatrix.rows && sub_j < submatrix.cols) {
                    submatrix.data[sub_i][sub_j] = m.data[i][j];
                    sub_j++;
                }
            }
            sub_i++;
        }
        
        int minor = recursive_determinant(submatrix, size - 1);
        det += sign * m.data[0][col] * minor;
        sign = -sign;
    }
    
    return det;
}

int get_valid_int(const char* prompt, int min, int max) {
    int value;
    char buffer[100];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            continue;
        }
        
        if (sscanf(buffer, "%d", &value) != 1) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if (value < min || value > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        
        break;
    }
    return value;
}

void fill_matrix(Matrix* m) {
    printf("Enter matrix elements:\n");
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            char prompt[50];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            m->data[i][j] = get_valid_int(prompt, -100, 100);
        }
    }
}

int main() {
    printf("Matrix Determinant Calculator\n");
    printf("=============================\n");
    
    int size = get_valid_int("Enter matrix size (1-10): ", 1, MAX_SIZE);
    
    Matrix m = create_matrix(size, size);
    fill_matrix(&m);
    
    printf("\nYour matrix:\n");
    print_matrix(m);
    
    int det = recursive_determinant(m, size);
    printf("\nDeterminant: %d\n", det);
    
    return 0;
}