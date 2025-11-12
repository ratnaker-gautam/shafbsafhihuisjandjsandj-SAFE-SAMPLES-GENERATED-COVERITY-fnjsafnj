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
    DONE,
    ERROR
};

struct context {
    enum state current_state;
    char buffer[256];
    size_t buffer_len;
    int processed_count;
    int max_items;
};

int validate_input(const char* input, size_t len) {
    if (input == NULL || len == 0 || len >= 256) {
        return 0;
    }
    for (size_t i = 0; i < len; i++) {
        if (input[i] == 0) {
            return 0;
        }
        if (input[i] < 32 || input[i] > 126) {
            return 0;
        }
    }
    return 1;
}

void process_buffer(struct context* ctx) {
    if (ctx->buffer_len > 0) {
        for (size_t i = 0; i < ctx->buffer_len; i++) {
            if (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') {
                ctx->buffer[i] = ctx->buffer[i] - 'a' + 'A';
            }
        }
    }
}

int main(void) {
    struct context ctx;
    ctx.current_state = INIT;
    ctx.buffer_len = 0;
    ctx.processed_count = 0;
    ctx.max_items = 5;
    
    char input[512];
    
    while (ctx.current_state != DONE && ctx.current_state != ERROR) {
        switch (ctx.current_state) {
            case INIT:
                printf("Starting processing (max %d items)\n", ctx.max_items);
                ctx.current_state = READING;
                break;
                
            case READING:
                printf("Enter text (or 'quit' to finish): ");
                if (fgets(input, sizeof(input), stdin) == NULL) {
                    ctx.current_state = ERROR;
                    break;
                }
                
                size_t len = strlen(input);
                if (len > 0 && input[len - 1] == '\n') {
                    input[len - 1] = '\0';
                    len--;
                }
                
                if (strcmp(input, "quit") == 0) {
                    ctx.current_state = DONE;
                    break;
                }
                
                if (!validate_input(input, len)) {
                    printf("Invalid input\n");
                    break;
                }
                
                if (len >= sizeof(ctx.buffer)) {
                    printf("Input too long\n");
                    break;
                }
                
                strncpy(ctx.buffer, input, sizeof(ctx.buffer) - 1);
                ctx.buffer[sizeof(ctx.buffer) - 1] = '\0';
                ctx.buffer_len = len;
                ctx.current_state = PROCESSING;
                break;
                
            case PROCESSING:
                process_buffer(&ctx);
                ctx.current_state = OUTPUT;
                break;
                
            case OUTPUT:
                printf("Processed: %s\n", ctx.buffer);
                ctx.processed_count++;
                
                if (ctx.processed_count >= ctx.max_items) {
                    ctx.current_state = DONE;
                } else {
                    ctx.current_state = READING;
                }
                break;
                
            case ERROR:
                printf("Error occurred\n");
                break;
                
            case DONE:
                break;
        }
    }
    
    printf("Processed %d items\n", ctx.processed_count);
    return 0;
}