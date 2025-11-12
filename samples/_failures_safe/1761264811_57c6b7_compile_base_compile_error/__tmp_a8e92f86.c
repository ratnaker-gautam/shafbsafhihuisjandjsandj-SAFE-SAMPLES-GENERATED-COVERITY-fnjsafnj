//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum Operation {
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_TRANSPOSE,
    OP_INVALID
};

enum Operation get_operation(const char* input) {
    if (strcmp(input, "add") == 0) return OP_ADD;
    if (strcmp(input, "subtract") == 0) return OP_SUBTRACT;
    if (strcmp(input, "multiply") == 0) return OP_MULTIPLY;
    if (strcmp(input, "transpose") == 0) return OP_TRANSPOSE;
    return OP_INVALID;
}

void print_matrix(int rows, int cols, int matrix[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

int read_matrix(int rows, int cols, int matrix[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (scanf("%d", &matrix[i][j]) != 1) {
                return 0;
            }
        }
    }
    return 1;
}

int main(void) {
    int rows1, cols1, rows2, cols2;
    char op_str[16];
    
    printf("Enter operation (add/subtract/multiply/transpose): ");
    if (scanf("%15s", op_str) != 1) {
        printf("Error reading operation\n");
        return 1;
    }
    
    enum Operation op = get_operation(op_str);
    if (op == OP_INVALID) {
        printf("Invalid operation\n");
        return 1;
    }
    
    if (op == OP_TRANSPOSE) {
        printf("Enter matrix dimensions (rows cols): ");
        if (scanf("%d %d", &rows1, &cols1) != 2 || rows1 <= 0 || cols1 <= 0) {
            printf("Invalid dimensions\n");
            return 1;
        }
        
        int matrix[rows1][cols1];
        printf("Enter matrix elements:\n");
        if (!read_matrix(rows1, cols1, matrix)) {
            printf("Error reading matrix\n");
            return 1;
        }
        
        printf("Transposed matrix:\n");
        for (int j = 0; j < cols1; j++) {
            for (int i = 0; i < rows1; i++) {
                printf("%d ", matrix[i][j]);
            }
            printf("\n");
        }
        
        return 0;
    }
    
    printf("Enter first matrix dimensions (rows cols): ");
    if (scanf("%d %d", &rows1, &cols1) != 2 || rows1 <= 0 || cols1 <= 0) {
        printf("Invalid dimensions\n");
        return 1;
    }
    
    printf("Enter second matrix dimensions (rows cols): ");
    if (scanf("%d %d", &rows2, &cols2) != 2 || rows2 <= 0 || cols2 <= 0) {
        printf("Invalid dimensions\n");
        return 1;
    }
    
    switch (op) {
        case OP_ADD:
        case OP_SUBTRACT:
            if (rows1 != rows2 || cols1 != cols2) {
                printf("Matrix dimensions must match for addition/subtraction\n");
                return 1;
            }
            break;
        case OP_MULTIPLY:
            if (cols1 != rows2) {
                printf("Invalid dimensions for multiplication\n");
                return 1;
            }
            break;
        default:
            break;
    }
    
    int matrix1[rows1][cols1];
    int matrix2[rows2][cols2];
    
    printf("Enter first matrix elements:\n");
    if (!read_matrix(rows1, cols1, matrix1)) {
        printf("Error reading first matrix\n");
        return 1;
    }
    
    printf("Enter second matrix elements:\n");
    if (!read_matrix(rows2, cols2, matrix2)) {
        printf("Error reading second matrix\n");
        return 1;
    }
    
    switch (op) {
        case OP_ADD: {
            printf("Result:\n");
            for (int i = 0; i < rows1; i++) {
                for (int j = 0; j < cols1; j++) {
                    printf("%d ", matrix1[i][j] + matrix2[i][j]);
                }
                printf("\n");
            }
            break;
        }
        case OP_SUBTRACT: {
            printf("Result:\n");
            for (int i = 0; i < rows1; i++) {
                for (int j = 0; j < cols1; j++) {
                    printf("%d ", matrix1[i][j] - matrix2[i][j]);
                }
                printf("\n");
            }
            break;
        }
        case OP_MULTIPLY: {
            printf("Result:\n");
            for (int i = 0; i < rows1; i++) {
                for (int j = 0; j < cols2; j++) {
                    int sum = 0;
                    for (int k = 0; k < cols1; k++) {
                        sum += matrix1[i][k] * matrix2[k