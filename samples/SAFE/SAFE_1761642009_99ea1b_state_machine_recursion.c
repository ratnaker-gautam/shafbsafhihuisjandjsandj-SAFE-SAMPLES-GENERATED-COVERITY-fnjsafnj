//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State {
    START,
    PROCESSING,
    VALIDATING,
    COMPLETED,
    ERROR
};

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
    int value;
    if (sscanf(ctx->buffer, "%d", &value) != 1) return ERROR;
    if (value < 0 || value > 100) return ERROR;
    ctx->data_value = value;
    ctx->step_count++;
    return PROCESSING;
}

enum State handle_processing(struct Context *ctx) {
    if (ctx->step_count >= 10) return ERROR;
    if (ctx->data_value < 0) return ERROR;
    ctx->data_value *= 2;
    if (ctx->data_value > 1000) return ERROR;
    ctx->step_count++;
    return VALIDATING;
}

enum State handle_validating(struct Context *ctx) {
    if (ctx->step_count >= 10) return ERROR;
    if (ctx->data_value % 2 != 0) return ERROR;
    if (ctx->data_value > 500) {
        ctx->step_count++;
        return COMPLETED;
    }
    ctx->step_count++;
    return PROCESSING;
}

enum State handle_completed(struct Context *ctx) {
    printf("Final result: %d\n", ctx->data_value);
    return COMPLETED;
}

enum State handle_error(struct Context *ctx) {
    printf("Error state reached\n");
    return ERROR;
}

enum State process_state(struct Context *ctx) {
    if (ctx == NULL) return ERROR;
    
    switch (ctx->current_state) {
        case START:
            return handle_start(ctx);
        case PROCESSING:
            return handle_processing(ctx);
        case VALIDATING:
            return handle_validating(ctx);
        case COMPLETED:
            return handle_completed(ctx);
        case ERROR:
            return handle_error(ctx);
        default:
            return ERROR;
    }
}

void state_machine_recursive(struct Context *ctx, int depth) {
    if (ctx == NULL || depth >= 20) return;
    
    enum State next_state = process_state(ctx);
    if (next_state == ctx->current_state && next_state != COMPLETED && next_state != ERROR) {
        return;
    }
    
    ctx->current_state = next_state;
    
    if (ctx->current_state != COMPLETED && ctx->current_state != ERROR) {
        state_machine_recursive(ctx, depth + 1);
    }
}

int main(void) {
    struct Context ctx;
    ctx.current_state = START;
    ctx.data_value = 0;
    ctx.step_count = 0;
    memset(ctx.buffer, 0, sizeof(ctx.buffer));
    
    state_machine_recursive(&ctx, 0);
    
    if (ctx.current_state == COMPLETED) {
        printf("State machine completed successfully\n");
        return 0;
    } else {
        printf("State machine terminated with error\n");
        return 1;
    }
}