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

int recursive_determinant(Matrix m, int size) {
    if (size == 1) {
        return m.data[0][0];
    }
    if (size == 2) {
        return m.data[0][0] * m.data[1][1] - m.data[0][1] * m.data[1][0];
    }
    int det = 0;
    int sign = 1;
    for (int i = 0; i < size; i++) {
        Matrix sub = create_matrix(size - 1, size - 1);
        for (int j = 1; j < size; j++) {
            int sub_col = 0;
            for (int k = 0; k < size; k++) {
                if (k == i) continue;
                sub.data[j-1][sub_col] = m.data[j][k];
                sub_col++;
            }
        }
        det += sign * m.data[0][i] * recursive_determinant(sub, size - 1);
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
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            char prompt[50];
            snprintf(prompt, sizeof(prompt), "Enter element [%d][%d]: ", i, j);
            m->data[i][j] = get_valid_int(prompt, -100, 100);
        }
    }
}

int main() {
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    int rows = get_valid_int("Enter number of rows (1-10): ", 1, MAX_SIZE);
    int cols = get_valid_int("Enter number of columns (1-10): ", 1, MAX_SIZE);
    
    Matrix original = create_matrix(rows, cols);
    printf("\nFill the matrix:\n");
    fill_matrix(&original);
    
    printf("\nOriginal matrix:\n");
    print_matrix(original);
    
    printf("\nTransposed matrix:\n");
    Matrix transposed = recursive_transpose(original, 0, 0);
    print_matrix(transposed);
    
    if (rows == cols) {
        int det = recursive_determinant(original, rows);
        printf("\nDeterminant: %d\n", det);
    } else {
        printf("\nDeterminant calculation requires a square matrix.\n");
    }
    
    return 0;
}