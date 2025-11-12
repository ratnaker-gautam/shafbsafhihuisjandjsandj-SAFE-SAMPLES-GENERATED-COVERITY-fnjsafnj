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
    ctx->processed_count++;
}

int main(void) {
    struct context ctx;
    ctx.current_state = IDLE;
    ctx.buffer_len = 0;
    ctx.processed_count = 0;
    ctx.max_iterations = 5;
    
    int iteration = 0;
    
    while (ctx.current_state != DONE && ctx.current_state != ERROR) {
        switch (ctx.current_state) {
            case IDLE:
                printf("Enter text to process (or 'quit' to exit): ");
                fflush(stdout);
                
                if (fgets(ctx.buffer, sizeof(ctx.buffer), stdin) == NULL) {
                    ctx.current_state = ERROR;
                    break;
                }
                
                ctx.buffer_len = strlen(ctx.buffer);
                if (ctx.buffer_len > 0 && ctx.buffer[ctx.buffer_len - 1] == '\n') {
                    ctx.buffer[--ctx.buffer_len] = '\0';
                }
                
                if (ctx.buffer_len == 0) {
                    ctx.current_state = IDLE;
                } else if (strcmp(ctx.buffer, "quit") == 0) {
                    ctx.current_state = DONE;
                } else if (validate_input(ctx.buffer, ctx.buffer_len)) {
                    ctx.current_state = READING;
                } else {
                    printf("Invalid input detected\n");
                    ctx.current_state = ERROR;
                }
                break;
                
            case READING:
                if (ctx.buffer_len > 0) {
                    ctx.current_state = PROCESSING;
                } else {
                    ctx.current_state = IDLE;
                }
                break;
                
            case PROCESSING:
                process_buffer(&ctx);
                ctx.current_state = WRITING;
                break;
                
            case WRITING:
                printf("Processed: %s\n", ctx.buffer);
                printf("Total processed: %d\n", ctx.processed_count);
                
                iteration++;
                if (iteration >= ctx.max_iterations) {
                    ctx.current_state = DONE;
                } else {
                    ctx.current_state = IDLE;
                }
                break;
                
            case ERROR:
                printf("Error state reached\n");
                ctx.current_state = DONE;
                break;
                
            case DONE:
                break;
        }
    }
    
    printf("Final count: %d strings processed\n", ctx.processed_count);
    
    return ctx.current_state == ERROR ? EXIT_FAILURE : EXIT_SUCCESS;
}