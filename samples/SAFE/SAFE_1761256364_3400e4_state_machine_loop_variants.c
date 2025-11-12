//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

enum state {
    START,
    READ_NUM,
    READ_OP,
    READ_SECOND,
    CALCULATE,
    ERROR,
    EXIT
};

struct calculator {
    enum state current;
    int first_num;
    int second_num;
    char operation;
    int result;
};

int perform_calculation(int a, int b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': 
            if (b == 0) return 0;
            return a / b;
        default: return 0;
    }
}

int is_valid_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

int read_number(int *num) {
    char buffer[32];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return 0;
    
    char *endptr;
    long value = strtol(buffer, &endptr, 10);
    if (endptr == buffer || *endptr != '\n') return 0;
    if (value < -1000000 || value > 1000000) return 0;
    
    *num = (int)value;
    return 1;
}

int read_operator(char *op) {
    char buffer[8];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return 0;
    
    if (buffer[0] == '\n' || buffer[1] != '\n') return 0;
    if (!is_valid_operator(buffer[0])) return 0;
    
    *op = buffer[0];
    return 1;
}

int main(void) {
    struct calculator calc = {START, 0, 0, 0, 0};
    int running = 1;
    
    printf("Simple Calculator (enter 'q' to quit)\n");
    
    while (running) {
        switch (calc.current) {
            case START:
                printf("Enter first number: ");
                calc.current = READ_NUM;
                break;
                
            case READ_NUM:
                if (read_number(&calc.first_num)) {
                    calc.current = READ_OP;
                } else {
                    char buffer[8];
                    if (fgets(buffer, sizeof(buffer), stdin) && buffer[0] == 'q') {
                        calc.current = EXIT;
                    } else {
                        printf("Invalid number. Try again: ");
                    }
                }
                break;
                
            case READ_OP:
                printf("Enter operation (+, -, *, /): ");
                if (read_operator(&calc.operation)) {
                    calc.current = READ_SECOND;
                } else {
                    char buffer[8];
                    if (fgets(buffer, sizeof(buffer), stdin) && buffer[0] == 'q') {
                        calc.current = EXIT;
                    } else {
                        printf("Invalid operator. ");
                    }
                }
                break;
                
            case READ_SECOND:
                printf("Enter second number: ");
                if (read_number(&calc.second_num)) {
                    if (calc.operation == '/' && calc.second_num == 0) {
                        printf("Division by zero not allowed. ");
                        calc.current = READ_SECOND;
                    } else {
                        calc.current = CALCULATE;
                    }
                } else {
                    char buffer[8];
                    if (fgets(buffer, sizeof(buffer), stdin) && buffer[0] == 'q') {
                        calc.current = EXIT;
                    } else {
                        printf("Invalid number. Try again: ");
                    }
                }
                break;
                
            case CALCULATE:
                calc.result = perform_calculation(calc.first_num, calc.second_num, calc.operation);
                printf("%d %c %d = %d\n", calc.first_num, calc.operation, calc.second_num, calc.result);
                calc.current = START;
                break;
                
            case ERROR:
                printf("An error occurred. Resetting calculator.\n");
                calc.current = START;
                break;
                
            case EXIT:
                printf("Goodbye!\n");
                running = 0;
                break;
        }
    }
    
    return 0;
}