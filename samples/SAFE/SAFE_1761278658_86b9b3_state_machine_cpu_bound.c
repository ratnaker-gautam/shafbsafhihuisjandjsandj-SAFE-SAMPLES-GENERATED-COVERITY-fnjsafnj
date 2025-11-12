//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

enum state {
    STATE_IDLE,
    STATE_PROCESSING,
    STATE_COMPLETED,
    STATE_ERROR,
    STATE_COUNT
};

struct context {
    enum state current_state;
    uint32_t counter;
    uint32_t max_iterations;
    uint32_t error_threshold;
};

static int validate_input(const char *input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > 10) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] < '0' || input[i] > '9') return 0;
    }
    return 1;
}

static uint32_t parse_uint32(const char *input) {
    unsigned long long val = 0;
    for (size_t i = 0; input[i] != '\0'; i++) {
        if (val > UINT32_MAX / 10) return UINT32_MAX;
        val = val * 10 + (input[i] - '0');
        if (val > UINT32_MAX) return UINT32_MAX;
    }
    return (uint32_t)val;
}

static void state_idle(struct context *ctx) {
    if (ctx == NULL) return;
    ctx->current_state = STATE_PROCESSING;
    ctx->counter = 0;
}

static void state_processing(struct context *ctx) {
    if (ctx == NULL) return;
    if (ctx->counter >= ctx->max_iterations) {
        ctx->current_state = STATE_COMPLETED;
        return;
    }
    uint32_t result = 0;
    for (uint32_t i = 0; i < ctx->counter; i++) {
        result += i * i;
        if (result > UINT32_MAX / 2) result = 0;
    }
    ctx->counter++;
    if (ctx->counter % ctx->error_threshold == 0) {
        ctx->current_state = STATE_ERROR;
    }
}

static void state_completed(struct context *ctx) {
    if (ctx == NULL) return;
}

static void state_error(struct context *ctx) {
    if (ctx == NULL) return;
    ctx->current_state = STATE_IDLE;
}

static const char *state_name(enum state s) {
    switch (s) {
        case STATE_IDLE: return "IDLE";
        case STATE_PROCESSING: return "PROCESSING";
        case STATE_COMPLETED: return "COMPLETED";
        case STATE_ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

int main(void) {
    struct context ctx;
    ctx.current_state = STATE_IDLE;
    ctx.max_iterations = 1000;
    ctx.error_threshold = 100;

    char input[32];
    printf("Enter maximum iterations (default 1000): ");
    if (fgets(input, sizeof(input), stdin) != NULL) {
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') input[len - 1] = '\0';
        if (validate_input(input)) {
            uint32_t val = parse_uint32(input);
            if (val > 0 && val <= 1000000) {
                ctx.max_iterations = val;
            }
        }
    }

    printf("Enter error threshold (default 100): ");
    if (fgets(input, sizeof(input), stdin) != NULL) {
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') input[len - 1] = '\0';
        if (validate_input(input)) {
            uint32_t val = parse_uint32(input);
            if (val > 0 && val <= ctx.max_iterations) {
                ctx.error_threshold = val;
            }
        }
    }

    printf("Starting state machine with max_iterations=%u, error_threshold=%u\n", ctx.max_iterations, ctx.error_threshold);

    uint32_t cycles = 0;
    while (cycles < 10000) {
        printf("Cycle %u: State=%s\n", cycles, state_name(ctx.current_state));
        switch (ctx.current_state) {
            case STATE_IDLE:
                state_idle(&ctx);
                break;
            case STATE_PROCESSING:
                state_processing(&ctx);
                break;
            case STATE_COMPLETED:
                state_completed(&ctx);
                break;
            case STATE_ERROR:
                state_error(&ctx);
                break;
            default:
                ctx.current_state = STATE_IDLE;
                break;
        }
        cycles++;
        if (ctx.current_state == STATE_COMPLETED) break;
    }

    printf("Final state: %s after %u cycles\n", state_name(ctx.current_state), cycles);
    return 0;
}