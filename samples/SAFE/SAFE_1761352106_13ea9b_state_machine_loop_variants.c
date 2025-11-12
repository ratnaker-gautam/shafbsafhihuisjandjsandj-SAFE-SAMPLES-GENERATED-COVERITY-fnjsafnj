//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

enum State {
    INIT,
    READ_INPUT,
    PROCESS_DATA,
    VALIDATE,
    OUTPUT_RESULT,
    ERROR,
    EXIT
};

struct Context {
    enum State current_state;
    char input_buffer[256];
    int data_value;
    int validation_count;
};

int validate_input(const char* input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > 255) return 0;
    
    for (size_t i = 0; i < len; i++) {
        if (input[i] < '0' || input[i] > '9') {
            return 0;
        }
    }
    return 1;
}

int process_data(const char* input) {
    if (input == NULL) return -1;
    int sum = 0;
    size_t len = strlen(input);
    
    for (size_t i = 0; i < len; i++) {
        int digit = input[i] - '0';
        if (digit < 0 || digit > 9) {
            return -1;
        }
        if (sum > INT_MAX - digit) {
            return -1;
        }
        sum += digit;
    }
    return sum;
}

int main(void) {
    struct Context ctx = {INIT, {0}, 0, 0};
    int running = 1;
    
    while (running) {
        switch (ctx.current_state) {
            case INIT:
                printf("State Machine Started\n");
                ctx.current_state = READ_INPUT;
                break;
                
            case READ_INPUT:
                printf("Enter a number (or 'quit' to exit): ");
                if (fgets(ctx.input_buffer, sizeof(ctx.input_buffer), stdin) == NULL) {
                    ctx.current_state = ERROR;
                    break;
                }
                
                size_t len = strlen(ctx.input_buffer);
                if (len > 0 && ctx.input_buffer[len - 1] == '\n') {
                    ctx.input_buffer[len - 1] = '\0';
                }
                
                if (strcmp(ctx.input_buffer, "quit") == 0) {
                    ctx.current_state = EXIT;
                } else {
                    ctx.current_state = VALIDATE;
                }
                break;
                
            case VALIDATE:
                if (validate_input(ctx.input_buffer)) {
                    ctx.current_state = PROCESS_DATA;
                } else {
                    printf("Invalid input. Please enter digits only.\n");
                    ctx.current_state = READ_INPUT;
                }
                break;
                
            case PROCESS_DATA:
                ctx.data_value = process_data(ctx.input_buffer);
                if (ctx.data_value == -1) {
                    printf("Error: Integer overflow detected.\n");
                    ctx.current_state = ERROR;
                } else {
                    ctx.current_state = OUTPUT_RESULT;
                }
                break;
                
            case OUTPUT_RESULT:
                printf("Sum of digits: %d\n", ctx.data_value);
                ctx.validation_count++;
                if (ctx.validation_count >= 3) {
                    printf("Maximum operations reached.\n");
                    ctx.current_state = EXIT;
                } else {
                    ctx.current_state = READ_INPUT;
                }
                break;
                
            case ERROR:
                printf("An error occurred. Resetting...\n");
                ctx.current_state = READ_INPUT;
                ctx.validation_count = 0;
                memset(ctx.input_buffer, 0, sizeof(ctx.input_buffer));
                break;
                
            case EXIT:
                printf("Exiting state machine.\n");
                running = 0;
                break;
        }
    }
    
    return 0;
}