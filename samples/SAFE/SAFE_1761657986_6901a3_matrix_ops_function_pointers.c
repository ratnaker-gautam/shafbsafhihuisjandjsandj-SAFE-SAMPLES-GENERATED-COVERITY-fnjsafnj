//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: matrix_ops
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
            if (matrix[i][j] > INT_MAX - 5) {
                matrix[i][j] = INT_MAX;
            } else {
                matrix[i][j] += 5;
            }
        }
    }
}

void matrix_multiply(int rows, int cols, int matrix[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (matrix[i][j] > INT_MAX / 2) {
                matrix[i][j] = INT_MAX;
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
            printf("%6d", matrix[i][j]);
        }
        printf("\n");
    }
}

int safe_str_to_int(const char* str, int* result) {
    if (str == NULL) return 0;
    
    char* endptr;
    long val = strtol(str, &endptr, 10);
    
    if (endptr == str || *endptr != '\0') return 0;
    if (val < INT_MIN || val > INT_MAX) return 0;
    
    *result = (int)val;
    return 1;
}

int read_matrix(int rows, int cols, int matrix[rows][cols]) {
    char buffer[256];
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("Enter element [%d][%d]: ", i, j);
            
            if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
                return 0;
            }
            
            buffer[strcspn(buffer, "\n")] = '\0';
            
            int value;
            if (!safe_str_to_int(buffer, &value)) {
                printf("Invalid input. Please enter an integer.\n");
                j--;
                continue;
            }
            
            matrix[i][j] = value;
        }
    }
    return 1;
}

int main() {
    int rows, cols;
    char buffer[256];
    
    printf("Enter number of rows (1-%d): ", MAX_SIZE);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return 1;
    if (!safe_str_to_int(buffer, &rows) || rows < 1 || rows > MAX_SIZE) {
        printf("Invalid row count.\n");
        return 1;
    }
    
    printf("Enter number of columns (1-%d): ", MAX_SIZE);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return 1;
    if (!safe_str_to_int(buffer, &cols) || cols < 1 || cols > MAX_SIZE) {
        printf("Invalid column count.\n");
        return 1;
    }
    
    int matrix[MAX_SIZE][MAX_SIZE];
    
    printf("Enter matrix elements:\n");
    if (!read_matrix(rows, cols, matrix)) {
        printf("Error reading matrix.\n");
        return 1;
    }
    
    MatrixOp operations[] = {matrix_add, matrix_multiply, matrix_transpose};
    char* op_names[] = {"Add 5 to each element", "Multiply each element by 2", "Transpose matrix"};
    int num_ops = sizeof(operations) / sizeof(operations[0]);
    
    printf("\nOriginal matrix:\n");
    matrix_print(rows, cols, matrix);
    
    printf("\nAvailable operations:\n");
    for (int i = 0; i < num_ops; i++) {
        printf("%d. %s\n", i + 1, op_names[i]);
    }
    
    printf("Choose operation (1-%d): ", num_ops);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return 1;
    
    int choice;
    if (!safe_str_to_int(buffer, &choice) || choice < 1 || choice > num_ops) {
        printf("Invalid choice.\n");
        return 1;
    }
    
    MatrixOp selected_op = operations[choice - 1];
    
    int temp_matrix[MAX_SIZE][MAX_SIZE];
    memcpy(temp_matrix, matrix, sizeof(temp_matrix));
    
    selected_op(rows, cols, temp_matrix);
    
    printf("\nResult:\n");
    matrix_print(rows, cols, temp_matrix);
    
    return 0;
}