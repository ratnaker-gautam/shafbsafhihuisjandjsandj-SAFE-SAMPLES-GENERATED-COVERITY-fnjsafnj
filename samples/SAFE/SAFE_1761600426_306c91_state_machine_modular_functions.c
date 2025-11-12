//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

enum state {
    STATE_IDLE,
    STATE_READY,
    STATE_PROCESSING,
    STATE_COMPLETE,
    STATE_ERROR
};

struct context {
    enum state current_state;
    uint32_t counter;
    char buffer[64];
};

int validate_input(const char* input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > 63) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] < '0' || input[i] > '9') return 0;
    }
    return 1;
}

enum state handle_idle(struct context* ctx, const char* input) {
    if (ctx == NULL || input == NULL) return STATE_ERROR;
    if (validate_input(input)) {
        strncpy(ctx->buffer, input, sizeof(ctx->buffer) - 1);
        ctx->buffer[sizeof(ctx->buffer) - 1] = '\0';
        ctx->counter = 0;
        return STATE_READY;
    }
    return STATE_ERROR;
}

enum state handle_ready(struct context* ctx) {
    if (ctx == NULL) return STATE_ERROR;
    if (ctx->counter > 1000) return STATE_ERROR;
    ctx->counter++;
    return STATE_PROCESSING;
}

enum state handle_processing(struct context* ctx) {
    if (ctx == NULL) return STATE_ERROR;
    uint32_t value;
    if (sscanf(ctx->buffer, "%u", &value) != 1) return STATE_ERROR;
    if (value > 10000) return STATE_ERROR;
    value *= 2;
    if (value > 10000) return STATE_ERROR;
    if (snprintf(ctx->buffer, sizeof(ctx->buffer), "%u", value) >= sizeof(ctx->buffer)) return STATE_ERROR;
    if (ctx->counter >= 3) return STATE_COMPLETE;
    return STATE_READY;
}

enum state handle_complete(struct context* ctx) {
    if (ctx == NULL) return STATE_ERROR;
    printf("Final result: %s\n", ctx->buffer);
    return STATE_IDLE;
}

enum state handle_error(struct context* ctx) {
    if (ctx == NULL) return STATE_ERROR;
    printf("Error occurred in state machine\n");
    return STATE_IDLE;
}

void run_state_machine(struct context* ctx) {
    if (ctx == NULL) return;
    char input[128];
    int running = 1;
    while (running) {
        switch (ctx->current_state) {
            case STATE_IDLE:
                printf("Enter a number (or 'quit' to exit): ");
                if (fgets(input, sizeof(input), stdin) == NULL) break;
                input[strcspn(input, "\n")] = '\0';
                if (strcmp(input, "quit") == 0) {
                    running = 0;
                    break;
                }
                ctx->current_state = handle_idle(ctx, input);
                break;
            case STATE_READY:
                ctx->current_state = handle_ready(ctx);
                break;
            case STATE_PROCESSING:
                ctx->current_state = handle_processing(ctx);
                break;
            case STATE_COMPLETE:
                ctx->current_state = handle_complete(ctx);
                break;
            case STATE_ERROR:
                ctx->current_state = handle_error(ctx);
                break;
        }
    }
}

int main(void) {
    struct context ctx;
    ctx.current_state = STATE_IDLE;
    ctx.counter = 0;
    memset(ctx.buffer, 0, sizeof(ctx.buffer));
    run_state_machine(&ctx);
    return 0;
}