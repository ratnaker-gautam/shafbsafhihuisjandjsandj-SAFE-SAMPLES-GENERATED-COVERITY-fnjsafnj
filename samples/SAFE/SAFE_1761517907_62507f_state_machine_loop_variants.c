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

void state_init(struct context* ctx) {
    ctx->current_state = READY;
    ctx->data_value = 0;
    ctx->step_count = 0;
    ctx->max_steps = 10;
}

void state_ready(struct context* ctx) {
    char input[32];
    printf("Enter initial value (0-100): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        ctx->current_state = ERROR;
        return;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }
    
    if (!validate_input(input)) {
        ctx->current_state = ERROR;
        return;
    }
    
    int value;
    if (!safe_atoi(input, &value)) {
        ctx->current_state = ERROR;
        return;
    }
    
    if (value < 0 || value > 100) {
        ctx->current_state = ERROR;
        return;
    }
    
    ctx->data_value = value;
    ctx->current_state = PROCESSING;
}

void state_processing(struct context* ctx) {
    if (ctx->step_count >= ctx->max_steps) {
        ctx->current_state = FINISHED;
        return;
    }
    
    int operation = ctx->step_count % 4;
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
        case 3:
            if (ctx->data_value >= 2) {
                ctx->data_value /= 2;
            }
            break;
    }
    
    printf("Step %d: %d -> %d (op: %d)\n", 
           ctx->step_count + 1, old_value, ctx->data_value, operation);
    
    ctx->step_count++;
    
    if (ctx->data_value <= 0 || ctx->data_value > 10000) {
        ctx->current_state = FINISHED;
    }
}

void state_finished(struct context* ctx) {
    printf("Processing finished. Final value: %d\n", ctx->data_value);
    ctx->current_state = INIT;
}

void state_error(struct context* ctx) {
    printf("Error occurred. Resetting state machine.\n");
    ctx->current_state = INIT;
}

int main(void) {
    struct context ctx;
    state_init(&ctx);
    
    int cycle_count = 0;
    const int max_cycles = 3;
    
    while (cycle_count < max_cycles) {
        switch (ctx.current_state) {
            case INIT:
                state_init(&ctx);
                break;
            case READY:
                state_ready(&ctx);
                break;
            case PROCESSING:
                state_processing(&ctx);
                break;
            case FINISHED:
                state_finished(&ctx);
                cycle_count++;
                break;
            case ERROR:
                state_error(&ctx);
                break;
        }
        
        if (ctx.current_state == FINISHED && cycle_count >= max_cycles) {
            break;
        }
    }
    
    printf("Completed %d processing cycles.\n", cycle_count);
    return 0;
}