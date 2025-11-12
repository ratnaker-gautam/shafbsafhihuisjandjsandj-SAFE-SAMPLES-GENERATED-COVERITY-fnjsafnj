//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define MAX_INPUT_LEN 100
#define STATE_COUNT 5

typedef enum {
    STATE_START,
    STATE_READING,
    STATE_PROCESSING,
    STATE_WRITING,
    STATE_DONE
} state_t;

typedef struct {
    state_t current_state;
    uint32_t data_value;
    uint32_t processed_value;
    uint32_t iteration_count;
} context_t;

static state_t handle_start(context_t *ctx) {
    if (ctx->iteration_count >= 1000000) {
        return STATE_DONE;
    }
    ctx->data_value = ctx->iteration_count * 2;
    return STATE_READING;
}

static state_t handle_reading(context_t *ctx) {
    if (ctx->data_value > 2000000) {
        return STATE_DONE;
    }
    return STATE_PROCESSING;
}

static state_t handle_processing(context_t *ctx) {
    uint32_t temp = ctx->data_value;
    for (int i = 0; i < 100; i++) {
        temp = (temp * 1103515245 + 12345) & 0x7FFFFFFF;
    }
    ctx->processed_value = temp % 1000;
    return STATE_WRITING;
}

static state_t handle_writing(context_t *ctx) {
    if (ctx->iteration_count % 100000 == 0) {
        printf("Iteration: %u, Value: %u\n", ctx->iteration_count, ctx->processed_value);
    }
    ctx->iteration_count++;
    return STATE_START;
}

static state_t handle_done(context_t *ctx) {
    printf("Final iteration count: %u\n", ctx->iteration_count);
    return STATE_DONE;
}

static void run_state_machine(void) {
    context_t ctx = {STATE_START, 0, 0, 0};
    int max_iterations = 10000000;
    int safety_counter = 0;
    
    while (ctx.current_state != STATE_DONE && safety_counter < max_iterations) {
        switch (ctx.current_state) {
            case STATE_START:
                ctx.current_state = handle_start(&ctx);
                break;
            case STATE_READING:
                ctx.current_state = handle_reading(&ctx);
                break;
            case STATE_PROCESSING:
                ctx.current_state = handle_processing(&ctx);
                break;
            case STATE_WRITING:
                ctx.current_state = handle_writing(&ctx);
                break;
            case STATE_DONE:
                ctx.current_state = handle_done(&ctx);
                break;
            default:
                ctx.current_state = STATE_DONE;
                break;
        }
        safety_counter++;
    }
    
    if (safety_counter >= max_iterations) {
        printf("Safety limit reached\n");
    }
}

int main(void) {
    printf("Starting CPU-bound state machine simulation\n");
    run_state_machine();
    printf("Simulation completed\n");
    return 0;
}