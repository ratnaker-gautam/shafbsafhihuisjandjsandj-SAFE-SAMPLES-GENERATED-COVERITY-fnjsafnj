//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdint.h>
#include <string.h>

enum State {
    INIT,
    READING,
    PROCESSING,
    WRITING,
    ERROR,
    DONE
};

struct Context {
    enum State current_state;
    char buffer[256];
    size_t buffer_len;
    int processed_count;
    int max_iterations;
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

void process_buffer(struct Context* ctx) {
    if (ctx->buffer_len > 0) {
        for (size_t i = 0; i < ctx->buffer_len; i++) {
            if (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') {
                ctx->buffer[i] = ctx->buffer[i] - 'a' + 'A';
            }
        }
    }
}

enum State handle_init(struct Context* ctx) {
    ctx->buffer_len = 0;
    ctx->processed_count = 0;
    ctx->max_iterations = 5;
    return READING;
}

enum State handle_reading(struct Context* ctx) {
    printf("Enter text (max 255 chars): ");
    if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) == NULL) {
        return ERROR;
    }
    
    size_t len = strlen(ctx->buffer);
    if (len > 0 && ctx->buffer[len - 1] == '\n') {
        ctx->buffer[len - 1] = '\0';
        len--;
    }
    
    if (!validate_input(ctx->buffer, len)) {
        return ERROR;
    }
    
    ctx->buffer_len = len;
    return PROCESSING;
}

enum State handle_processing(struct Context* ctx) {
    if (ctx->buffer_len == 0) {
        return WRITING;
    }
    
    process_buffer(ctx);
    ctx->processed_count++;
    
    if (ctx->processed_count >= ctx->max_iterations) {
        return DONE;
    }
    
    return WRITING;
}

enum State handle_writing(struct Context* ctx) {
    printf("Processed: %s\n", ctx->buffer);
    printf("Processed count: %d/%d\n", ctx->processed_count, ctx->max_iterations);
    return READING;
}

enum State handle_error(struct Context* ctx) {
    printf("Error: Invalid input detected\n");
    ctx->buffer_len = 0;
    return READING;
}

enum State handle_done(struct Context* ctx) {
    printf("Processing complete. Total iterations: %d\n", ctx->processed_count);
    return DONE;
}

int main(void) {
    struct Context ctx = {INIT, {0}, 0, 0, 0};
    
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
            case WRITING:
                ctx.current_state = handle_writing(&ctx);
                break;
            case ERROR:
                ctx.current_state = handle_error(&ctx);
                break;
            case DONE:
                return 0;
            default:
                ctx.current_state = ERROR;
                break;
        }
    }
    
    return 0;
}