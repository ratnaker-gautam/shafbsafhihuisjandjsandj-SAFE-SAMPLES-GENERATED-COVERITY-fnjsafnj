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
    DONE
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
            if (b != 0) return a / b;
            else return 0;
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

char read_operator(void) {
    char buffer[32];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return 0;
    
    if (buffer[0] != '\0' && buffer[1] == '\n' && is_valid_operator(buffer[0])) {
        return buffer[0];
    }
    return 0;
}

int main(void) {
    struct calculator calc = {START, 0, 0, 0, 0};
    int running = 1;
    
    printf("Simple Calculator (enter 'q' to quit)\n");
    
    while (running) {
        switch (calc.current) {
            case START:
                printf("Enter first number: ");
                if (read_number(&calc.first_num)) {
                    calc.current = READ_OP;
                } else {
                    calc.current = ERROR;
                }
                break;
                
            case READ_OP:
                printf("Enter operation (+, -, *, /): ");
                calc.operation = read_operator();
                if (calc.operation) {
                    calc.current = READ_SECOND;
                } else {
                    calc.current = ERROR;
                }
                break;
                
            case READ_SECOND:
                printf("Enter second number: ");
                if (read_number(&calc.second_num)) {
                    if (calc.operation == '/' && calc.second_num == 0) {
                        calc.current = ERROR;
                    } else {
                        calc.current = CALCULATE;
                    }
                } else {
                    calc.current = ERROR;
                }
                break;
                
            case CALCULATE:
                calc.result = perform_calculation(calc.first_num, calc.second_num, calc.operation);
                printf("Result: %d %c %d = %d\n", calc.first_num, calc.operation, calc.second_num, calc.result);
                calc.current = DONE;
                break;
                
            case ERROR:
                printf("Invalid input. Starting over.\n");
                calc.current = START;
                break;
                
            case DONE:
                printf("Calculate again? (y/n): ");
                char buffer[32];
                if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
                    if (buffer[0] == 'y' || buffer[0] == 'Y') {
                        calc.current = START;
                    } else if (buffer[0] == 'n' || buffer[0] == 'N') {
                        running = 0;
                    } else {
                        calc.current = ERROR;
                    }
                } else {
                    running = 0;
                }
                break;
        }
        
        char ch;
        while ((ch = getchar()) != '\n' && ch != EOF);
    }
    
    printf("Goodbye!\n");
    return 0;
}