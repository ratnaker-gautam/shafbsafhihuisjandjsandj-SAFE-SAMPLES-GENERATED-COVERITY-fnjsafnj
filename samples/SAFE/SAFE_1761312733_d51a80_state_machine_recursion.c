//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State { START, PROCESSING, FINISHED, ERROR };

struct Context {
    enum State current_state;
    int data;
    int step_count;
};

enum State handle_start(struct Context *ctx, int input);
enum State handle_processing(struct Context *ctx, int input);
enum State handle_finished(struct Context *ctx, int input);
enum State handle_error(struct Context *ctx, int input);

enum State transition(struct Context *ctx, int input) {
    if (ctx == NULL) return ERROR;
    if (ctx->step_count >= 100) return ERROR;
    
    ctx->step_count++;
    
    switch (ctx->current_state) {
        case START: return handle_start(ctx, input);
        case PROCESSING: return handle_processing(ctx, input);
        case FINISHED: return handle_finished(ctx, input);
        case ERROR: return handle_error(ctx, input);
        default: return ERROR;
    }
}

enum State handle_start(struct Context *ctx, int input) {
    if (input < 0 || input > 100) return ERROR;
    
    ctx->data = input;
    printf("Starting with value: %d\n", input);
    return PROCESSING;
}

enum State handle_processing(struct Context *ctx, int input) {
    if (input == 0) {
        printf("Processing complete. Final value: %d\n", ctx->data);
        return FINISHED;
    }
    
    if (input < -10 || input > 10) return ERROR;
    
    if (__builtin_add_overflow(ctx->data, input, &ctx->data)) return ERROR;
    
    printf("Processing: %d (current: %d)\n", input, ctx->data);
    
    if (ctx->data < 0 || ctx->data > 200) return ERROR;
    
    return PROCESSING;
}

enum State handle_finished(struct Context *ctx, int input) {
    if (input != 0) return ERROR;
    printf("State machine finished successfully.\n");
    return FINISHED;
}

enum State handle_error(struct Context *ctx, int input) {
    printf("State machine in error state.\n");
    return ERROR;
}

void run_state_machine(struct Context *ctx, int max_steps) {
    if (ctx == NULL || max_steps <= 0 || max_steps > 100) return;
    
    if (ctx->current_state == FINISHED || ctx->current_state == ERROR) return;
    
    int input;
    printf("Enter input for state %d: ", ctx->current_state);
    
    if (scanf("%d", &input) != 1) {
        ctx->current_state = ERROR;
        return;
    }
    
    if (input < -1000 || input > 1000) {
        ctx->current_state = ERROR;
        return;
    }
    
    enum State new_state = transition(ctx, input);
    ctx->current_state = new_state;
    
    if (ctx->step_count < max_steps && new_state != FINISHED && new_state != ERROR) {
        run_state_machine(ctx, max_steps);
    }
}

int main(void) {
    struct Context ctx = {START, 0, 0};
    
    printf("State Machine Demo\n");
    printf("Enter integers to transition states:\n");
    printf("- Start: 0-100 to begin\n");
    printf("- Processing: -10 to 10 to modify value, 0 to finish\n");
    printf("- Finished: 0 to confirm\n");
    
    run_state_machine(&ctx, 20);
    
    if (ctx.current_state == FINISHED) {
        printf("Final result: %d\n", ctx.data);
    } else {
        printf("State machine terminated in state: %d\n", ctx.current_state);
    }
    
    return ctx.current_state == FINISHED ? 0 : 1;
}