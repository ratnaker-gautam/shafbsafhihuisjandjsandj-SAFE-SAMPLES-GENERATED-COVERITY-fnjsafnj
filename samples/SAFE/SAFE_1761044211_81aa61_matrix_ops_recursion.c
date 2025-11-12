//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: matrix_ops
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

int read_int(const char* prompt, int min, int max) {
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

void read_matrix_elements(Matrix* m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            char prompt[50];
            snprintf(prompt, sizeof(prompt), "Enter element [%d][%d]: ", i, j);
            m->data[i][j] = read_int(prompt, -1000, 1000);
        }
    }
}

int recursive_determinant(Matrix m) {
    if (m.rows != m.cols) {
        return 0;
    }
    if (m.rows == 1) {
        return m.data[0][0];
    }
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
        det += sign * m.data[0][col] * recursive_determinant(sub);
    }
    return det;
}

Matrix recursive_transpose(Matrix m, int row, int col) {
    Matrix result = create_matrix(m.cols, m.rows);
    if (row >= m.rows) {
        return result;
    }
    if (col >= m.cols) {
        return recursive_transpose(m, row + 1, 0);
    }
    result.data[col][row] = m.data[row][col];
    Matrix partial = recursive_transpose(m, row, col + 1);
    for (int i = 0; i < partial.rows; i++) {
        for (int j = 0; j < partial.cols; j++) {
            if (i == col && j == row) {
                result.data[i][j] = m.data[row][col];
            } else if (i < partial.rows && j < partial.cols) {
                result.data[i][j] = partial.data[i][j];
            }
        }
    }
    return result;
}

int main() {
    printf("Matrix Operations Program\n");
    int rows = read_int("Enter number of rows (1-10): ", 1, MAX_SIZE);
    int cols = read_int("Enter number of columns (1-10): ", 1, MAX_SIZE);
    
    Matrix m = create_matrix(rows, cols);
    read_matrix_elements(&m);
    
    printf("\nOriginal matrix:\n");
    print_matrix(m);
    
    if (rows == cols) {
        int det = recursive_determinant(m);
        printf("\nDeterminant: %d\n", det);
    } else {
        printf("\nMatrix is not square, cannot compute determinant.\n");
    }
    
    Matrix transposed = recursive_transpose(m, 0, 0);
    printf("\nTransposed matrix:\n");
    print_matrix(transposed);
    
    return 0;
}