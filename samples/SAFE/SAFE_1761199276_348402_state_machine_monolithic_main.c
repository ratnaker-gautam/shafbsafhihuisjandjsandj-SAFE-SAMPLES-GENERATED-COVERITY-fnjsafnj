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
    printf("Commands: add, sub, mul, div, reset, exit\n");
    
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
                
                for (size_t i = 0; input_buffer[i]; i++) {
                    input_buffer[i] = tolower(input_buffer[i]);
                }
                
                if (strcmp(input_buffer, "exit") == 0) {
                    current_state = EXIT_STATE;
                } else if (strcmp(input_buffer, "reset") == 0) {
                    result = 0;
                    printf("Result reset to 0\n");
                    current_state = INIT;
                } else if (strcmp(input_buffer, "add") == 0 || 
                           strcmp(input_buffer, "sub") == 0 ||
                           strcmp(input_buffer, "mul") == 0 ||
                           strcmp(input_buffer, "div") == 0) {
                    current_state = PROCESS_NUMBER;
                } else {
                    printf("Invalid command. Try again.\n");
                    current_state = INIT;
                }
                break;
                
            case PROCESS_NUMBER:
                printf("Enter number: ");
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                    current_state = ERROR_STATE;
                    break;
                }
                
                valid_input = 0;
                if (sscanf(input_buffer, "%d", &number) == 1) {
                    valid_input = 1;
                }
                
                if (!valid_input) {
                    printf("Invalid number format.\n");
                    current_state = INIT;
                    break;
                }
                
                current_state = DISPLAY_RESULT;
                break;
                
            case DISPLAY_RESULT:
                if (strcmp(input_buffer, "add") == 0) {
                    if ((number > 0 && result > INT_MAX - number) || 
                        (number < 0 && result < INT_MIN - number)) {
                        printf("Integer overflow detected.\n");
                        current_state = INIT;
                        break;
                    }
                    result += number;
                } else if (strcmp(input_buffer, "sub") == 0) {
                    if ((number > 0 && result < INT_MIN + number) || 
                        (number < 0 && result > INT_MAX + number)) {
                        printf("Integer overflow detected.\n");
                        current_state = INIT;
                        break;
                    }
                    result -= number;
                } else if (strcmp(input_buffer, "mul") == 0) {
                    if (number != 0 && result != 0) {
                        if ((number > 0 && result > INT_MAX / number) || 
                            (number > 0 && result < INT_MIN / number) ||
                            (number < 0 && result < INT_MAX / number) ||
                            (number < 0 && result > INT_MIN / number)) {
                            printf("Integer overflow detected.\n");
                            current_state = INIT;
                            break;
                        }
                    }
                    result *= number;
                } else if (strcmp(input_buffer, "div") == 0) {
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
                
                printf("Current result: %d\n", result);
                current_state = INIT;
                break;
                
            case ERROR_STATE:
                printf("Input error occurred.\n");
                current_state = INIT;
                break;
                
            case EXIT_STATE:
                break;
        }
    }
    
    printf("Goodbye!\n");
    return 0;
}