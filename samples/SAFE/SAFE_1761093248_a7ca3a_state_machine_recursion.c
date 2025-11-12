//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: state_machine
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

enum State handle_start(struct Context* ctx) {
    if (ctx->step_count >= ctx->max_steps) return ERROR;
    printf("Starting process...\n");
    ctx->data = 0;
    ctx->step_count++;
    return PROCESSING;
}

enum State handle_processing(struct Context* ctx) {
    if (ctx->step_count >= ctx->max_steps) return ERROR;
    printf("Processing data...\n");
    ctx->data += 10;
    ctx->step_count++;
    if (ctx->data > 100) return ERROR;
    return VALIDATING;
}

enum State handle_validating(struct Context* ctx) {
    if (ctx->step_count >= ctx->max_steps) return ERROR;
    printf("Validating data: %d\n", ctx->data);
    ctx->step_count++;
    if (ctx->data % 20 == 0) return FINISHED;
    return PROCESSING;
}

enum State handle_finished(struct Context* ctx) {
    printf("Process completed successfully. Final data: %d\n", ctx->data);
    return FINISHED;
}

enum State handle_error(struct Context* ctx) {
    printf("Error occurred at step %d. Data: %d\n", ctx->step_count, ctx->data);
    return ERROR;
}

enum State process_state(struct Context* ctx) {
    if (ctx == NULL) return ERROR;
    if (ctx->step_count < 0 || ctx->max_steps <= 0) return ERROR;
    
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

void run_state_machine(struct Context* ctx) {
    if (ctx == NULL) return;
    
    enum State next_state = process_state(ctx);
    if (next_state == ctx->current_state && next_state != FINISHED && next_state != ERROR) {
        return;
    }
    
    ctx->current_state = next_state;
    
    if (ctx->current_state != FINISHED && ctx->current_state != ERROR) {
        run_state_machine(ctx);
    }
}

int main() {
    struct Context ctx;
    ctx.current_state = START;
    ctx.step_count = 0;
    ctx.max_steps = 50;
    
    run_state_machine(&ctx);
    
    if (ctx.current_state == FINISHED) {
        printf("State machine completed successfully.\n");
        return 0;
    } else {
        printf("State machine terminated with error.\n");
        return 1;
    }
}