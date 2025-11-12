//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State {
    START,
    PROCESSING,
    VALIDATING,
    FINISHED,
    ERROR
};

struct Context {
    enum State current_state;
    int data;
    int step;
    int max_depth;
};

enum State handle_start(struct Context *ctx);
enum State handle_processing(struct Context *ctx);
enum State handle_validating(struct Context *ctx);
enum State handle_finished(struct Context *ctx);
enum State handle_error(struct Context *ctx);

enum State transition(struct Context *ctx, enum State next_state) {
    if (ctx == NULL) return ERROR;
    if (next_state < START || next_state > ERROR) return ERROR;
    ctx->current_state = next_state;
    return next_state;
}

enum State process_state(struct Context *ctx) {
    if (ctx == NULL) return ERROR;
    if (ctx->step >= ctx->max_depth) return ERROR;
    
    ctx->step++;
    
    switch (ctx->current_state) {
        case START:
            return handle_start(ctx);
        case PROCESSING:
            return handle_processing(ctx);
        case VALIDATING:
            return handle_validating(ctx);
        case FINISHED:
            return handle_finished(ctx);
        case ERROR:
            return handle_error(ctx);
        default:
            return ERROR;
    }
}

enum State handle_start(struct Context *ctx) {
    printf("State: START - Initializing data\n");
    ctx->data = 0;
    return transition(ctx, PROCESSING);
}

enum State handle_processing(struct Context *ctx) {
    if (ctx->data < 0 || ctx->data > 1000) {
        return transition(ctx, ERROR);
    }
    
    printf("State: PROCESSING - Data value: %d\n", ctx->data);
    ctx->data += 100;
    
    if (ctx->data > 500) {
        return transition(ctx, VALIDATING);
    }
    
    return process_state(ctx);
}

enum State handle_validating(struct Context *ctx) {
    printf("State: VALIDATING - Checking data: %d\n", ctx->data);
    
    if (ctx->data >= 600 && ctx->data <= 800) {
        return transition(ctx, FINISHED);
    } else if (ctx->data > 800) {
        ctx->data -= 200;
        return process_state(ctx);
    } else {
        return transition(ctx, ERROR);
    }
}

enum State handle_finished(struct Context *ctx) {
    printf("State: FINISHED - Final data value: %d\n", ctx->data);
    return FINISHED;
}

enum State handle_error(struct Context *ctx) {
    printf("State: ERROR - Invalid data: %d\n", ctx->data);
    return ERROR;
}

int main(void) {
    struct Context ctx;
    ctx.current_state = START;
    ctx.data = 0;
    ctx.step = 0;
    ctx.max_depth = 20;
    
    enum State final_state = process_state(&ctx);
    
    if (final_state == FINISHED) {
        printf("State machine completed successfully\n");
        return EXIT_SUCCESS;
    } else {
        printf("State machine terminated with error\n");
        return EXIT_FAILURE;
    }
}