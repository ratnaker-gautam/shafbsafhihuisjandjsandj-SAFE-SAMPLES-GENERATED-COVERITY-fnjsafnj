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
    if (strlen(input) > 10) return 0;
    for (size_t i = 0; i < strlen(input); i++) {
        if (input[i] < '0' || input[i] > '9') return 0;
    }
    return 1;
}

static uint32_t safe_str_to_uint32(const char *str) {
    unsigned long long val = 0;
    for (size_t i = 0; i < strlen(str); i++) {
        if (val > UINT32_MAX / 10) return UINT32_MAX;
        val = val * 10;
        int digit = str[i] - '0';
        if (val > UINT32_MAX - digit) return UINT32_MAX;
        val += digit;
    }
    return (uint32_t)val;
}

static void state_idle(struct context *ctx) {
    ctx->current_state = STATE_READING;
}

static void state_reading(struct context *ctx) {
    char input[32];
    printf("Enter initial value (0-4294967295): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        ctx->current_state = STATE_ERROR;
        return;
    }
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    if (!validate_input(input)) {
        ctx->current_state = STATE_ERROR;
        return;
    }
    ctx->data_value = safe_str_to_uint32(input);
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
    ctx->data_value = temp;
    ctx->iteration_count++;
    if (ctx->iteration_count < ctx->max_iterations) {
        ctx->current_state = STATE_PROCESSING;
    } else {
        ctx->current_state = STATE_WRITING;
    }
}

static void state_writing(struct context *ctx) {
    printf("Final processed value: %u\n", ctx->processed_value);
    ctx->current_state = STATE_DONE;
}

static void state_error(struct context *ctx) {
    printf("Error: Invalid input detected\n");
    ctx->current_state = STATE_DONE;
}

int main(void) {
    struct context ctx;
    ctx.current_state = STATE_IDLE;
    ctx.data_value = 0;
    ctx.processed_value = 0;
    ctx.iteration_count = 0;
    ctx.max_iterations = 10000;
    
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