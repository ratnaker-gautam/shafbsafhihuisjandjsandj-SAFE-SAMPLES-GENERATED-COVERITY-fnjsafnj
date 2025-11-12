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
    ctx->processed_count++;
}

enum state handle_idle(struct context* ctx) {
    printf("Enter text (or 'quit' to exit): ");
    
    if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) == NULL) {
        return ERROR;
    }
    
    ctx->buffer_len = strlen(ctx->buffer);
    if (ctx->buffer_len > 0 && ctx->buffer[ctx->buffer_len - 1] == '\n') {
        ctx->buffer[--ctx->buffer_len] = '\0';
    }
    
    if (ctx->buffer_len == 0) {
        return IDLE;
    }
    
    if (strcmp(ctx->buffer, "quit") == 0) {
        return DONE;
    }
    
    if (!validate_input(ctx->buffer, ctx->buffer_len)) {
        return ERROR;
    }
    
    return READING;
}

enum state handle_reading(struct context* ctx) {
    if (ctx->processed_count >= ctx->max_items) {
        return DONE;
    }
    return PROCESSING;
}

enum state handle_processing(struct context* ctx) {
    process_buffer(ctx);
    return WRITING;
}

enum state handle_writing(struct context* ctx) {
    printf("Processed: %s\n", ctx->buffer);
    printf("Total processed: %d/%d\n", ctx->processed_count, ctx->max_items);
    return IDLE;
}

enum state handle_error(struct context* ctx) {
    printf("Error: Invalid input detected\n");
    ctx->buffer[0] = '\0';
    ctx->buffer_len = 0;
    return IDLE;
}

int main(void) {
    struct context ctx;
    ctx.current_state = IDLE;
    ctx.buffer[0] = '\0';
    ctx.buffer_len = 0;
    ctx.processed_count = 0;
    ctx.max_items = 5;
    
    while (ctx.current_state != DONE) {
        switch (ctx.current_state) {
            case IDLE:
                ctx.current_state = handle_idle(&ctx);
                break;
            case READING:
                ctx.current_state = handle_reading(&ctx);
                break;
            case PROCESSING:
                ctx.current_state = handle_processing(&ctx);
                break;
            case WRITING:
                ctx.current_state = handle_writing(&ctx);
                break;
            case ERROR:
                ctx.current_state = handle_error(&ctx);
                break;
            case DONE:
                break;
        }
    }
    
    printf("Processing complete. Goodbye.\n");
    return 0;
}