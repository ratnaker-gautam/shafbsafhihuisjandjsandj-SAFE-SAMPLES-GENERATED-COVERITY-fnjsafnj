//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

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

int process_data(char* data, size_t len) {
    if (data == NULL || len == 0 || len >= 256) {
        return 0;
    }
    for (size_t i = 0; i < len; i++) {
        if (data[i] >= 'a' && data[i] <= 'z') {
            data[i] = data[i] - 'a' + 'A';
        }
    }
    return 1;
}

enum state handle_idle(struct context* ctx) {
    printf("Enter data (or 'quit' to exit): ");
    if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) == NULL) {
        return STATE_ERROR;
    }
    
    ctx->buffer_len = strlen(ctx->buffer);
    if (ctx->buffer_len > 0 && ctx->buffer[ctx->buffer_len - 1] == '\n') {
        ctx->buffer[ctx->buffer_len - 1] = '\0';
        ctx->buffer_len--;
    }
    
    if (ctx->buffer_len == 0) {
        return STATE_IDLE;
    }
    
    if (strcmp(ctx->buffer, "quit") == 0) {
        return STATE_DONE;
    }
    
    if (!validate_input(ctx->buffer, ctx->buffer_len)) {
        printf("Invalid input detected\n");
        return STATE_ERROR;
    }
    
    return STATE_READING;
}

enum state handle_reading(struct context* ctx) {
    if (ctx->processed_count >= ctx->max_items) {
        printf("Maximum items processed\n");
        return STATE_DONE;
    }
    return STATE_PROCESSING;
}

enum state handle_processing(struct context* ctx) {
    if (!process_data(ctx->buffer, ctx->buffer_len)) {
        return STATE_ERROR;
    }
    ctx->processed_count++;
    return STATE_WRITING;
}

enum state handle_writing(struct context* ctx) {
    printf("Processed: %s\n", ctx->buffer);
    return STATE_IDLE;
}

enum state handle_error(struct context* ctx) {
    printf("Error occurred, resetting...\n");
    ctx->buffer[0] = '\0';
    ctx->buffer_len = 0;
    return STATE_IDLE;
}

enum state handle_done(struct context* ctx) {
    printf("Processing complete. Total items: %d\n", ctx->processed_count);
    return STATE_DONE;
}

void run_state_machine(struct context* ctx) {
    int max_iterations = 1000;
    int iteration = 0;
    
    while (ctx->current_state != STATE_DONE && iteration < max_iterations) {
        switch (ctx->current_state) {
            case STATE_IDLE:
                ctx->current_state = handle_idle(ctx);
                break;
            case STATE_READING:
                ctx->current_state = handle_reading(ctx);
                break;
            case STATE_PROCESSING:
                ctx->current_state = handle_processing(ctx);
                break;
            case STATE_WRITING:
                ctx->current_state = handle_writing(ctx);
                break;
            case STATE_ERROR:
                ctx->current_state = handle_error(ctx);
                break;
            case STATE_DONE:
                ctx->current_state = handle_done(ctx);
                break;
            default:
                ctx->current_state = STATE_ERROR;
                break;
        }
        iteration++;
    }
    
    if (iteration >= max_iterations) {
        printf("Maximum iterations reached\n");
    }
}

int main(void) {
    struct context ctx;
    ctx.current_state = STATE_IDLE;
    ctx.buffer[0] = '\0';
    ctx.buffer_len = 0;
    ctx.processed_count = 0;
    ctx.max_items = 10;
    
    run_state_machine(&ctx);
    
    return 0;
}