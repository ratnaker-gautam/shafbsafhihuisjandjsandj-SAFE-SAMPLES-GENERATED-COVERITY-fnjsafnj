//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>

enum Operation {
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_TRANSPOSE,
    OP_EXIT
};

void print_matrix(int rows, int cols, int matrix[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%4d", matrix[i][j]);
        }
        printf("\n");
    }
}

void read_matrix(int rows, int cols, int matrix[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (scanf("%d", &matrix[i][j]) != 1) {
                printf("Invalid input\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

int main(void) {
    enum Operation op;
    int choice;
    int rows1, cols1, rows2, cols2;
    
    while (1) {
        printf("Matrix Operations:\n");
        printf("0: Add\n");
        printf("1: Subtract\n");
        printf("2: Multiply\n");
        printf("3: Transpose\n");
        printf("4: Exit\n");
        printf("Select operation: ");
        
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input\n");
            return EXIT_FAILURE;
        }
        
        if (choice < 0 || choice > 4) {
            printf("Invalid choice\n");
            continue;
        }
        
        op = (enum Operation)choice;
        
        if (op == OP_ADD || op == OP_SUBTRACT) {
            printf("Enter rows and columns for matrices: ");
            if (scanf("%d %d", &rows1, &cols1) != 2 || rows1 <= 0 || cols1 <= 0) {
                printf("Invalid dimensions\n");
                continue;
            }
            rows2 = rows1;
            cols2 = cols1;
            
            int mat1[rows1][cols1];
            int mat2[rows2][cols2];
            int result[rows1][cols1];
            
            printf("Enter first matrix:\n");
            read_matrix(rows1, cols1, mat1);
            printf("Enter second matrix:\n");
            read_matrix(rows2, cols2, mat2);
            
            for (int i = 0; i < rows1; i++) {
                for (int j = 0; j < cols1; j++) {
                    if (op == OP_ADD) {
                        result[i][j] = mat1[i][j] + mat2[i][j];
                    } else {
                        result[i][j] = mat1[i][j] - mat2[i][j];
                    }
                }
            }
            
            printf("Result:\n");
            print_matrix(rows1, cols1, result);
        } else if (op == OP_MULTIPLY) {
            printf("Enter rows and columns for first matrix: ");
            if (scanf("%d %d", &rows1, &cols1) != 2 || rows1 <= 0 || cols1 <= 0) {
                printf("Invalid dimensions\n");
                continue;
            }
            printf("Enter columns for second matrix: ");
            if (scanf("%d", &cols2) != 1 || cols2 <= 0) {
                printf("Invalid dimensions\n");
                continue;
            }
            rows2 = cols1;
            
            int matm1[rows1][cols1];
            int matm2[rows2][cols2];
            int resultm[rows1][cols2];
            
            printf("Enter first matrix:\n");
            read_matrix(rows1, cols1, matm1);
            printf("Enter second matrix:\n");
            read_matrix(rows2, cols2, matm2);
            
            for (int i = 0; i < rows1; i++) {
                for (int j = 0; j < cols2; j++) {
                    resultm[i][j] = 0;
                    for (int k = 0; k < cols1; k++) {
                        resultm[i][j] += matm1[i][k] * matm2[k][j];
                    }
                }
            }
            
            printf("Result:\n");
            print_matrix(rows1, cols2, resultm);
        } else if (op == OP_TRANSPOSE) {
            printf("Enter rows and columns for matrix: ");
            if (scanf("%d %d", &rows1, &cols1) != 2 || rows1 <= 0 || cols1 <= 0) {
                printf("Invalid dimensions\n");
                continue;
            }
            
            int matt[rows1][cols1];
            int resultt[cols1][rows1];
            
            printf("Enter matrix:\n");
            read_matrix(rows1, cols1, matt);
            
            for (int i = 0; i < rows1; i++) {
                for (int j = 0; j < cols1; j++) {
                    resultt[j][i] = matt[i][j];
                }
            }
            
            printf("Transpose:\n");
            print_matrix(cols1, rows1, resultt);
        } else if (op == OP_EXIT) {
            return EXIT_SUCCESS;
        }
    }
    
    return EXIT_SUCCESS;
}