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

static int validate_input(const char* input, size_t len) {
    if (input == NULL || len == 0 || len >= 256) {
        return 0;
    }
    for (size_t i = 0; i < len; i++) {
        if (input[i] == '\0') {
            return 0;
        }
    }
    return 1;
}

static enum state handle_init(struct context* ctx) {
    ctx->buffer_len = 0;
    ctx->processed_count = 0;
    ctx->max_items = 5;
    printf("State machine initialized. Enter up to %d items.\n", ctx->max_items);
    return READING;
}

static enum state handle_reading(struct context* ctx) {
    printf("Enter item (or 'quit' to finish): ");
    
    char input[256];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return ERROR;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (!validate_input(input, len)) {
        return ERROR;
    }
    
    if (strcmp(input, "quit") == 0) {
        return DONE;
    }
    
    if (len >= sizeof(ctx->buffer)) {
        return ERROR;
    }
    
    memcpy(ctx->buffer, input, len + 1);
    ctx->buffer_len = len;
    return PROCESSING;
}

static enum state handle_processing(struct context* ctx) {
    for (size_t i = 0; i < ctx->buffer_len; i++) {
        if (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') {
            ctx->buffer[i] = ctx->buffer[i] - 'a' + 'A';
        }
    }
    ctx->processed_count++;
    return OUTPUT;
}

static enum state handle_output(struct context* ctx) {
    printf("Processed item %d: %s\n", ctx->processed_count, ctx->buffer);
    
    if (ctx->processed_count >= ctx->max_items) {
        return DONE;
    }
    
    return READING;
}

static enum state handle_error(struct context* ctx) {
    printf("Error occurred. Resetting state machine.\n");
    return INIT;
}

static enum state handle_done(struct context* ctx) {
    printf("Processing complete. Processed %d items.\n", ctx->processed_count);
    return DONE;
}

int main(void) {
    struct context ctx;
    ctx.current_state = INIT;
    
    while (1) {
        switch (ctx.current_state) {
            case INIT:
                ctx.current_state = handle_init(&ctx);
                break;
            case READING:
                ctx.current_state = handle_reading(&ctx);
                break;
            case PROCESSING:
                ctx.current_state = handle_processing(&ctx);
                break;
            case OUTPUT:
                ctx.current_state = handle_output(&ctx);
                break;
            case ERROR:
                ctx.current_state = handle_error(&ctx);
                break;
            case DONE:
                handle_done(&ctx);
                return 0;
        }
    }
    
    return 0;
}