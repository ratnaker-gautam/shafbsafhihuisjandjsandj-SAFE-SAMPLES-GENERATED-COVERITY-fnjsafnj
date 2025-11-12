//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: function_pointers ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 8

typedef void (*MatrixOp)(int rows, int cols, int matrix[rows][cols]);

void matrix_add(int rows, int cols, int matrix[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] += 1;
        }
    }
}

void matrix_multiply(int rows, int cols, int matrix[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (matrix[i][j] > INT_MAX / 2) {
                matrix[i][j] = 1;
            } else {
                matrix[i][j] *= 2;
            }
        }
    }
}

void matrix_transpose(int rows, int cols, int matrix[rows][cols]) {
    if (rows != cols) return;
    
    for (int i = 0; i < rows; i++) {
        for (int j = i + 1; j < cols; j++) {
            int temp = matrix[i][j];
            matrix[i][j] = matrix[j][i];
            matrix[j][i] = temp;
        }
    }
}

void matrix_print(int rows, int cols, int matrix[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%4d", matrix[i][j]);
        }
        printf("\n");
    }
}

int safe_strtol(const char *str, int *result) {
    if (str == NULL) return 0;
    
    char *endptr;
    long val = strtol(str, &endptr, 10);
    
    if (endptr == str || *endptr != '\0') return 0;
    if (val < INT_MIN || val > INT_MAX) return 0;
    
    *result = (int)val;
    return 1;
}

int read_matrix(int rows, int cols, int matrix[rows][cols]) {
    char buffer[32];
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("Enter element [%d][%d]: ", i, j);
            
            if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
                return 0;
            }
            
            buffer[strcspn(buffer, "\n")] = '\0';
            
            int value;
            if (!safe_strtol(buffer, &value)) {
                printf("Invalid input. Using 0.\n");
                value = 0;
            }
            
            matrix[i][j] = value;
        }
    }
    return 1;
}

int main() {
    int rows, cols;
    char buffer[32];
    
    printf("Enter number of rows (1-%d): ", MAX_SIZE);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return 1;
    buffer[strcspn(buffer, "\n")] = '\0';
    if (!safe_strtol(buffer, &rows) || rows < 1 || rows > MAX_SIZE) {
        printf("Invalid rows. Using 3.\n");
        rows = 3;
    }
    
    printf("Enter number of columns (1-%d): ", MAX_SIZE);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return 1;
    buffer[strcspn(buffer, "\n")] = '\0';
    if (!safe_strtol(buffer, &cols) || cols < 1 || cols > MAX_SIZE) {
        printf("Invalid columns. Using 3.\n");
        cols = 3;
    }
    
    int matrix[MAX_SIZE][MAX_SIZE];
    
    printf("Enter matrix elements:\n");
    if (!read_matrix(rows, cols, matrix)) {
        printf("Error reading matrix.\n");
        return 1;
    }
    
    MatrixOp operations[] = {matrix_add, matrix_multiply, matrix_transpose};
    const char *op_names[] = {"Add 1 to each element", "Multiply each element by 2", "Transpose matrix"};
    int num_ops = sizeof(operations) / sizeof(operations[0]);
    
    printf("\nOriginal matrix:\n");
    matrix_print(rows, cols, matrix);
    
    for (int i = 0; i < num_ops; i++) {
        printf("\nOperation %d: %s\n", i + 1, op_names[i]);
        
        int temp_matrix[MAX_SIZE][MAX_SIZE];
        memcpy(temp_matrix, matrix, sizeof(temp_matrix));
        
        operations[i](rows, cols, temp_matrix);
        matrix_print(rows, cols, temp_matrix);
    }
    
    return 0;
}