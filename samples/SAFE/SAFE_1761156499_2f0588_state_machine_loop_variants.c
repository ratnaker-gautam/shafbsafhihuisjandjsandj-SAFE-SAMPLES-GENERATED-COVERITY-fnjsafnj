//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

enum state {
    INIT,
    READING,
    PROCESSING,
    OUTPUT,
    ERROR,
    DONE
};

struct context {
    enum state current_state;
    char buffer[256];
    size_t buffer_len;
    int data_count;
    int max_items;
};

int validate_input(const char* input, size_t len) {
    if (input == NULL || len == 0 || len >= 256) {
        return 0;
    }
    for (size_t i = 0; i < len; i++) {
        if (input[i] < 32 || input[i] > 126) {
            return 0;
        }
    }
    return 1;
}

void process_data(struct context* ctx) {
    if (ctx == NULL || ctx->buffer_len == 0) {
        return;
    }
    
    for (size_t i = 0; i < ctx->buffer_len; i++) {
        if (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') {
            ctx->buffer[i] = ctx->buffer[i] - 'a' + 'A';
        }
    }
}

int main(void) {
    struct context ctx;
    ctx.current_state = INIT;
    ctx.buffer_len = 0;
    ctx.data_count = 0;
    ctx.max_items = 5;
    
    while (ctx.current_state != DONE) {
        switch (ctx.current_state) {
            case INIT:
                printf("State machine initialized. Enter up to %d data items.\n", ctx.max_items);
                ctx.current_state = READING;
                break;
                
            case READING:
                printf("Enter data (or 'quit' to exit): ");
                if (fgets(ctx.buffer, sizeof(ctx.buffer), stdin) == NULL) {
                    ctx.current_state = ERROR;
                    break;
                }
                
                ctx.buffer_len = strlen(ctx.buffer);
                if (ctx.buffer_len > 0 && ctx.buffer[ctx.buffer_len - 1] == '\n') {
                    ctx.buffer[ctx.buffer_len - 1] = '\0';
                    ctx.buffer_len--;
                }
                
                if (ctx.buffer_len == 0) {
                    printf("Empty input, try again.\n");
                    break;
                }
                
                if (!validate_input(ctx.buffer, ctx.buffer_len)) {
                    ctx.current_state = ERROR;
                    break;
                }
                
                if (strcmp(ctx.buffer, "quit") == 0) {
                    ctx.current_state = DONE;
                    break;
                }
                
                ctx.current_state = PROCESSING;
                break;
                
            case PROCESSING:
                process_data(&ctx);
                ctx.current_state = OUTPUT;
                break;
                
            case OUTPUT:
                printf("Processed data: %s\n", ctx.buffer);
                ctx.data_count++;
                
                if (ctx.data_count >= ctx.max_items) {
                    printf("Maximum items reached.\n");
                    ctx.current_state = DONE;
                } else {
                    ctx.current_state = READING;
                }
                break;
                
            case ERROR:
                printf("Error: Invalid input detected.\n");
                ctx.current_state = READING;
                break;
                
            case DONE:
                break;
        }
    }
    
    printf("Processing complete. Processed %d items.\n", ctx.data_count);
    return 0;
}