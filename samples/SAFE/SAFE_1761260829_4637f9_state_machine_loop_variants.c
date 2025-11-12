//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

enum state {
    INIT,
    READY,
    PROCESSING,
    FINISHED,
    ERROR
};

struct context {
    enum state current_state;
    int data_value;
    int step_count;
    int max_steps;
};

int validate_input(const char* input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > 10) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] < '0' || input[i] > '9') return 0;
    }
    return 1;
}

int safe_atoi(const char* str, int* result) {
    if (str == NULL || result == NULL) return 0;
    long val = 0;
    int sign = 1;
    size_t i = 0;
    
    if (str[0] == '-') {
        sign = -1;
        i = 1;
    }
    
    for (; str[i] != '\0'; i++) {
        if (str[i] < '0' || str[i] > '9') return 0;
        val = val * 10 + (str[i] - '0');
        if (val > INT32_MAX) return 0;
    }
    
    val *= sign;
    if (val < INT32_MIN || val > INT32_MAX) return 0;
    *result = (int)val;
    return 1;
}

enum state handle_init(struct context* ctx) {
    ctx->data_value = 0;
    ctx->step_count = 0;
    ctx->max_steps = 10;
    return READY;
}

enum state handle_ready(struct context* ctx) {
    char input[32];
    printf("Enter initial value (0-100): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return ERROR;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (!validate_input(input)) {
        printf("Invalid input\n");
        return ERROR;
    }
    
    int value;
    if (!safe_atoi(input, &value)) {
        printf("Conversion error\n");
        return ERROR;
    }
    
    if (value < 0 || value > 100) {
        printf("Value out of range\n");
        return ERROR;
    }
    
    ctx->data_value = value;
    return PROCESSING;
}

enum state handle_processing(struct context* ctx) {
    if (ctx->step_count >= ctx->max_steps) {
        return FINISHED;
    }
    
    int operation = ctx->step_count % 3;
    int old_value = ctx->data_value;
    
    switch (operation) {
        case 0:
            if (ctx->data_value <= INT32_MAX - 5) {
                ctx->data_value += 5;
            }
            break;
        case 1:
            if (ctx->data_value >= 3) {
                ctx->data_value -= 3;
            }
            break;
        case 2:
            if (ctx->data_value <= INT32_MAX / 2) {
                ctx->data_value *= 2;
            }
            break;
    }
    
    printf("Step %d: %d -> %d\n", ctx->step_count + 1, old_value, ctx->data_value);
    ctx->step_count++;
    
    if (ctx->step_count >= ctx->max_steps) {
        return FINISHED;
    }
    
    return PROCESSING;
}

enum state handle_finished(struct context* ctx) {
    printf("Final result: %d\n", ctx->data_value);
    return FINISHED;
}

enum state handle_error(struct context* ctx) {
    printf("Error state reached\n");
    return ERROR;
}

int main(void) {
    struct context ctx = {INIT, 0, 0, 10};
    int running = 1;
    
    while (running) {
        switch (ctx.current_state) {
            case INIT:
                ctx.current_state = handle_init(&ctx);
                break;
            case READY:
                ctx.current_state = handle_ready(&ctx);
                break;
            case PROCESSING:
                ctx.current_state = handle_processing(&ctx);
                break;
            case FINISHED:
                ctx.current_state = handle_finished(&ctx);
                running = 0;
                break;
            case ERROR:
                ctx.current_state = handle_error(&ctx);
                running = 0;
                break;
        }
    }
    
    return ctx.current_state == FINISHED ? 0 : 1;
}