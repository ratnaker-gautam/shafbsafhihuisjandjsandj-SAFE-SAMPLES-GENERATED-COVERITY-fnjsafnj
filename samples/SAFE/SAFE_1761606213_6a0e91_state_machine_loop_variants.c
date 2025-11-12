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
        if (input[i] < 32 || input[i] > 126) {
            return 0;
        }
    }
    return 1;
}

void process_buffer(struct context* ctx) {
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
    ctx.processed_count = 0;
    ctx.max_items = 5;
    
    const char* inputs[] = {
        "hello",
        "world",
        "test",
        "data",
        "final"
    };
    size_t num_inputs = sizeof(inputs) / sizeof(inputs[0]);
    size_t input_index = 0;
    
    while (ctx.current_state != DONE && ctx.current_state != ERROR) {
        switch (ctx.current_state) {
            case INIT:
                ctx.processed_count = 0;
                ctx.current_state = READING;
                break;
                
            case READING:
                if (input_index < num_inputs && ctx.processed_count < ctx.max_items) {
                    const char* current_input = inputs[input_index];
                    size_t input_len = strlen(current_input);
                    
                    if (validate_input(current_input, input_len)) {
                        if (input_len < sizeof(ctx.buffer)) {
                            memcpy(ctx.buffer, current_input, input_len);
                            ctx.buffer_len = input_len;
                            ctx.buffer[ctx.buffer_len] = '\0';
                            ctx.current_state = PROCESSING;
                            input_index++;
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
                if (ctx.buffer_len > 0) {
                    printf("Processed: %s\n", ctx.buffer);
                    ctx.processed_count++;
                    if (ctx.processed_count >= ctx.max_items) {
                        ctx.current_state = DONE;
                    } else {
                        ctx.current_state = READING;
                    }
                } else {
                    ctx.current_state = ERROR;
                }
                break;
                
            case DONE:
                break;
                
            case ERROR:
                printf("Error occurred during processing\n");
                return 1;
        }
    }
    
    printf("Successfully processed %d items\n", ctx.processed_count);
    return 0;
}