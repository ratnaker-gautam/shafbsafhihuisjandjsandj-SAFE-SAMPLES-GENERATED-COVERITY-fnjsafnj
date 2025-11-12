//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State { START, PROCESSING, VALIDATING, FINISHED, ERROR };

struct Context {
    enum State current_state;
    int data;
    int step_count;
    int max_steps;
};

enum State handle_start(struct Context *ctx) {
    if (ctx->step_count >= ctx->max_steps) return ERROR;
    printf("Starting process with data: %d\n", ctx->data);
    ctx->step_count++;
    return PROCESSING;
}

enum State handle_processing(struct Context *ctx) {
    if (ctx->step_count >= ctx->max_steps) return ERROR;
    if (ctx->data < 0) return ERROR;
    ctx->data *= 2;
    printf("Processing data: %d\n", ctx->data);
    ctx->step_count++;
    return VALIDATING;
}

enum State handle_validating(struct Context *ctx) {
    if (ctx->step_count >= ctx->max_steps) return ERROR;
    if (ctx->data > 1000) {
        printf("Data too large: %d\n", ctx->data);
        return ERROR;
    }
    if (ctx->data < 0) return ERROR;
    printf("Validating data: %d\n", ctx->data);
    ctx->step_count++;
    if (ctx->data >= 100) return FINISHED;
    return PROCESSING;
}

enum State handle_finished(struct Context *ctx) {
    printf("Process finished successfully. Final data: %d\n", ctx->data);
    return FINISHED;
}

enum State handle_error(struct Context *ctx) {
    printf("Process encountered an error at step %d\n", ctx->step_count);
    return ERROR;
}

void state_machine(struct Context *ctx) {
    if (ctx->step_count >= ctx->max_steps) {
        ctx->current_state = ERROR;
        return;
    }

    switch (ctx->current_state) {
        case START:
            ctx->current_state = handle_start(ctx);
            break;
        case PROCESSING:
            ctx->current_state = handle_processing(ctx);
            break;
        case VALIDATING:
            ctx->current_state = handle_validating(ctx);
            break;
        case FINISHED:
            ctx->current_state = handle_finished(ctx);
            return;
        case ERROR:
            ctx->current_state = handle_error(ctx);
            return;
        default:
            ctx->current_state = ERROR;
            return;
    }

    if (ctx->current_state != FINISHED && ctx->current_state != ERROR) {
        state_machine(ctx);
    }
}

int main() {
    struct Context ctx;
    ctx.current_state = START;
    ctx.step_count = 0;
    ctx.max_steps = 20;

    printf("Enter initial data value (0-500): ");
    if (scanf("%d", &ctx.data) != 1) {
        printf("Invalid input\n");
        return 1;
    }

    if (ctx.data < 0 || ctx.data > 500) {
        printf("Input out of range\n");
        return 1;
    }

    state_machine(&ctx);

    if (ctx.current_state == FINISHED) {
        printf("State machine completed successfully\n");
        return 0;
    } else {
        printf("State machine failed\n");
        return 1;
    }
}