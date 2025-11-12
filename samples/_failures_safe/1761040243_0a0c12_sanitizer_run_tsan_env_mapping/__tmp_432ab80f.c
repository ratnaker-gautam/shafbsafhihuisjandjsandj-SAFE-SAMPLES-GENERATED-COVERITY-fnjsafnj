//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 10

int validate_int(const char *str, int *result) {
    char *endptr;
    long val = strtol(str, &endptr, 10);
    if (endptr == str || *endptr != '\0') return 0;
    if (val < INT_MIN || val > INT_MAX) return 0;
    *result = (int)val;
    return 1;
}

void read_matrix(int *matrix, int rows, int cols) {
    char buffer[32];
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("Enter element [%d][%d]: ", i, j);
            if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
                printf("Input error\n");
                exit(1);
            }
            buffer[strcspn(buffer, "\n")] = 0;
            int value;
            if (!validate_int(buffer, &value)) {
                printf("Invalid input\n");
                exit(1);
            }
            *(matrix + i * cols + j) = value;
        }
    }
}

void print_matrix(int *matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%4d", *(matrix + i * cols + j));
        }
        printf("\n");
    }
}

void multiply_matrices(int *a, int *b, int *result, int rows_a, int cols_a, int cols_b) {
    for (int i = 0; i < rows_a; i++) {
        for (int j = 0; j < cols_b; j++) {
            int sum = 0;
            for (int k = 0; k < cols_a; k++) {
                sum += *(a + i * cols_a + k) * *(b + k * cols_b + j);
            }
            *(result + i * cols_b + j) = sum;
        }
    }
}

int main() {
    int rows1, cols1, rows2, cols2;
    char buffer[32];

    printf("Enter rows for matrix 1 (1-%d): ", MAX_SIZE);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    buffer[strcspn(buffer, "\n")] = 0;
    if (!validate_int(buffer, &rows1) || rows1 < 1 || rows1 > MAX_SIZE) {
        printf("Invalid rows\n");
        return 1;
    }

    printf("Enter columns for matrix 1 (1-%d): ", MAX_SIZE);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    buffer[strcspn(buffer, "\n")] = 0;
    if (!validate_int(buffer, &cols1) || cols1 < 1 || cols1 > MAX_SIZE) {
        printf("Invalid columns\n");
        return 1;
    }

    printf("Enter rows for matrix 2 (1-%d): ", MAX_SIZE);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    buffer[strcspn(buffer, "\n")] = 0;
    if (!validate_int(buffer, &rows2) || rows2 < 1 || rows2 > MAX_SIZE) {
        printf("Invalid rows\n");
        return 1;
    }

    printf("Enter columns for matrix 2 (1-%d): ", MAX_SIZE);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    buffer[strcspn(buffer, "\n")] = 0;
    if (!validate_int(buffer, &cols2) || cols2 < 1 || cols2 > MAX_SIZE) {
        printf("Invalid columns\n");
        return 1;
    }

    if (cols1 != rows2) {
        printf("Matrix dimensions incompatible for multiplication\n");
        return 1;
    }

    int matrix1[MAX_SIZE * MAX_SIZE];
    int matrix2[MAX_SIZE * MAX_SIZE];
    int result[MAX_SIZE * MAX_SIZE];

    printf("Enter elements for matrix 1:\n");
    read_matrix(matrix1, rows1, cols1);

    printf("Enter elements for matrix 2:\n");
    read_matrix(matrix2, rows2, cols2);

    printf("Matrix 1:\n");
    print_matrix(matrix1, rows1, cols1);

    printf("Matrix 2:\n");
    print_matrix(matrix2, rows2, cols2);

    multiply_matrices(matrix1, matrix2, result, rows1, cols1, cols2);

    printf("Result of multiplication:\n");
    print_matrix(result, rows1, cols2);

    return 0;
}