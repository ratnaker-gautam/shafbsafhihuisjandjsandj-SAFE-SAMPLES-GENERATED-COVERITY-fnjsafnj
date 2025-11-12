//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State { START, PROCESSING, VALIDATING, COMPLETED, ERROR };

struct Context {
    enum State current_state;
    int data_value;
    int step_count;
    char buffer[32];
};

enum State handle_start(struct Context *ctx) {
    if (ctx->step_count >= 10) return ERROR;
    printf("Enter initial value (0-100): ");
    if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) == NULL) return ERROR;
    char *endptr;
    long val = strtol(ctx->buffer, &endptr, 10);
    if (endptr == ctx->buffer || *endptr != '\n') return ERROR;
    if (val < 0 || val > 100) return ERROR;
    ctx->data_value = (int)val;
    ctx->step_count++;
    return PROCESSING;
}

enum State handle_processing(struct Context *ctx) {
    if (ctx->step_count >= 10) return ERROR;
    if (ctx->data_value < 0) return ERROR;
    ctx->data_value *= 2;
    if (ctx->data_value > 1000) ctx->data_value = 1000;
    ctx->step_count++;
    return VALIDATING;
}

enum State handle_validating(struct Context *ctx) {
    if (ctx->step_count >= 10) return ERROR;
    if (ctx->data_value % 3 == 0) {
        ctx->data_value /= 3;
        return COMPLETED;
    } else {
        ctx->data_value -= 5;
        if (ctx->data_value < 0) return ERROR;
        return PROCESSING;
    }
}

enum State handle_completed(struct Context *ctx) {
    printf("Final result: %d\n", ctx->data_value);
    return COMPLETED;
}

enum State handle_error(struct Context *ctx) {
    printf("Error state reached\n");
    return ERROR;
}

enum State execute_state(struct Context *ctx) {
    switch (ctx->current_state) {
        case START: return handle_start(ctx);
        case PROCESSING: return handle_processing(ctx);
        case VALIDATING: return handle_validating(ctx);
        case COMPLETED: return handle_completed(ctx);
        case ERROR: return handle_error(ctx);
        default: return ERROR;
    }
}

void state_machine_recursive(struct Context *ctx, int depth) {
    if (depth > 20) {
        ctx->current_state = ERROR;
        return;
    }
    enum State next_state = execute_state(ctx);
    if (next_state == ctx->current_state && (next_state == COMPLETED || next_state == ERROR)) {
        return;
    }
    ctx->current_state = next_state;
    state_machine_recursive(ctx, depth + 1);
}

int main(void) {
    struct Context ctx;
    ctx.current_state = START;
    ctx.data_value = 0;
    ctx.step_count = 0;
    memset(ctx.buffer, 0, sizeof(ctx.buffer));
    
    state_machine_recursive(&ctx, 0);
    
    return ctx.current_state == COMPLETED ? 0 : 1;
}