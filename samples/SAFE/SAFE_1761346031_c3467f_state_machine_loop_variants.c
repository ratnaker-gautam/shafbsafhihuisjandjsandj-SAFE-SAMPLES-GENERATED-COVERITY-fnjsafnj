//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

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
    int processed_count;
    int max_iterations;
};

static int validate_input(const char* input, size_t len) {
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

static void process_buffer(struct context* ctx) {
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
    ctx.current_state = IDLE;
    ctx.buffer_len = 0;
    ctx.processed_count = 0;
    ctx.max_iterations = 5;
    
    const char* inputs[] = {
        "hello world",
        "test input",
        "another line",
        "final data",
        "last one"
    };
    size_t input_count = sizeof(inputs) / sizeof(inputs[0]);
    size_t input_index = 0;
    
    while (ctx.current_state != DONE && ctx.current_state != ERROR) {
        switch (ctx.current_state) {
            case IDLE:
                if (input_index < input_count && ctx.processed_count < ctx.max_iterations) {
                    const char* current_input = inputs[input_index];
                    size_t input_len = strlen(current_input);
                    if (validate_input(current_input, input_len)) {
                        if (input_len < sizeof(ctx.buffer)) {
                            memcpy(ctx.buffer, current_input, input_len);
                            ctx.buffer_len = input_len;
                            ctx.buffer[ctx.buffer_len] = '\0';
                            ctx.current_state = READING;
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
                
            case READING:
                if (ctx.buffer_len > 0) {
                    ctx.current_state = PROCESSING;
                } else {
                    ctx.current_state = ERROR;
                }
                break;
                
            case PROCESSING:
                process_buffer(&ctx);
                ctx.processed_count++;
                ctx.current_state = WRITING;
                break;
                
            case WRITING:
                if (ctx.buffer_len > 0) {
                    printf("Processed: %s\n", ctx.buffer);
                    input_index++;
                    ctx.current_state = IDLE;
                } else {
                    ctx.current_state = ERROR;
                }
                break;
                
            case ERROR:
                printf("Error occurred during processing\n");
                ctx.current_state = DONE;
                break;
                
            case DONE:
                break;
        }
    }
    
    printf("Completed processing %d items\n", ctx.processed_count);
    return ctx.current_state == ERROR ? 1 : 0;
}