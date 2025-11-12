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
    char command_buffer[256];
    int number = 0;
    int result = 0;
    
    printf("Number Processor - Commands: add, sub, mul, div, quit\n");
    
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
                    len--;
                }
                
                for (size_t i = 0; i < len; i++) {
                    input_buffer[i] = tolower(input_buffer[i]);
                }
                
                if (strcmp(input_buffer, "quit") == 0) {
                    current_state = EXIT_STATE;
                } else if (strcmp(input_buffer, "add") == 0 || 
                          strcmp(input_buffer, "sub") == 0 ||
                          strcmp(input_buffer, "mul") == 0 ||
                          strcmp(input_buffer, "div") == 0) {
                    strncpy(command_buffer, input_buffer, sizeof(command_buffer) - 1);
                    command_buffer[sizeof(command_buffer) - 1] = '\0';
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
                
                char *endptr;
                long num_long = strtol(input_buffer, &endptr, 10);
                if (endptr == input_buffer || (*endptr != '\n' && *endptr != '\0')) {
                    printf("Invalid number format.\n");
                    current_state = INIT;
                    break;
                }
                
                if (num_long < INT_MIN || num_long > INT_MAX) {
                    printf("Number out of range.\n");
                    current_state = INIT;
                    break;
                }
                number = (int)num_long;
                
                if (strcmp(command_buffer, "add") == 0) {
                    if ((number > 0 && result > INT_MAX - number) || 
                        (number < 0 && result < INT_MIN - number)) {
                        printf("Integer overflow detected.\n");
                        current_state = ERROR_STATE;
                        break;
                    }
                    result += number;
                } else if (strcmp(command_buffer, "sub") == 0) {
                    if ((number < 0 && result > INT_MAX + number) || 
                        (number > 0 && result < INT_MIN + number)) {
                        printf("Integer overflow detected.\n");
                        current_state = ERROR_STATE;
                        break;
                    }
                    result -= number;
                } else if (strcmp(command_buffer, "mul") == 0) {
                    if (number != 0) {
                        if ((result > 0 && number > 0 && result > INT_MAX / number) ||
                            (result > 0 && number < 0 && number < INT_MIN / result) ||
                            (result < 0 && number > 0 && result < INT_MIN / number) ||
                            (result < 0 && number < 0 && result < INT_MAX / number)) {
                            printf("Integer overflow detected.\n");
                            current_state = ERROR_STATE;
                            break;
                        }
                    }
                    result *= number;
                } else if (strcmp(command_buffer, "div") == 0) {
                    if (number == 0) {
                        printf("Division by zero error.\n");
                        current_state = ERROR_STATE;
                        break;
                    }
                    if (number == -1 && result == INT_MIN) {
                        printf("Integer overflow detected.\n");
                        current_state = ERROR_STATE;
                        break;
                    }
                    result /= number;
                }
                
                current_state = DISPLAY_RESULT;
                break;
                
            case DISPLAY_RESULT:
                printf("Current result: %d\n", result);
                current_state = INIT;
                break;
                
            case ERROR_STATE:
                printf("An error occurred. Resetting...\n");
                result = 0;
                current_state = INIT;
                break;
                
            case EXIT_STATE:
                break;
        }
    }
    
    printf("Final result: %d\n", result);
    printf("Goodbye!\n");
    
    return 0;
}