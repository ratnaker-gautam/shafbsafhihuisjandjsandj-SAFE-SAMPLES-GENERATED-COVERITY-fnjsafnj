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
    int data_value;
    int input_count;
    char input_buffer[32];
    int result;
};

int validate_input(const char* input) {
    if (input == NULL) return 0;
    if (strlen(input) == 0) return 0;
    if (strlen(input) > 10) return 0;
    
    for (size_t i = 0; input[i] != '\0'; i++) {
        if (input[i] < '0' || input[i] > '9') {
            return 0;
        }
    }
    return 1;
}

int parse_number(const char* input) {
    long value = 0;
    for (size_t i = 0; input[i] != '\0'; i++) {
        value = value * 10 + (input[i] - '0');
        if (value > 1000000) return -1;
    }
    return (int)value;
}

void state_machine_loop(struct context* ctx) {
    int running = 1;
    
    while (running) {
        switch (ctx->current_state) {
            case INIT:
                ctx->data_value = 0;
                ctx->input_count = 0;
                ctx->result = 0;
                memset(ctx->input_buffer, 0, sizeof(ctx->input_buffer));
                ctx->current_state = READ_INPUT;
                break;
                
            case READ_INPUT:
                printf("Enter a number (1-1000000) or 'quit': ");
                if (fgets(ctx->input_buffer, sizeof(ctx->input_buffer), stdin) == NULL) {
                    ctx->current_state = ERROR;
                    break;
                }
                
                size_t len = strlen(ctx->input_buffer);
                if (len > 0 && ctx->input_buffer[len - 1] == '\n') {
                    ctx->input_buffer[len - 1] = '\0';
                }
                
                if (strcmp(ctx->input_buffer, "quit") == 0) {
                    ctx->current_state = EXIT;
                } else {
                    ctx->current_state = VALIDATE;
                }
                break;
                
            case VALIDATE:
                if (!validate_input(ctx->input_buffer)) {
                    printf("Invalid input. Please enter digits only.\n");
                    ctx->current_state = READ_INPUT;
                } else {
                    ctx->data_value = parse_number(ctx->input_buffer);
                    if (ctx->data_value < 1 || ctx->data_value > 1000000) {
                        printf("Number out of range. Please enter 1-1000000.\n");
                        ctx->current_state = READ_INPUT;
                    } else {
                        ctx->current_state = PROCESS_DATA;
                    }
                }
                break;
                
            case PROCESS_DATA:
                ctx->result = 0;
                for (int i = 1; i <= ctx->data_value; i++) {
                    if (ctx->data_value % i == 0) {
                        ctx->result++;
                    }
                }
                ctx->current_state = OUTPUT_RESULT;
                break;
                
            case OUTPUT_RESULT:
                printf("Number %d has %d divisors.\n", ctx->data_value, ctx->result);
                ctx->input_count++;
                if (ctx->input_count >= 5) {
                    printf("Maximum inputs reached.\n");
                    ctx->current_state = EXIT;
                } else {
                    ctx->current_state = READ_INPUT;
                }
                break;
                
            case ERROR:
                printf("Input error occurred.\n");
                ctx->current_state = EXIT;
                break;
                
            case EXIT:
                printf("Goodbye. Processed %d numbers.\n", ctx->input_count);
                running = 0;
                break;
        }
    }
}

int main(void) {
    struct context ctx;
    ctx.current_state = INIT;
    state_machine_loop(&ctx);
    return 0;
}