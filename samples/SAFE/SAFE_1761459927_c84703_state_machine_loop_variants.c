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
    int data_count;
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

static enum state handle_idle(struct context* ctx) {
    printf("Enter data (or 'quit' to exit): ");
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
        printf("Invalid input\n");
        return IDLE;
    }
    
    return READING;
}

static enum state handle_reading(struct context* ctx) {
    if (ctx->data_count >= ctx->max_items) {
        printf("Maximum items reached\n");
        return DONE;
    }
    
    ctx->data_count++;
    return PROCESSING;
}

static enum state handle_processing(struct context* ctx) {
    for (size_t i = 0; i < ctx->buffer_len; i++) {
        if (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') {
            ctx->buffer[i] = ctx->buffer[i] - 'a' + 'A';
        }
    }
    return WRITING;
}

static enum state handle_writing(struct context* ctx) {
    printf("Processed: %s\n", ctx->buffer);
    return IDLE;
}

static enum state handle_error(struct context* ctx) {
    printf("An error occurred\n");
    return DONE;
}

int main(void) {
    struct context ctx;
    ctx.current_state = IDLE;
    ctx.buffer_len = 0;
    ctx.data_count = 0;
    ctx.max_items = 10;
    
    while (1) {
        enum state next_state;
        
        switch (ctx.current_state) {
            case IDLE:
                next_state = handle_idle(&ctx);
                break;
            case READING:
                next_state = handle_reading(&ctx);
                break;
            case PROCESSING:
                next_state = handle_processing(&ctx);
                break;
            case WRITING:
                next_state = handle_writing(&ctx);
                break;
            case ERROR:
                next_state = handle_error(&ctx);
                break;
            case DONE:
                goto done;
            default:
                next_state = ERROR;
                break;
        }
        
        ctx.current_state = next_state;
    }
    
done:
    printf("Processed %d items\n", ctx.data_count);
    return 0;
}