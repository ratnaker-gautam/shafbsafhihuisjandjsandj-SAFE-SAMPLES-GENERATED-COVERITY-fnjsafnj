//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

enum state {
    IDLE,
    READING,
    PROCESSING,
    WRITING,
    ERROR,
    DONE
};

struct context {
    enum state current_state;
    char buffer[256];
    size_t buffer_len;
    int data_value;
    int processed_count;
};

int validate_input(const char* input, size_t len) {
    if (input == NULL || len == 0 || len >= 256) {
        return 0;
    }
    for (size_t i = 0; i < len; i++) {
        if (input[i] < '0' || input[i] > '9') {
            return 0;
        }
    }
    return 1;
}

int parse_number(const char* str, size_t len) {
    int result = 0;
    for (size_t i = 0; i < len; i++) {
        if (result > INT_MAX / 10) {
            return -1;
        }
        result = result * 10;
        int digit = str[i] - '0';
        if (result > INT_MAX - digit) {
            return -1;
        }
        result += digit;
    }
    return result;
}

void state_machine_loop(struct context* ctx) {
    while (ctx->current_state != DONE && ctx->current_state != ERROR) {
        switch (ctx->current_state) {
            case IDLE:
                printf("Enter a number (or 'quit' to exit): ");
                if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) == NULL) {
                    ctx->current_state = ERROR;
                    break;
                }
                ctx->buffer_len = strlen(ctx->buffer);
                if (ctx->buffer_len > 0 && ctx->buffer[ctx->buffer_len - 1] == '\n') {
                    ctx->buffer[--ctx->buffer_len] = '\0';
                }
                if (strcmp(ctx->buffer, "quit") == 0) {
                    ctx->current_state = DONE;
                } else if (validate_input(ctx->buffer, ctx->buffer_len)) {
                    ctx->current_state = READING;
                } else {
                    printf("Invalid input. Please enter digits only.\n");
                    ctx->current_state = IDLE;
                }
                break;
                
            case READING:
                ctx->data_value = parse_number(ctx->buffer, ctx->buffer_len);
                if (ctx->data_value < 0) {
                    printf("Number too large.\n");
                    ctx->current_state = ERROR;
                } else {
                    ctx->current_state = PROCESSING;
                }
                break;
                
            case PROCESSING:
                if (ctx->data_value > 1000) {
                    ctx->data_value = ctx->data_value / 10;
                } else {
                    ctx->data_value = ctx->data_value * 2;
                }
                ctx->processed_count++;
                ctx->current_state = WRITING;
                break;
                
            case WRITING:
                printf("Processed value: %d (total processed: %d)\n", 
                       ctx->data_value, ctx->processed_count);
                ctx->current_state = IDLE;
                break;
                
            default:
                ctx->current_state = ERROR;
                break;
        }
    }
}

int main(void) {
    struct context ctx;
    ctx.current_state = IDLE;
    ctx.buffer_len = 0;
    ctx.data_value = 0;
    ctx.processed_count = 0;
    
    state_machine_loop(&ctx);
    
    if (ctx.current_state == ERROR) {
        printf("An error occurred during processing.\n");
        return 1;
    }
    
    printf("Processing completed. Total items: %d\n", ctx.processed_count);
    return 0;
}