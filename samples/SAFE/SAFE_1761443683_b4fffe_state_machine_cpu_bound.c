//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

enum state {
    STATE_IDLE,
    STATE_PROCESSING,
    STATE_COMPLETE,
    STATE_ERROR,
    STATE_COUNT
};

struct context {
    enum state current_state;
    uint32_t counter;
    uint32_t max_iterations;
    char buffer[256];
    size_t buffer_pos;
};

static void state_idle_enter(struct context *ctx) {
    ctx->counter = 0;
    ctx->buffer_pos = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
}

static enum state state_idle_run(struct context *ctx) {
    if (ctx->buffer_pos >= sizeof(ctx->buffer) - 1) {
        return STATE_ERROR;
    }
    ctx->buffer[ctx->buffer_pos++] = 'A' + (ctx->counter % 26);
    ctx->counter++;
    if (ctx->counter >= 10) {
        return STATE_PROCESSING;
    }
    return STATE_IDLE;
}

static void state_processing_enter(struct context *ctx) {
    ctx->counter = 0;
}

static enum state state_processing_run(struct context *ctx) {
    if (ctx->buffer_pos == 0 || ctx->buffer_pos >= sizeof(ctx->buffer)) {
        return STATE_ERROR;
    }
    for (size_t i = 0; i < ctx->buffer_pos; i++) {
        if (ctx->buffer[i] >= 'A' && ctx->buffer[i] <= 'Z') {
            ctx->buffer[i] = ctx->buffer[i] + ('a' - 'A');
        }
    }
    ctx->counter++;
    if (ctx->counter >= 5) {
        return STATE_COMPLETE;
    }
    return STATE_PROCESSING;
}

static void state_complete_enter(struct context *ctx) {
    ctx->counter = 0;
}

static enum state state_complete_run(struct context *ctx) {
    if (ctx->buffer_pos == 0 || ctx->buffer_pos >= sizeof(ctx->buffer)) {
        return STATE_ERROR;
    }
    uint32_t sum = 0;
    for (size_t i = 0; i < ctx->buffer_pos; i++) {
        sum += (uint32_t)ctx->buffer[i];
        if (sum > UINT32_MAX - 255) {
            sum = UINT32_MAX;
            break;
        }
    }
    ctx->counter = sum % 1000;
    return STATE_COMPLETE;
}

static void state_error_enter(struct context *ctx) {
    ctx->counter = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    strncpy(ctx->buffer, "ERROR", sizeof(ctx->buffer) - 1);
    ctx->buffer[sizeof(ctx->buffer) - 1] = '\0';
    ctx->buffer_pos = strlen(ctx->buffer);
}

static enum state state_error_run(struct context *ctx) {
    ctx->counter++;
    if (ctx->counter >= 3) {
        return STATE_IDLE;
    }
    return STATE_ERROR;
}

static void run_state_machine(struct context *ctx) {
    enum state next_state = ctx->current_state;
    
    switch (ctx->current_state) {
        case STATE_IDLE:
            next_state = state_idle_run(ctx);
            break;
        case STATE_PROCESSING:
            next_state = state_processing_run(ctx);
            break;
        case STATE_COMPLETE:
            next_state = state_complete_run(ctx);
            break;
        case STATE_ERROR:
            next_state = state_error_run(ctx);
            break;
        default:
            next_state = STATE_ERROR;
            break;
    }
    
    if (next_state != ctx->current_state) {
        switch (next_state) {
            case STATE_IDLE:
                state_idle_enter(ctx);
                break;
            case STATE_PROCESSING:
                state_processing_enter(ctx);
                break;
            case STATE_COMPLETE:
                state_complete_enter(ctx);
                break;
            case STATE_ERROR:
                state_error_enter(ctx);
                break;
            default:
                break;
        }
        ctx->current_state = next_state;
    }
}

int main(void) {
    struct context ctx;
    ctx.current_state = STATE_IDLE;
    ctx.max_iterations = 50;
    
    state_idle_enter(&ctx);
    
    uint32_t iteration = 0;
    while (iteration < ctx.max_iterations) {
        run_state_machine(&ctx);
        
        if (ctx.current_state == STATE_COMPLETE) {
            printf("State: COMPLETE, Counter: %u, Buffer: %s\n", 
                   ctx.counter, ctx.buffer);
            break;
        } else if (ctx.current_state == STATE_ERROR) {
            printf("State: ERROR, Counter: %u, Buffer: %s\n", 
                   ctx.counter, ctx.buffer);
        }
        
        iteration++;
        if (iteration >= ctx.max_iterations) {
            printf("State: TIMEOUT, Final State: %d\n", ctx.current_state);
            break;
        }
    }
    
    return 0;
}