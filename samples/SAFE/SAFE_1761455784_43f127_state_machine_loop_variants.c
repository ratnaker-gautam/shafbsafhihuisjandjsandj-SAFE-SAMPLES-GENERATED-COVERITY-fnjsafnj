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

struct context {
    enum state current_state;
    char input_buffer[256];
    int processed_value;
    int error_code;
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

int process_numeric_input(const char* input) {
    long value = 0;
    char* endptr;
    
    value = strtol(input, &endptr, 10);
    if (endptr == input || *endptr != '\0') {
        return -1;
    }
    
    if (value < -10000 || value > 10000) {
        return -1;
    }
    
    return (int)value;
}

int main(void) {
    struct context ctx = {INIT, {0}, 0, 0};
    int running = 1;
    
    while (running) {
        switch (ctx.current_state) {
            case INIT:
                printf("Numeric Processor - Enter numbers or 'quit' to exit\n");
                ctx.current_state = READ_INPUT;
                break;
                
            case READ_INPUT:
                printf("Enter a number: ");
                if (fgets(ctx.input_buffer, sizeof(ctx.input_buffer), stdin) != NULL) {
                    size_t len = strlen(ctx.input_buffer);
                    if (len > 0 && ctx.input_buffer[len - 1] == '\n') {
                        ctx.input_buffer[len - 1] = '\0';
                    }
                    ctx.current_state = PROCESS_DATA;
                } else {
                    ctx.error_code = 1;
                    ctx.current_state = ERROR;
                }
                break;
                
            case PROCESS_DATA:
                if (strcmp(ctx.input_buffer, "quit") == 0) {
                    ctx.current_state = EXIT;
                } else {
                    ctx.current_state = VALIDATE;
                }
                break;
                
            case VALIDATE:
                if (validate_input(ctx.input_buffer)) {
                    int result = process_numeric_input(ctx.input_buffer);
                    if (result != -1) {
                        ctx.processed_value = result;
                        ctx.current_state = OUTPUT_RESULT;
                    } else {
                        ctx.error_code = 2;
                        ctx.current_state = ERROR;
                    }
                } else {
                    ctx.error_code = 3;
                    ctx.current_state = ERROR;
                }
                break;
                
            case OUTPUT_RESULT:
                printf("Processed value: %d\n", ctx.processed_value);
                printf("Squared value: %ld\n", (long)ctx.processed_value * ctx.processed_value);
                ctx.current_state = READ_INPUT;
                break;
                
            case ERROR:
                switch (ctx.error_code) {
                    case 1:
                        printf("Error: Failed to read input\n");
                        break;
                    case 2:
                        printf("Error: Invalid numeric value\n");
                        break;
                    case 3:
                        printf("Error: Input must contain only digits\n");
                        break;
                    default:
                        printf("Error: Unknown error occurred\n");
                        break;
                }
                ctx.current_state = READ_INPUT;
                break;
                
            case EXIT:
                printf("Goodbye!\n");
                running = 0;
                break;
        }
    }
    
    return 0;
}