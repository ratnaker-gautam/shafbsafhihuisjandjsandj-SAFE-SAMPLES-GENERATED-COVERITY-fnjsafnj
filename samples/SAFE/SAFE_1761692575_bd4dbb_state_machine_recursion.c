//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State { START, PROCESSING, VALIDATING, FINISHED, ERROR };

struct Context {
    enum State current_state;
    int data;
    int step;
    char buffer[32];
};

enum State handle_start(struct Context* ctx) {
    if (ctx->step == 0) {
        printf("Enter initial value (0-100): ");
        if (scanf("%d", &ctx->data) != 1) {
            return ERROR;
        }
        if (ctx->data < 0 || ctx->data > 100) {
            return ERROR;
        }
        ctx->step = 1;
        return PROCESSING;
    }
    return ERROR;
}

enum State handle_processing(struct Context* ctx) {
    if (ctx->step == 1) {
        ctx->data *= 2;
        if (ctx->data > 200) {
            return ERROR;
        }
        ctx->step = 2;
        return VALIDATING;
    }
    return ERROR;
}

enum State handle_validating(struct Context* ctx) {
    if (ctx->step == 2) {
        if (ctx->data % 3 == 0) {
            ctx->step = 3;
            return FINISHED;
        } else {
            return ERROR;
        }
    }
    return ERROR;
}

enum State handle_finished(struct Context* ctx) {
    if (ctx->step == 3) {
        printf("Final result: %d\n", ctx->data);
        return FINISHED;
    }
    return ERROR;
}

enum State handle_error(struct Context* ctx) {
    printf("Error occurred in state machine\n");
    return ERROR;
}

enum State transition(struct Context* ctx) {
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

void run_state_machine(struct Context* ctx, int depth) {
    if (depth > 10) {
        ctx->current_state = ERROR;
        return;
    }
    
    enum State next_state = transition(ctx);
    if (next_state == ctx->current_state && next_state != FINISHED && next_state != ERROR) {
        return;
    }
    
    ctx->current_state = next_state;
    
    if (ctx->current_state != FINISHED && ctx->current_state != ERROR) {
        run_state_machine(ctx, depth + 1);
    }
}

int main(void) {
    struct Context ctx;
    memset(&ctx, 0, sizeof(ctx));
    ctx.current_state = START;
    ctx.step = 0;
    
    run_state_machine(&ctx, 0);
    
    if (ctx.current_state == ERROR) {
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}