//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 10

typedef void (*MatrixOperation)(int rows, int cols, int matrix[MAX_SIZE][MAX_SIZE]);

void transpose(int rows, int cols, int matrix[MAX_SIZE][MAX_SIZE]) {
    int temp[MAX_SIZE][MAX_SIZE];
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            temp[j][i] = matrix[i][j];
        }
    }
    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < rows; j++) {
            matrix[i][j] = temp[i][j];
        }
    }
}

void scale(int rows, int cols, int matrix[MAX_SIZE][MAX_SIZE]) {
    int factor;
    printf("Enter scaling factor: ");
    if (scanf("%d", &factor) != 1) {
        printf("Invalid input\n");
        return;
    }
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if ((factor > 0 && matrix[i][j] > INT_MAX / factor) || 
                (factor < 0 && matrix[i][j] < INT_MIN / factor)) {
                printf("Integer overflow detected\n");
                return;
            }
            matrix[i][j] *= factor;
        }
    }
}

void add_constant(int rows, int cols, int matrix[MAX_SIZE][MAX_SIZE]) {
    int constant;
    printf("Enter constant to add: ");
    if (scanf("%d", &constant) != 1) {
        printf("Invalid input\n");
        return;
    }
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if ((constant > 0 && matrix[i][j] > INT_MAX - constant) || 
                (constant < 0 && matrix[i][j] < INT_MIN - constant)) {
                printf("Integer overflow detected\n");
                return;
            }
            matrix[i][j] += constant;
        }
    }
}

void print_matrix(int rows, int cols, int matrix[MAX_SIZE][MAX_SIZE]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%4d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void initialize_matrix(int rows, int cols, int matrix[MAX_SIZE][MAX_SIZE]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = i * cols + j + 1;
        }
    }
}

int main() {
    int rows, cols;
    printf("Enter matrix dimensions (rows cols, max %d): ", MAX_SIZE);
    if (scanf("%d %d", &rows, &cols) != 2) {
        printf("Invalid dimensions\n");
        return 1;
    }
    if (rows <= 0 || cols <= 0 || rows > MAX_SIZE || cols > MAX_SIZE) {
        printf("Invalid dimensions\n");
        return 1;
    }

    int matrix[MAX_SIZE][MAX_SIZE];
    initialize_matrix(rows, cols, matrix);

    MatrixOperation operations[] = {transpose, scale, add_constant};
    char* names[] = {"Transpose", "Scale", "Add Constant"};
    int num_operations = sizeof(operations) / sizeof(operations[0]);

    while (1) {
        printf("\nCurrent matrix:\n");
        print_matrix(rows, cols, matrix);

        printf("\nAvailable operations:\n");
        for (int i = 0; i < num_operations; i++) {
            printf("%d. %s\n", i + 1, names[i]);
        }
        printf("%d. Exit\n", num_operations + 1);

        int choice;
        printf("Select operation: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid choice\n");
            while (getchar() != '\n');
            continue;
        }

        if (choice == num_operations + 1) {
            break;
        }

        if (choice < 1 || choice > num_operations) {
            printf("Invalid choice\n");
            continue;
        }

        MatrixOperation op = operations[choice - 1];
        if (choice == 1) {
            if (rows != cols) {
                printf("Transpose requires square matrix\n");
                continue;
            }
        }
        op(rows, cols, matrix);
        if (choice == 1) {
            int temp = rows;
            rows = cols;
            cols = temp;
        }
    }

    return 0;
}