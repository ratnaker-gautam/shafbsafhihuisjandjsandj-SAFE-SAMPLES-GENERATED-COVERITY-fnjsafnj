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
    int max_items;
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
    if (ctx->buffer_len > 0) {
        for (size_t i = 0; i < ctx->buffer_len; i++) {
            if (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') {
                ctx->buffer[i] = ctx->buffer[i] - 'a' + 'A';
            }
        }
        ctx->processed_count++;
    }
}

int main(void) {
    struct context ctx = {0};
    ctx.current_state = IDLE;
    ctx.buffer_len = 0;
    ctx.processed_count = 0;
    ctx.max_items = 5;
    
    const char* inputs[] = {
        "hello",
        "world",
        "test",
        "data",
        "end"
    };
    size_t input_count = sizeof(inputs) / sizeof(inputs[0]);
    size_t input_index = 0;
    
    while (ctx.current_state != DONE && ctx.current_state != ERROR) {
        switch (ctx.current_state) {
            case IDLE:
                if (input_index < input_count) {
                    const char* input = inputs[input_index];
                    size_t input_len = strlen(input);
                    if (validate_input(input, input_len)) {
                        if (input_len < sizeof(ctx.buffer)) {
                            memcpy(ctx.buffer, input, input_len);
                            ctx.buffer_len = input_len;
                            ctx.current_state = READING;
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
                
            case READING:
                if (ctx.buffer_len > 0) {
                    ctx.current_state = PROCESSING;
                } else {
                    ctx.current_state = ERROR;
                }
                break;
                
            case PROCESSING:
                process_buffer(&ctx);
                ctx.current_state = WRITING;
                break;
                
            case WRITING:
                if (ctx.buffer_len > 0) {
                    if (fwrite(ctx.buffer, 1, ctx.buffer_len, stdout) == ctx.buffer_len) {
                        fputc('\n', stdout);
                        ctx.buffer_len = 0;
                        if (ctx.processed_count >= ctx.max_items) {
                            ctx.current_state = DONE;
                        } else {
                            ctx.current_state = IDLE;
                        }
                    } else {
                        ctx.current_state = ERROR;
                    }
                } else {
                    ctx.current_state = ERROR;
                }
                break;
                
            case ERROR:
                fputs("Error occurred\n", stderr);
                return EXIT_FAILURE;
                
            case DONE:
                break;
        }
    }
    
    printf("Processed %d items\n", ctx.processed_count);
    return EXIT_SUCCESS;
}