//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 100

int read_int(const char *prompt, int min, int max) {
    char buffer[32];
    int value;
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return min;
        }
        if (sscanf(buffer, "%d", &value) != 1) {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        if (value < min || value > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        return value;
    }
}

void fill_matrix(int *matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            *(matrix + i * cols + j) = read_int("Enter element: ", INT_MIN, INT_MAX);
        }
    }
}

void print_matrix(int *matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%6d ", *(matrix + i * cols + j));
        }
        printf("\n");
    }
}

void transpose_matrix(int *src, int *dest, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            *(dest + j * rows + i) = *(src + i * cols + j);
        }
    }
}

int multiply_matrices(int *a, int a_rows, int a_cols, int *b, int b_rows, int b_cols, int *result) {
    if (a_cols != b_rows) {
        return 0;
    }
    for (int i = 0; i < a_rows; i++) {
        for (int j = 0; j < b_cols; j++) {
            int sum = 0;
            for (int k = 0; k < a_cols; k++) {
                sum += *(a + i * a_cols + k) * *(b + k * b_cols + j);
            }
            *(result + i * b_cols + j) = sum;
        }
    }
    return 1;
}

int main() {
    int choice;
    int rows1, cols1, rows2, cols2;
    int matrix1[MAX_DIM * MAX_DIM];
    int matrix2[MAX_DIM * MAX_DIM];
    int result[MAX_DIM * MAX_DIM];

    while (1) {
        printf("\nMatrix Operations Menu:\n");
        printf("1. Transpose Matrix\n");
        printf("2. Multiply Matrices\n");
        printf("3. Exit\n");
        choice = read_int("Enter choice: ", 1, 3);

        if (choice == 3) {
            break;
        }

        if (choice == 1) {
            rows1 = read_int("Enter rows: ", 1, MAX_DIM);
            cols1 = read_int("Enter columns: ", 1, MAX_DIM);
            printf("Enter matrix elements:\n");
            fill_matrix(matrix1, rows1, cols1);
            printf("Original matrix:\n");
            print_matrix(matrix1, rows1, cols1);
            transpose_matrix(matrix1, result, rows1, cols1);
            printf("Transposed matrix:\n");
            print_matrix(result, cols1, rows1);
        } else if (choice == 2) {
            rows1 = read_int("Enter rows for matrix 1: ", 1, MAX_DIM);
            cols1 = read_int("Enter columns for matrix 1: ", 1, MAX_DIM);
            printf("Enter matrix 1 elements:\n");
            fill_matrix(matrix1, rows1, cols1);

            rows2 = read_int("Enter rows for matrix 2: ", 1, MAX_DIM);
            cols2 = read_int("Enter columns for matrix 2: ", 1, MAX_DIM);
            printf("Enter matrix 2 elements:\n");
            fill_matrix(matrix2, rows2, cols2);

            if (multiply_matrices(matrix1, rows1, cols1, matrix2, rows2, cols2, result)) {
                printf("Matrix 1:\n");
                print_matrix(matrix1, rows1, cols1);
                printf("Matrix 2:\n");
                print_matrix(matrix2, rows2, cols2);
                printf("Product:\n");
                print_matrix(result, rows1, cols2);
            } else {
                printf("Matrix dimensions incompatible for multiplication.\n");
            }
        }
    }

    return 0;
}