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
    
    if (__builtin_add_overflow(ctx->data, input, &ctx->data)) {
        return ERROR;
    }
    
    printf("Processing: added %d, current value: %d\n", input, ctx->data);
    
    if (ctx->data > 200 || ctx->data < -200) {
        return ERROR;
    }
    
    return PROCESSING;
}

enum State handle_finished(struct Context *ctx, int input) {
    if (input == -1) {
        printf("Resetting state machine\n");
        ctx->data = 0;
        ctx->step_count = 0;
        return START;
    }
    return FINISHED;
}

enum State handle_error(struct Context *ctx, int input) {
    if (input == -1) {
        printf("Recovering from error state\n");
        ctx->data = 0;
        ctx->step_count = 0;
        return START;
    }
    return ERROR;
}

void run_state_machine(struct Context *ctx, int max_steps) {
    if (ctx == NULL || max_steps <= 0 || max_steps > 50) return;
    
    if (ctx->step_count >= max_steps) {
        printf("Maximum steps reached\n");
        return;
    }
    
    int input;
    printf("Enter input for state %d: ", ctx->current_state);
    
    if (scanf("%d", &input) != 1) {
        printf("Invalid input\n");
        while (getchar() != '\n');
        ctx->current_state = ERROR;
    } else {
        ctx->current_state = transition(ctx, input);
    }
    
    if (ctx->current_state != FINISHED && ctx->current_state != ERROR) {
        run_state_machine(ctx, max_steps);
    }
}

int main(void) {
    struct Context ctx = {START, 0, 0};
    int max_iterations;
    
    printf("Enter maximum iterations (1-50): ");
    if (scanf("%d", &max_iterations) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (max_iterations < 1 || max_iterations > 50) {
        printf("Invalid range\n");
        return 1;
    }
    
    printf("State machine started. Enter integers to transition states.\n");
    printf("Special inputs: 0 to finish processing, -1 to reset\n");
    
    run_state_machine(&ctx, max_iterations);
    
    printf("Final state: %d, Final value: %d\n", ctx.current_state, ctx.data);
    
    return 0;
}