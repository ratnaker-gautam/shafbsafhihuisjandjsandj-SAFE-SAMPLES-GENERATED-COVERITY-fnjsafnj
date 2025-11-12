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
    size_t len = strlen(input);
    if (len == 0 || len > 10) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] < '0' || input[i] > '9') return 0;
    }
    return 1;
}

static uint32_t safe_strtoul(const char *str) {
    char *endptr;
    unsigned long val = strtoul(str, &endptr, 10);
    if (endptr == str || *endptr != '\0') return 0;
    if (val > UINT32_MAX) return 0;
    return (uint32_t)val;
}

static void state_idle(struct context *ctx) {
    ctx->current_state = STATE_READING;
    ctx->data_value = 0;
    ctx->processed_value = 0;
    ctx->iteration_count = 0;
}

static void state_reading(struct context *ctx) {
    char input[32];
    printf("Enter initial value (0-100000): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        ctx->current_state = STATE_ERROR;
        return;
    }
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') input[len-1] = '\0';
    if (!validate_input(input)) {
        ctx->current_state = STATE_ERROR;
        return;
    }
    ctx->data_value = safe_strtoul(input);
    if (ctx->data_value > 100000) {
        ctx->current_state = STATE_ERROR;
        return;
    }
    printf("Enter iterations (1-1000): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        ctx->current_state = STATE_ERROR;
        return;
    }
    len = strlen(input);
    if (len > 0 && input[len-1] == '\n') input[len-1] = '\0';
    if (!validate_input(input)) {
        ctx->current_state = STATE_ERROR;
        return;
    }
    ctx->max_iterations = safe_strtoul(input);
    if (ctx->max_iterations < 1 || ctx->max_iterations > 1000) {
        ctx->current_state = STATE_ERROR;
        return;
    }
    ctx->current_state = STATE_PROCESSING;
}

static void state_processing(struct context *ctx) {
    if (ctx->iteration_count >= ctx->max_iterations) {
        ctx->current_state = STATE_WRITING;
        return;
    }
    uint32_t temp = ctx->data_value;
    for (int i = 0; i < 1000; i++) {
        temp = (temp * 1103515245 + 12345) & 0x7FFFFFFF;
        temp = (temp >> 16) | (temp << 16);
        temp = temp ^ 0xDEADBEEF;
    }
    ctx->processed_value = temp;
    ctx->data_value = (ctx->data_value + 1) % 100001;
    ctx->iteration_count++;
    if (ctx->iteration_count < ctx->max_iterations) {
        ctx->current_state = STATE_PROCESSING;
    } else {
        ctx->current_state = STATE_WRITING;
    }
}

static void state_writing(struct context *ctx) {
    printf("Processing complete. Final value: %u\n", ctx->processed_value);
    ctx->current_state = STATE_DONE;
}

static void state_error(struct context *ctx) {
    printf("Error: Invalid input detected\n");
    ctx->current_state = STATE_DONE;
}

int main(void) {
    struct context ctx;
    ctx.current_state = STATE_IDLE;
    
    while (ctx.current_state != STATE_DONE) {
        switch (ctx.current_state) {
            case STATE_IDLE:
                state_idle(&ctx);
                break;
            case STATE_READING:
                state_reading(&ctx);
                break;
            case STATE_PROCESSING:
                state_processing(&ctx);
                break;
            case STATE_WRITING:
                state_writing(&ctx);
                break;
            case STATE_ERROR:
                state_error(&ctx);
                break;
            case STATE_DONE:
                break;
        }
    }
    
    return 0;
}