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

static uint32_t safe_atoi(const char *str) {
    if (str == NULL) return 0;
    unsigned long long result = 0;
    for (size_t i = 0; str[i] != '\0'; i++) {
        if (result > UINT32_MAX / 10) return UINT32_MAX;
        result = result * 10 + (str[i] - '0');
        if (result > UINT32_MAX) return UINT32_MAX;
    }
    return (uint32_t)result;
}

static void state_idle(struct context *ctx) {
    ctx->data_value = 0;
    ctx->processed_value = 0;
    ctx->iteration_count = 0;
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
    if (len > 0 && input[len-1] == '\n') input[len-1] = '\0';
    if (!validate_input(input)) {
        ctx->current_state = STATE_ERROR;
        return;
    }
    ctx->data_value = safe_atoi(input);
    ctx->current_state = STATE_PROCESSING;
}

static void state_processing(struct context *ctx) {
    if (ctx->iteration_count >= ctx->max_iterations) {
        ctx->current_state = STATE_WRITING;
        return;
    }
    uint32_t temp = ctx->data_value;
    for (int i = 0; i < 1000; i++) {
        temp = (temp * 1103515245U + 12345U) % 2147483648U;
        temp = (temp >> 16) | (temp << 16);
        temp = temp ^ 0x5A5A5A5A;
    }
    ctx->processed_value = temp;
    ctx->data_value = ctx->processed_value;
    ctx->iteration_count++;
    if (ctx->iteration_count < ctx->max_iterations) {
        ctx->current_state = STATE_PROCESSING;
    } else {
        ctx->current_state = STATE_WRITING;
    }
}

static void state_writing(struct context *ctx) {
    printf("Final result after %d iterations: %u\n", 
           ctx->iteration_count, ctx->processed_value);
    ctx->current_state = STATE_DONE;
}

static void state_error(struct context *ctx) {
    printf("Error: Invalid input detected\n");
    ctx->current_state = STATE_DONE;
}

int main(void) {
    struct context ctx;
    ctx.current_state = STATE_IDLE;
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