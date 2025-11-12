//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <limits.h>

enum operation {
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_TRANSPOSE,
    OP_DETERMINANT
};

void print_matrix(int rows, int cols, int matrix[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%6d", matrix[i][j]);
        }
        printf("\n");
    }
}

int get_valid_int(const char* prompt, int min_val, int max_val) {
    int value;
    char buffer[100];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            continue;
        }
        
        if (sscanf(buffer, "%d", &value) != 1) {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (value < min_val || value > max_val) {
            printf("Value must be between %d and %d.\n", min_val, max_val);
            continue;
        }
        
        return value;
    }
}

void get_matrix_input(int rows, int cols, int matrix[rows][cols], const char* name) {
    printf("Enter values for matrix %s (%dx%d):\n", name, rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            char prompt[50];
            snprintf(prompt, sizeof(prompt), "  [%d][%d]: ", i, j);
            matrix[i][j] = get_valid_int(prompt, -1000, 1000);
        }
    }
}

int calculate_determinant(int size, int matrix[size][size]) {
    if (size == 1) {
        return matrix[0][0];
    }
    if (size == 2) {
        return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
    }
    
    int det = 0;
    for (int col = 0; col < size; col++) {
        int submatrix[size-1][size-1];
        int sub_i = 0;
        
        for (int i = 1; i < size; i++) {
            int sub_j = 0;
            for (int j = 0; j < size; j++) {
                if (j != col) {
                    submatrix[sub_i][sub_j] = matrix[i][j];
                    sub_j++;
                }
            }
            sub_i++;
        }
        
        int sign = (col % 2 == 0) ? 1 : -1;
        int sub_det = calculate_determinant(size-1, submatrix);
        
        if (sub_det > 0 && sign > 0 && det > INT_MAX - sub_det) {
            return 0;
        }
        if (sub_det < 0 && sign < 0 && det < INT_MIN - sub_det) {
            return 0;
        }
        
        det += sign * matrix[0][col] * sub_det;
    }
    
    return det;
}

int main(void) {
    enum operation op;
    int choice;
    
    printf("Matrix Operations Program\n");
    printf("Available operations:\n");
    printf("1. Add\n");
    printf("2. Subtract\n");
    printf("3. Multiply\n");
    printf("4. Transpose\n");
    printf("5. Determinant\n");
    
    choice = get_valid_int("Select operation (1-5): ", 1, 5);
    op = (enum operation)(choice - 1);
    
    switch (op) {
        case OP_ADD:
        case OP_SUBTRACT: {
            int rows = get_valid_int("Enter number of rows: ", 1, 10);
            int cols = get_valid_int("Enter number of columns: ", 1, 10);
            
            int matrix1[rows][cols];
            int matrix2[rows][cols];
            int result[rows][cols];
            
            get_matrix_input(rows, cols, matrix1, "A");
            get_matrix_input(rows, cols, matrix2, "B");
            
            printf("\nMatrix A:\n");
            print_matrix(rows, cols, matrix1);
            printf("\nMatrix B:\n");
            print_matrix(rows, cols, matrix2);
            
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                    if (op == OP_ADD) {
                        if (matrix1[i][j] > 0 && matrix2[i][j] > 0 && 
                            matrix1[i][j] > INT_MAX - matrix2[i][j]) {
                            result[i][j] = INT_MAX;
                        } else if (matrix1[i][j] < 0 && matrix2[i][j] < 0 && 
                                   matrix1[i][j] < INT_MIN - matrix2[i][j]) {
                            result[i][j] = INT_MIN;
                        } else {
                            result[i][j] = matrix1[i][j] + matrix2[i][j];
                        }
                    } else {
                        if (matrix1[i][