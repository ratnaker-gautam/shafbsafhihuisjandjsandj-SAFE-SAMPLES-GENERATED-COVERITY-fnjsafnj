//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

enum state {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_WRITING,
    STATE_ERROR,
    STATE_DONE
};

struct context {
    enum state current_state;
    char buffer[256];
    size_t buffer_len;
    int processed_count;
    int max_operations;
};

void initialize_context(struct context *ctx, int max_ops) {
    if (ctx == NULL || max_ops <= 0) {
        return;
    }
    ctx->current_state = STATE_IDLE;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    ctx->buffer_len = 0;
    ctx->processed_count = 0;
    ctx->max_operations = max_ops;
}

int validate_input(const char *input, size_t len) {
    if (input == NULL || len == 0 || len >= 256) {
        return 0;
    }
    for (size_t i = 0; i < len; i++) {
        if (!isalnum((unsigned char)input[i]) && input[i] != ' ') {
            return 0;
        }
    }
    return 1;
}

enum state handle_idle(struct context *ctx) {
    if (ctx == NULL) {
        return STATE_ERROR;
    }
    printf("Enter text to process (alphanumeric and spaces only): ");
    if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) == NULL) {
        return STATE_ERROR;
    }
    ctx->buffer_len = strlen(ctx->buffer);
    if (ctx->buffer_len > 0 && ctx->buffer[ctx->buffer_len - 1] == '\n') {
        ctx->buffer[ctx->buffer_len - 1] = '\0';
        ctx->buffer_len--;
    }
    if (!validate_input(ctx->buffer, ctx->buffer_len)) {
        return STATE_ERROR;
    }
    return STATE_READING;
}

enum state handle_reading(struct context *ctx) {
    if (ctx == NULL || ctx->buffer_len == 0) {
        return STATE_ERROR;
    }
    printf("Read %zu characters: '%s'\n", ctx->buffer_len, ctx->buffer);
    return STATE_PROCESSING;
}

enum state handle_processing(struct context *ctx) {
    if (ctx == NULL || ctx->buffer_len == 0) {
        return STATE_ERROR;
    }
    for (size_t i = 0; i < ctx->buffer_len; i++) {
        if (isalpha((unsigned char)ctx->buffer[i])) {
            ctx->buffer[i] = toupper((unsigned char)ctx->buffer[i]);
        }
    }
    ctx->processed_count++;
    return STATE_WRITING;
}

enum state handle_writing(struct context *ctx) {
    if (ctx == NULL || ctx->buffer_len == 0) {
        return STATE_ERROR;
    }
    printf("Processed result: '%s'\n", ctx->buffer);
    if (ctx->processed_count >= ctx->max_operations) {
        return STATE_DONE;
    }
    return STATE_IDLE;
}

enum state handle_error(struct context *ctx) {
    if (ctx == NULL) {
        return STATE_DONE;
    }
    printf("Error occurred during processing\n");
    return STATE_DONE;
}

enum state handle_done(struct context *ctx) {
    if (ctx == NULL) {
        return STATE_DONE;
    }
    printf("Processing complete. Total operations: %d\n", ctx->processed_count);
    return STATE_DONE;
}

enum state execute_state(struct context *ctx) {
    if (ctx == NULL) {
        return STATE_ERROR;
    }
    switch (ctx->current_state) {
        case STATE_IDLE:
            return handle_idle(ctx);
        case STATE_READING:
            return handle_reading(ctx);
        case STATE_PROCESSING:
            return handle_processing(ctx);
        case STATE_WRITING:
            return handle_writing(ctx);
        case STATE_ERROR:
            return handle_error(ctx);
        case STATE_DONE:
            return handle_done(ctx);
        default:
            return STATE_ERROR;
    }
}

int main(void) {
    struct context ctx;
    initialize_context(&ctx, 3);
    
    while (ctx.current_state != STATE_DONE) {
        ctx.current_state = execute_state(&ctx);
    }
    
    return 0;
}