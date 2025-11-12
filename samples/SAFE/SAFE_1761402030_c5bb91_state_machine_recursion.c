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
    
    if (ctx->data < 0 || ctx->data > 200) {
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
    printf("Already finished. Input %d ignored.\n", input);
    return FINISHED;
}

enum State handle_error(struct Context *ctx, int input) {
    if (input == -1) {
        printf("Recovering from error state\n");
        ctx->data = 0;
        ctx->step_count = 0;
        return START;
    }
    printf("In error state. Input %d ignored.\n", input);
    return ERROR;
}

void run_state_machine(struct Context *ctx, int *inputs, int count) {
    if (ctx == NULL || inputs == NULL || count <= 0 || count > 50) return;
    
    for (int i = 0; i < count; i++) {
        ctx->current_state = transition(ctx, inputs[i]);
        if (ctx->current_state == ERROR) {
            printf("Error detected at step %d\n", i);
            break;
        }
        if (ctx->current_state == FINISHED) {
            printf("State machine completed successfully\n");
            break;
        }
    }
}

int main() {
    struct Context ctx = {START, 0, 0};
    
    int test_inputs[] = {5, 3, 2, -1, 4, 0};
    int input_count = sizeof(test_inputs) / sizeof(test_inputs[0]);
    
    run_state_machine(&ctx, test_inputs, input_count);
    
    printf("Final state: %d, Final value: %d\n", ctx.current_state, ctx.data);
    
    return 0;
}