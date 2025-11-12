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
    char current_command[16] = {0};
    int number = 0;
    int result = 0;
    int valid_input = 0;
    
    printf("Number Processor State Machine\n");
    printf("Commands: add, sub, mul, div, exit\n");
    
    while (current_state != EXIT_STATE) {
        switch (current_state) {
            case INIT:
                printf("Enter command: ");
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                    current_state = ERROR_STATE;
                    break;
                }
                current_state = READ_COMMAND;
                break;
                
            case READ_COMMAND:
                input_buffer[strcspn(input_buffer, "\n")] = '\0';
                
                if (strcmp(input_buffer, "exit") == 0) {
                    current_state = EXIT_STATE;
                } else if (strcmp(input_buffer, "add") == 0 || 
                           strcmp(input_buffer, "sub") == 0 ||
                           strcmp(input_buffer, "mul") == 0 ||
                           strcmp(input_buffer, "div") == 0) {
                    strncpy(current_command, input_buffer, sizeof(current_command)-1);
                    current_command[sizeof(current_command)-1] = '\0';
                    printf("Enter number: ");
                    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                        current_state = ERROR_STATE;
                        break;
                    }
                    current_state = PROCESS_NUMBER;
                } else {
                    printf("Invalid command. Try again.\n");
                    current_state = INIT;
                }
                break;
                
            case PROCESS_NUMBER:
                valid_input = 0;
                if (sscanf(input_buffer, "%d", &number) == 1) {
                    valid_input = 1;
                }
                
                if (!valid_input) {
                    printf("Invalid number. Try again.\n");
                    current_state = INIT;
                    break;
                }
                
                if (strcmp(current_command, "add") == 0) {
                    if ((number > 0 && result > INT_MAX - number) || 
                        (number < 0 && result < INT_MIN - number)) {
                        printf("Integer overflow detected.\n");
                        current_state = ERROR_STATE;
                        break;
                    }
                    result += number;
                } else if (strcmp(current_command, "sub") == 0) {
                    if ((number < 0 && result > INT_MAX + number) || 
                        (number > 0 && result < INT_MIN + number)) {
                        printf("Integer overflow detected.\n");
                        current_state = ERROR_STATE;
                        break;
                    }
                    result -= number;
                } else if (strcmp(current_command, "mul") == 0) {
                    if (number > 0) {
                        if (result > INT_MAX / number || result < INT_MIN / number) {
                            printf("Integer overflow detected.\n");
                            current_state = ERROR_STATE;
                            break;
                        }
                    } else if (number < 0) {
                        if (number == -1) {
                            if (result < -INT_MAX) {
                                printf("Integer overflow detected.\n");
                                current_state = ERROR_STATE;
                                break;
                            }
                        } else if (result > INT_MIN / number || result < INT_MAX / number) {
                            printf("Integer overflow detected.\n");
                            current_state = ERROR_STATE;
                            break;
                        }
                    }
                    result *= number;
                } else if (strcmp(current_command, "div") == 0) {
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
                printf("Error occurred. Resetting state machine.\n");
                result = 0;
                current_state = INIT;
                break;
                
            case EXIT_STATE:
                break;
        }
    }
    
    printf("Goodbye!\n");
    return 0;
}