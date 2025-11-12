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
    int running = 1;
    
    while (running) {
        switch (ctx.current_state) {
            case INIT:
                printf("Enter up to %d items (empty line to finish):\n", ctx.max_items);
                ctx.current_state = READING;
                break;
                
            case READING:
                if (fgets(input, sizeof(input), stdin) != NULL) {
                    size_t len = strlen(input);
                    if (len > 0 && input[len - 1] == '\n') {
                        input[len - 1] = '\0';
                        len--;
                    }
                    
                    if (len == 0) {
                        ctx.current_state = DONE;
                    } else if (validate_input(input, len)) {
                        if (len < sizeof(ctx.buffer)) {
                            strncpy(ctx.buffer, input, sizeof(ctx.buffer) - 1);
                            ctx.buffer[sizeof(ctx.buffer) - 1] = '\0';
                            ctx.buffer_len = len;
                            ctx.current_state = PROCESSING;
                        } else {
                            ctx.current_state = ERROR;
                        }
                    } else {
                        ctx.current_state = ERROR;
                    }
                } else {
                    ctx.current_state = DONE;
                }
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
                printf("Invalid input detected\n");
                ctx.current_state = READING;
                break;
                
            case DONE:
                printf("Processed %d items. Exiting.\n", ctx.processed_count);
                running = 0;
                break;
        }
    }
    
    return 0;
}