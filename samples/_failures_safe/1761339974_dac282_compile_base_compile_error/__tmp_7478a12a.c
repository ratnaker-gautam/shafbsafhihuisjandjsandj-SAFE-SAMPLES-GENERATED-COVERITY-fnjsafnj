//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 100

int main(void) {
    int rows, cols;
    printf("Enter matrix dimensions (rows columns): ");
    if (scanf("%d %d", &rows, &cols) != 2) {
        fprintf(stderr, "Error: Invalid input\n");
        return 1;
    }
    if (rows <= 0 || cols <= 0 || rows > MAX_DIM || cols > MAX_DIM) {
        fprintf(stderr, "Error: Dimensions out of range\n");
        return 1;
    }

    int matrix1[MAX_DIM][MAX_DIM];
    int matrix2[MAX_DIM][MAX_DIM];
    int result[MAX_DIM][MAX_DIM];

    printf("Enter elements of first matrix:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (scanf("%d", &matrix1[i][j]) != 1) {
                fprintf(stderr, "Error: Invalid matrix element\n");
                return 1;
            }
        }
    }

    printf("Enter elements of second matrix:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (scanf("%d", &matrix2[i][j]) != 1) {
                fprintf(stderr, "Error: Invalid matrix element\n");
                return 1;
            }
        }
    }

    printf("Select operation (1=add, 2=subtract, 3=multiply): ");
    int op;
    if (scanf("%d", &op) != 1) {
        fprintf(stderr, "Error: Invalid operation\n");
        return 1;
    }

    if (op == 1 || op == 2) {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (op == 1) {
                    if ((matrix2[i][j] > 0 && matrix1[i][j] > INT_MAX - matrix2[i][j]) ||
                        (matrix2[i][j] < 0 && matrix1[i][j] < INT_MIN - matrix2[i][j])) {
                        fprintf(stderr, "Error: Integer overflow\n");
                        return 1;
                    }
                    result[i][j] = matrix1[i][j] + matrix2[i][j];
                } else {
                    if ((matrix2[i][j] < 0 && matrix1[i][j] > INT_MAX + matrix2[i][j]) ||
                        (matrix2[i][j] > 0 && matrix1[i][j] < INT_MIN + matrix2[i][j])) {
                        fprintf(stderr, "Error: Integer overflow\n");
                        return 1;
                    }
                    result[i][j] = matrix1[i][j] - matrix2[i][j];
                }
            }
        }
    } else if (op == 3) {
        int inner;
        printf("Enter inner dimension for multiplication: ");
        if (scanf("%d", &inner) != 1) {
            fprintf(stderr, "Error: Invalid inner dimension\n");
            return 1;
        }
        if (inner <= 0 || inner > MAX_DIM || cols != inner) {
            fprintf(stderr, "Error: Invalid inner dimension\n");
            return 1;
        }
        int matrix3[MAX_DIM][MAX_DIM];
        printf("Enter second matrix for multiplication (%dx%d):\n", cols, inner);
        for (int i = 0; i < cols; i++) {
            for (int j = 0; j < inner; j++) {
                if (scanf("%d", &matrix3[i][j]) != 1) {
                    fprintf(stderr, "Error: Invalid matrix element\n");
                    return 1;
                }
            }
        }
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < inner; j++) {
                long long sum = 0;
                for (int k = 0; k < cols; k++) {
                    long long product = (long long)matrix1[i][k] * matrix3[k][j];
                    if (matrix1[i][k] != 0 && product / matrix1[i][k] != matrix3[k][j]) {
                        fprintf(stderr, "Error: Integer overflow\n");
                        return 1;
                    }
                    if ((product > 0 && sum > LLONG_MAX - product) ||
                        (product < 0 && sum < LLONG_MIN - product)) {
                        fprintf(stderr, "Error: Integer overflow\n");
                        return 1;
                    }
                    sum += product;
                }
                if (sum > INT_MAX || sum < INT_MIN) {
                    fprintf(stderr, "Error: Integer overflow\n");
                    return 1;
                }
                result[i][j] = (int)sum;
            }
        }
        cols = inner;
    } else {
        fprintf(stderr, "Error: Invalid operation\n");
        return 1;
    }

    printf("Result matrix:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", result[i][j]);
        }
        printf("\n");
    }

    return 0