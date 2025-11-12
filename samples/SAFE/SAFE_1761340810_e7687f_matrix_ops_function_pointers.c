//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 8

typedef void (*MatrixOp)(int rows, int cols, int matrix[rows][cols]);

void matrix_add(int rows, int cols, int matrix[rows][cols]) {
    int scalar;
    printf("Enter scalar to add: ");
    if (scanf("%d", &scalar) != 1) {
        printf("Invalid input\n");
        return;
    }
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if ((scalar > 0 && matrix[i][j] > INT_MAX - scalar) ||
                (scalar < 0 && matrix[i][j] < INT_MIN - scalar)) {
                printf("Integer overflow detected\n");
                return;
            }
            matrix[i][j] += scalar;
        }
    }
}

void matrix_multiply(int rows, int cols, int matrix[rows][cols]) {
    int scalar;
    printf("Enter scalar to multiply: ");
    if (scanf("%d", &scalar) != 1) {
        printf("Invalid input\n");
        return;
    }
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (scalar != 0 && matrix[i][j] > INT_MAX / scalar) {
                printf("Integer overflow detected\n");
                return;
            }
            matrix[i][j] *= scalar;
        }
    }
}

void matrix_transpose(int rows, int cols, int matrix[rows][cols]) {
    if (rows != cols) {
        printf("Transpose requires square matrix\n");
        return;
    }
    
    for (int i = 0; i < rows; i++) {
        for (int j = i + 1; j < cols; j++) {
            int temp = matrix[i][j];
            matrix[i][j] = matrix[j][i];
            matrix[j][i] = temp;
        }
    }
}

void matrix_print(int rows, int cols, int matrix[rows][cols]) {
    printf("Matrix:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%4d ", matrix[i][j]);
        }
        printf("\n");
    }
}

int get_valid_int(const char* prompt, int min, int max) {
    int value;
    while (1) {
        printf("%s", prompt);
        if (scanf("%d", &value) != 1) {
            printf("Invalid input\n");
            while (getchar() != '\n');
            continue;
        }
        if (value >= min && value <= max) {
            return value;
        }
        printf("Value must be between %d and %d\n", min, max);
    }
}

void initialize_matrix(int rows, int cols, int matrix[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = i * cols + j + 1;
        }
    }
}

int main() {
    int rows = get_valid_int("Enter number of rows (1-8): ", 1, MAX_SIZE);
    int cols = get_valid_int("Enter number of columns (1-8): ", 1, MAX_SIZE);
    
    int matrix[MAX_SIZE][MAX_SIZE];
    initialize_matrix(rows, cols, matrix);
    
    MatrixOp operations[] = {
        matrix_add,
        matrix_multiply,
        matrix_transpose,
        matrix_print
    };
    
    const char* operation_names[] = {
        "Add scalar",
        "Multiply by scalar", 
        "Transpose",
        "Print matrix",
        "Exit"
    };
    
    int num_ops = sizeof(operations) / sizeof(operations[0]);
    
    while (1) {
        printf("\nAvailable operations:\n");
        for (int i = 0; i < num_ops; i++) {
            printf("%d. %s\n", i + 1, operation_names[i]);
        }
        printf("%d. %s\n", num_ops + 1, operation_names[num_ops]);
        
        int choice = get_valid_int("Select operation: ", 1, num_ops + 1);
        
        if (choice == num_ops + 1) {
            break;
        }
        
        if (choice >= 1 && choice <= num_ops) {
            operations[choice - 1](rows, cols, matrix);
        }
    }
    
    return 0;
}