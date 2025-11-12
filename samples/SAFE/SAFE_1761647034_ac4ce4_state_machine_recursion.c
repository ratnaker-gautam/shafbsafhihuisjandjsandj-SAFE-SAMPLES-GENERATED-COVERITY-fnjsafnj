//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State { START, PROCESSING, VALIDATING, FINISHED, ERROR };

struct Context {
    enum State current_state;
    int data_value;
    int step_count;
    char buffer[32];
};

enum State handle_start(struct Context *ctx, int input) {
    if (input < 0 || input > 100) {
        return ERROR;
    }
    ctx->data_value = input;
    ctx->step_count = 0;
    snprintf(ctx->buffer, sizeof(ctx->buffer), "Start:%d", input);
    return PROCESSING;
}

enum State handle_processing(struct Context *ctx, int input) {
    if (input < 0 || input > 50) {
        return ERROR;
    }
    ctx->data_value += input;
    ctx->step_count++;
    snprintf(ctx->buffer, sizeof(ctx->buffer), "Process:%d", ctx->data_value);
    if (ctx->step_count >= 3) {
        return VALIDATING;
    }
    return PROCESSING;
}

enum State handle_validating(struct Context *ctx, int input) {
    if (input != 0 && input != 1) {
        return ERROR;
    }
    if (input == 1) {
        if (ctx->data_value > 200) {
            return ERROR;
        }
        snprintf(ctx->buffer, sizeof(ctx->buffer), "Valid:%d", ctx->data_value);
        return FINISHED;
    } else {
        ctx->data_value /= 2;
        ctx->step_count = 0;
        snprintf(ctx->buffer, sizeof(ctx->buffer), "Reset:%d", ctx->data_value);
        return PROCESSING;
    }
}

enum State handle_finished(struct Context *ctx, int input) {
    if (input == 0) {
        ctx->current_state = START;
        return START;
    }
    return FINISHED;
}

enum State handle_error(struct Context *ctx, int input) {
    if (input == -1) {
        ctx->current_state = START;
        return START;
    }
    return ERROR;
}

enum State process_state(struct Context *ctx, int input) {
    switch (ctx->current_state) {
        case START:
            return handle_start(ctx, input);
        case PROCESSING:
            return handle_processing(ctx, input);
        case VALIDATING:
            return handle_validating(ctx, input);
        case FINISHED:
            return handle_finished(ctx, input);
        case ERROR:
            return handle_error(ctx, input);
        default:
            return ERROR;
    }
}

void run_state_machine(struct Context *ctx, int depth) {
    if (depth <= 0 || ctx->current_state == FINISHED || ctx->current_state == ERROR) {
        return;
    }
    
    int input;
    printf("Current state: %d, Buffer: %s\n", ctx->current_state, ctx->buffer);
    printf("Enter input for state %d: ", ctx->current_state);
    
    if (scanf("%d", &input) != 1) {
        printf("Invalid input format\n");
        while (getchar() != '\n');
        ctx->current_state = ERROR;
        return;
    }
    
    enum State next_state = process_state(ctx, input);
    ctx->current_state = next_state;
    
    run_state_machine(ctx, depth - 1);
}

int main() {
    struct Context ctx;
    ctx.current_state = START;
    ctx.data_value = 0;
    ctx.step_count = 0;
    memset(ctx.buffer, 0, sizeof(ctx.buffer));
    
    printf("State Machine Demo - Recursive Implementation\n");
    printf("Valid inputs: 0-100 (START), 0-50 (PROCESSING), 0-1 (VALIDATING)\n");
    printf("Use -1 in ERROR state to reset, 0 in FINISHED to restart\n");
    
    run_state_machine(&ctx, 10);
    
    printf("Final state: %d, Result: %s\n", ctx.current_state, ctx.buffer);
    
    return 0;
}