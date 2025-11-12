//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

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
    uint32_t data_value;
    uint32_t processed_value;
    int iteration_count;
    int max_iterations;
};

static int validate_input(const char *input) {
    if (input == NULL) return 0;
    if (strlen(input) == 0) return 0;
    for (size_t i = 0; input[i] != '\0'; i++) {
        if (input[i] < '0' || input[i] > '9') return 0;
    }
    return 1;
}

static uint32_t safe_strtoul(const char *str) {
    if (str == NULL) return 0;
    char *endptr;
    unsigned long value = strtoul(str, &endptr, 10);
    if (endptr == str || *endptr != '\0') return 0;
    if (value > UINT32_MAX) return 0;
    return (uint32_t)value;
}

static void state_idle(struct context *ctx) {
    if (ctx->iteration_count >= ctx->max_iterations) {
        ctx->current_state = STATE_DONE;
        return;
    }
    ctx->data_value = (ctx->data_value * 1103515245 + 12345) % 2147483648;
    ctx->current_state = STATE_READING;
}

static void state_reading(struct context *ctx) {
    if (ctx->data_value == 0) {
        ctx->current_state = STATE_ERROR;
        return;
    }
    ctx->current_state = STATE_PROCESSING;
}

static void state_processing(struct context *ctx) {
    uint32_t temp = ctx->data_value;
    for (int i = 0; i < 1000; i++) {
        temp = (temp * 1664525 + 1013904223) % 2147483648;
        temp ^= temp >> 15;
        temp ^= temp << 7;
        temp ^= temp >> 12;
    }
    ctx->processed_value = temp;
    ctx->current_state = STATE_WRITING;
}

static void state_writing(struct context *ctx) {
    printf("Iteration %d: input=%u output=%u\n", 
           ctx->iteration_count + 1, ctx->data_value, ctx->processed_value);
    ctx->iteration_count++;
    ctx->current_state = STATE_IDLE;
}

static void state_error(struct context *ctx) {
    printf("Error: Invalid data value detected\n");
    ctx->current_state = STATE_DONE;
}

static void run_state_machine(struct context *ctx) {
    while (ctx->current_state != STATE_DONE && ctx->current_state != STATE_ERROR) {
        switch (ctx->current_state) {
            case STATE_IDLE:
                state_idle(ctx);
                break;
            case STATE_READING:
                state_reading(ctx);
                break;
            case STATE_PROCESSING:
                state_processing(ctx);
                break;
            case STATE_WRITING:
                state_writing(ctx);
                break;
            case STATE_ERROR:
                state_error(ctx);
                break;
            default:
                ctx->current_state = STATE_ERROR;
                break;
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <initial_value> <iterations>\n", argv[0]);
        return 1;
    }

    if (!validate_input(argv[1]) || !validate_input(argv[2])) {
        fprintf(stderr, "Error: Invalid input format\n");
        return 1;
    }

    uint32_t initial_value = safe_strtoul(argv[1]);
    int iterations = (int)safe_strtoul(argv[2]);

    if (iterations <= 0 || iterations > 1000000) {
        fprintf(stderr, "Error: Iterations must be between 1 and 1000000\n");
        return 1;
    }

    struct context ctx;
    ctx.current_state = STATE_IDLE;
    ctx.data_value = initial_value;
    ctx.processed_value = 0;
    ctx.iteration_count = 0;
    ctx.max_iterations = iterations;

    run_state_machine(&ctx);

    return 0;
}