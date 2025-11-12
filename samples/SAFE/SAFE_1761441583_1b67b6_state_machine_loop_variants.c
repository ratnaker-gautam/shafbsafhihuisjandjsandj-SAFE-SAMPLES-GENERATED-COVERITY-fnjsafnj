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
    int data_value;
    int validation_count;
};

int read_input(struct context *ctx) {
    if (ctx == NULL) return -1;
    
    printf("Enter a number (0-1000): ");
    if (fgets(ctx->input_buffer, sizeof(ctx->input_buffer), stdin) == NULL) {
        return -1;
    }
    
    char *endptr;
    long val = strtol(ctx->input_buffer, &endptr, 10);
    if (endptr == ctx->input_buffer || *endptr != '\n') {
        return -1;
    }
    if (val < 0 || val > 1000) {
        return -1;
    }
    
    ctx->data_value = (int)val;
    return 0;
}

int process_data(struct context *ctx) {
    if (ctx == NULL) return -1;
    
    if (ctx->data_value < 0) return -1;
    
    ctx->data_value *= 2;
    if (ctx->data_value < 0) return -1;
    
    return 0;
}

int validate_data(struct context *ctx) {
    if (ctx == NULL) return -1;
    
    if (ctx->data_value < 0 || ctx->data_value > 2000) {
        return -1;
    }
    
    ctx->validation_count++;
    if (ctx->validation_count > 10) {
        return -1;
    }
    
    return 0;
}

void output_result(struct context *ctx) {
    if (ctx == NULL) return;
    
    printf("Processed result: %d\n", ctx->data_value);
}

int main(void) {
    struct context ctx;
    ctx.current_state = INIT;
    ctx.data_value = 0;
    ctx.validation_count = 0;
    
    while (ctx.current_state != EXIT) {
        switch (ctx.current_state) {
            case INIT:
                ctx.current_state = READ_INPUT;
                break;
                
            case READ_INPUT:
                if (read_input(&ctx) == 0) {
                    ctx.current_state = PROCESS_DATA;
                } else {
                    ctx.current_state = ERROR;
                }
                break;
                
            case PROCESS_DATA:
                if (process_data(&ctx) == 0) {
                    ctx.current_state = VALIDATE;
                } else {
                    ctx.current_state = ERROR;
                }
                break;
                
            case VALIDATE:
                if (validate_data(&ctx) == 0) {
                    ctx.current_state = OUTPUT_RESULT;
                } else {
                    ctx.current_state = ERROR;
                }
                break;
                
            case OUTPUT_RESULT:
                output_result(&ctx);
                ctx.current_state = READ_INPUT;
                break;
                
            case ERROR:
                printf("Error occurred. Resetting state machine.\n");
                ctx.current_state = INIT;
                ctx.data_value = 0;
                ctx.validation_count = 0;
                break;
                
            case EXIT:
                break;
                
            default:
                ctx.current_state = ERROR;
                break;
        }
        
        int loop_counter = 0;
        do {
            if (ctx.current_state == EXIT) {
                break;
            }
            loop_counter++;
            if (loop_counter > 1000) {
                ctx.current_state = ERROR;
                break;
            }
        } while (0);
        
        for (int i = 0; i < 1; i++) {
            if (ctx.current_state == READ_INPUT) {
                int ch;
                while ((ch = getchar()) != '\n' && ch != EOF) {
                    continue;
                }
            }
        }
    }
    
    printf("State machine terminated.\n");
    return 0;
}