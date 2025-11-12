//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

enum state {
    INIT,
    READ_INPUT,
    PROCESS_DATA,
    VALIDATE,
    OUTPUT_RESULT,
    ERROR,
    EXIT
};

int main(void) {
    enum state current_state = INIT;
    char input_buffer[256];
    int data_value = 0;
    int valid_input = 0;
    
    while (current_state != EXIT) {
        switch (current_state) {
            case INIT:
                printf("State Machine Calculator - Enter numbers (0-100) or 'quit'\n");
                current_state = READ_INPUT;
                break;
                
            case READ_INPUT:
                printf("Enter number: ");
                if (fgets(input_buffer, sizeof(input_buffer), stdin) != NULL) {
                    size_t len = strlen(input_buffer);
                    if (len > 0 && input_buffer[len - 1] == '\n') {
                        input_buffer[len - 1] = '\0';
                    }
                    
                    if (strcmp(input_buffer, "quit") == 0) {
                        current_state = EXIT;
                    } else {
                        current_state = PROCESS_DATA;
                    }
                } else {
                    current_state = ERROR;
                }
                break;
                
            case PROCESS_DATA:
                {
                    char *endptr;
                    long temp = strtol(input_buffer, &endptr, 10);
                    
                    if (endptr != input_buffer && *endptr == '\0' && 
                        temp >= 0 && temp <= 100) {
                        data_value = (int)temp;
                        valid_input = 1;
                        current_state = VALIDATE;
                    } else {
                        valid_input = 0;
                        current_state = ERROR;
                    }
                }
                break;
                
            case VALIDATE:
                if (valid_input && data_value >= 0 && data_value <= 100) {
                    current_state = OUTPUT_RESULT;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case OUTPUT_RESULT:
                {
                    int result = data_value * data_value;
                    if (data_value <= 50 && result / data_value != data_value) {
                        current_state = ERROR;
                        break;
                    }
                    printf("Square of %d is %d\n", data_value, result);
                    current_state = READ_INPUT;
                }
                break;
                
            case ERROR:
                printf("Error: Invalid input. Please enter a number 0-100 or 'quit'\n");
                current_state = READ_INPUT;
                break;
                
            case EXIT:
                printf("Goodbye!\n");
                break;
                
            default:
                current_state = ERROR;
                break;
        }
    }
    
    return 0;
}