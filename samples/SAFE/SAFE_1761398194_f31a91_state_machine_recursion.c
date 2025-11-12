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
    int data;
    int step_count;
    char buffer[32];
};

enum State handle_start(struct Context *ctx) {
    if (ctx->step_count >= 10) return ERROR;
    printf("Enter initial data (0-100): ");
    if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) == NULL) return ERROR;
    char *endptr;
    long val = strtol(ctx->buffer, &endptr, 10);
    if (endptr == ctx->buffer || *endptr != '\n') return ERROR;
    if (val < 0 || val > 100) return ERROR;
    ctx->data = (int)val;
    ctx->step_count++;
    return PROCESSING;
}

enum State handle_processing(struct Context *ctx) {
    if (ctx->step_count >= 10) return ERROR;
    if (ctx->data < 0) return ERROR;
    ctx->data *= 2;
    if (ctx->data > 1000) ctx->data = 1000;
    ctx->step_count++;
    return VALIDATING;
}

enum State handle_validating(struct Context *ctx) {
    if (ctx->step_count >= 10) return ERROR;
    if (ctx->data % 2 != 0) return ERROR;
    ctx->step_count++;
    return COMPLETED;
}

enum State handle_completed(struct Context *ctx) {
    printf("Processing completed. Final data: %d\n", ctx->data);
    return COMPLETED;
}

enum State handle_error(struct Context *ctx) {
    printf("Error occurred at step %d\n", ctx->step_count);
    return ERROR;
}

enum State state_machine_step(struct Context *ctx) {
    switch (ctx->current_state) {
        case START: return handle_start(ctx);
        case PROCESSING: return handle_processing(ctx);
        case VALIDATING: return handle_validating(ctx);
        case COMPLETED: return handle_completed(ctx);
        case ERROR: return handle_error(ctx);
        default: return ERROR;
    }
}

void run_state_machine(struct Context *ctx, int max_depth) {
    if (max_depth <= 0) {
        ctx->current_state = ERROR;
        return;
    }
    
    enum State next_state = state_machine_step(ctx);
    if (next_state == ctx->current_state) return;
    
    ctx->current_state = next_state;
    
    if (ctx->current_state != COMPLETED && ctx->current_state != ERROR) {
        run_state_machine(ctx, max_depth - 1);
    }
}

int main(void) {
    struct Context ctx;
    ctx.current_state = START;
    ctx.data = 0;
    ctx.step_count = 0;
    memset(ctx.buffer, 0, sizeof(ctx.buffer));
    
    run_state_machine(&ctx, 20);
    
    if (ctx.current_state == COMPLETED) {
        return EXIT_SUCCESS;
    } else {
        return EXIT_FAILURE;
    }
}