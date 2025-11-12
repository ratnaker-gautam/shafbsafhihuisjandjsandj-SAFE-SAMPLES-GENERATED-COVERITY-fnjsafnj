//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

enum State {
    INIT,
    READ_COMMAND,
    PROCESS_NUMBER,
    DISPLAY_RESULT,
    ERROR_STATE,
    EXIT_STATE
};

int main(void) {
    enum State current_state = INIT;
    char input_buffer[256];
    int number = 0;
    int result = 0;
    int valid_input = 0;
    
    printf("Number Processor State Machine\n");
    printf("Commands: add, sub, mul, div, exit\n");
    
    while (current_state != EXIT_STATE) {
        switch (current_state) {
            case INIT:
                printf("Enter command: ");
                current_state = READ_COMMAND;
                break;
                
            case READ_COMMAND:
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                    current_state = ERROR_STATE;
                    break;
                }
                
                size_t len = strlen(input_buffer);
                if (len > 0 && input_buffer[len - 1] == '\n') {
                    input_buffer[len - 1] = '\0';
                }
                
                for (size_t i = 0; i < strlen(input_buffer); i++) {
                    input_buffer[i] = tolower(input_buffer[i]);
                }
                
                if (strcmp(input_buffer, "exit") == 0) {
                    current_state = EXIT_STATE;
                } else if (strcmp(input_buffer, "add") == 0 || 
                           strcmp(input_buffer, "sub") == 0 ||
                           strcmp(input_buffer, "mul") == 0 ||
                           strcmp(input_buffer, "div") == 0) {
                    strncpy(input_buffer, input_buffer, sizeof(input_buffer) - 1);
                    input_buffer[sizeof(input_buffer) - 1] = '\0';
                    printf("Enter number: ");
                    current_state = PROCESS_NUMBER;
                } else {
                    printf("Invalid command. Try again.\n");
                    current_state = INIT;
                }
                break;
                
            case PROCESS_NUMBER:
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                    current_state = ERROR_STATE;
                    break;
                }
                
                char *endptr;
                number = strtol(input_buffer, &endptr, 10);
                
                if (endptr == input_buffer || *endptr != '\n') {
                    printf("Invalid number format.\n");
                    current_state = INIT;
                } else {
                    valid_input = 1;
                    current_state = DISPLAY_RESULT;
                }
                break;
                
            case DISPLAY_RESULT:
                if (!valid_input) {
                    current_state = ERROR_STATE;
                    break;
                }
                
                char operation[4];
                if (strcmp(input_buffer, "add") == 0) {
                    strcpy(operation, "add");
                    if ((number > 0 && result > INT_MAX - number) || 
                        (number < 0 && result < INT_MIN - number)) {
                        printf("Integer overflow detected.\n");
                        current_state = INIT;
                        break;
                    }
                    result += number;
                } else if (strcmp(input_buffer, "sub") == 0) {
                    strcpy(operation, "sub");
                    if ((number < 0 && result > INT_MAX + number) || 
                        (number > 0 && result < INT_MIN + number)) {
                        printf("Integer overflow detected.\n");
                        current_state = INIT;
                        break;
                    }
                    result -= number;
                } else if (strcmp(input_buffer, "mul") == 0) {
                    strcpy(operation, "mul");
                    if (number > 0 && result > 0 && result > INT_MAX / number) {
                        printf("Integer overflow detected.\n");
                        current_state = INIT;
                        break;
                    }
                    if (number > 0 && result < 0 && result < INT_MIN / number) {
                        printf("Integer overflow detected.\n");
                        current_state = INIT;
                        break;
                    }
                    if (number < 0 && result > 0 && result > INT_MIN / number) {
                        printf("Integer overflow detected.\n");
                        current_state = INIT;
                        break;
                    }
                    if (number < 0 && result < 0 && result < INT_MAX / number) {
                        printf("Integer overflow detected.\n");
                        current_state = INIT;
                        break;
                    }
                    result *= number;
                } else if (strcmp(input_buffer, "div") == 0) {
                    strcpy(operation, "div");
                    if (number == 0) {
                        printf("Division by zero error.\n");
                        current_state = INIT;
                        break;
                    }
                    if (number == -1 && result == INT_MIN) {
                        printf("Integer overflow detected.\n");
                        current_state = INIT;
                        break;
                    }
                    result /= number;
                }
                
                printf("Operation %s %d: result = %d\n", operation, number, result);
                current_state = INIT;
                break;
                
            case ERROR_STATE:
                printf("An error occurred. Resetting...\n");
                result = 0;
                valid_input = 0;
                current_state = INIT;
                break;
                
            case EXIT_STATE:
                printf("Final result: %d\n", result);
                printf("Goodbye!\n");
                break;
        }
    }
    
    return 0;
}