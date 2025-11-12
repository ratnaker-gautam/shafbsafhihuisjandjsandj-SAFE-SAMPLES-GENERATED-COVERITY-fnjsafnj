//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define STATE_COUNT 8
#define MAX_ITERATIONS 1000000

typedef enum {
    STATE_IDLE,
    STATE_PROCESSING,
    STATE_VALIDATING,
    STATE_COMPUTING,
    STATE_AGGREGATING,
    STATE_FINALIZING,
    STATE_ERROR,
    STATE_DONE
} state_t;

typedef struct {
    state_t current_state;
    uint64_t counter;
    uint32_t data[STATE_COUNT];
    int iteration;
} context_t;

static state_t state_idle(context_t *ctx) {
    if (ctx->counter > UINT64_MAX - 1) return STATE_ERROR;
    ctx->counter++;
    ctx->data[STATE_IDLE] = (ctx->data[STATE_IDLE] + 1) % 1000;
    return STATE_PROCESSING;
}

static state_t state_processing(context_t *ctx) {
    if (ctx->counter > UINT64_MAX - 2) return STATE_ERROR;
    ctx->counter += 2;
    ctx->data[STATE_PROCESSING] = (ctx->data[STATE_PROCESSING] * 3 + 1) % 1000;
    return STATE_VALIDATING;
}

static state_t state_validating(context_t *ctx) {
    if (ctx->counter > UINT64_MAX - 3) return STATE_ERROR;
    ctx->counter += 3;
    ctx->data[STATE_VALIDATING] = (ctx->data[STATE_VALIDATING] + ctx->counter) % 1000;
    return STATE_COMPUTING;
}

static state_t state_computing(context_t *ctx) {
    if (ctx->counter > UINT64_MAX - 5) return STATE_ERROR;
    ctx->counter += 5;
    uint32_t temp = ctx->data[STATE_COMPUTING];
    for (int i = 0; i < 10; i++) {
        temp = (temp * 7 + 11) % 1000;
    }
    ctx->data[STATE_COMPUTING] = temp;
    return STATE_AGGREGATING;
}

static state_t state_aggregating(context_t *ctx) {
    if (ctx->counter > UINT64_MAX - 7) return STATE_ERROR;
    ctx->counter += 7;
    uint32_t sum = 0;
    for (int i = 0; i < STATE_COUNT; i++) {
        if (sum > UINT32_MAX - ctx->data[i]) return STATE_ERROR;
        sum += ctx->data[i];
    }
    ctx->data[STATE_AGGREGATING] = sum % 1000;
    return STATE_FINALIZING;
}

static state_t state_finalizing(context_t *ctx) {
    if (ctx->counter > UINT64_MAX - 11) return STATE_ERROR;
    ctx->counter += 11;
    ctx->data[STATE_FINALIZING] = (ctx->data[STATE_FINALIZING] + ctx->counter) % 1000;
    if (ctx->iteration >= MAX_ITERATIONS - 1) return STATE_DONE;
    return STATE_IDLE;
}

static state_t state_error(context_t *ctx) {
    ctx->data[STATE_ERROR] = (ctx->data[STATE_ERROR] + 1) % 1000;
    return STATE_DONE;
}

static state_t state_done(context_t *ctx) {
    return STATE_DONE;
}

static void run_state_machine(context_t *ctx) {
    state_t (*state_handlers[STATE_COUNT])(context_t *) = {
        state_idle,
        state_processing,
        state_validating,
        state_computing,
        state_aggregating,
        state_finalizing,
        state_error,
        state_done
    };

    while (ctx->current_state != STATE_DONE) {
        if (ctx->current_state < 0 || ctx->current_state >= STATE_COUNT) {
            ctx->current_state = STATE_ERROR;
            continue;
        }
        ctx->current_state = state_handlers[ctx->current_state](ctx);
        ctx->iteration++;
        if (ctx->iteration >= MAX_ITERATIONS && ctx->current_state != STATE_DONE) {
            ctx->current_state = STATE_ERROR;
        }
    }
}

int main(void) {
    context_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    ctx.current_state = STATE_IDLE;

    for (int i = 0; i < STATE_COUNT; i++) {
        ctx.data[i] = i * 17 % 1000;
    }

    run_state_machine(&ctx);

    uint64_t checksum = 0;
    for (int i = 0; i < STATE_COUNT; i++) {
        if (checksum > UINT64_MAX - ctx.data[i]) {
            printf("Checksum overflow detected\n");
            return 1;
        }
        checksum += ctx.data[i];
    }
    checksum += ctx.counter;
    checksum += ctx.iteration;

    printf("Final counter: %lu\n", (unsigned long)ctx.counter);
    printf("Total iterations: %d\n", ctx.iteration);
    printf("Checksum: %lu\n", (unsigned long)checksum);

    return 0;
}