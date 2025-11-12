//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State { START, PROCESSING, FINISHED, ERROR };

struct Context {
    enum State current_state;
    int data;
    int step_count;
};

enum State handle_start(struct Context* ctx, int input);
enum State handle_processing(struct Context* ctx, int input);
enum State handle_finished(struct Context* ctx, int input);
enum State handle_error(struct Context* ctx, int input);

enum State process_state(struct Context* ctx, int input) {
    if (ctx == NULL) return ERROR;
    
    switch (ctx->current_state) {
        case START:
            return handle_start(ctx, input);
        case PROCESSING:
            return handle_processing(ctx, input);
        case FINISHED:
            return handle_finished(ctx, input);
        case ERROR:
            return handle_error(ctx, input);
        default:
            return ERROR;
    }
}

enum State handle_start(struct Context* ctx, int input) {
    if (input < 0 || input > 100) {
        return ERROR;
    }
    
    ctx->data = input;
    ctx->step_count = 1;
    printf("State: START -> PROCESSING (data: %d)\n", input);
    return PROCESSING;
}

enum State handle_processing(struct Context* ctx, int input) {
    if (ctx->step_count >= 10) {
        printf("State: PROCESSING -> FINISHED (max steps reached)\n");
        return FINISHED;
    }
    
    if (input < -50 || input > 50) {
        printf("State: PROCESSING -> ERROR (invalid input: %d)\n", input);
        return ERROR;
    }
    
    if (ctx->data > 0 && input < 0) {
        printf("State: PROCESSING -> ERROR (negative input on positive data)\n");
        return ERROR;
    }
    
    int new_data;
    if (__builtin_add_overflow(ctx->data, input, &new_data)) {
        printf("State: PROCESSING -> ERROR (integer overflow)\n");
        return ERROR;
    }
    
    ctx->data = new_data;
    ctx->step_count++;
    
    printf("State: PROCESSING -> PROCESSING (data: %d, step: %d)\n", 
           ctx->data, ctx->step_count);
    
    if (ctx->data >= 200) {
        printf("State: PROCESSING -> FINISHED (target reached)\n");
        return FINISHED;
    }
    
    return PROCESSING;
}

enum State handle_finished(struct Context* ctx, int input) {
    printf("State: FINISHED (final data: %d, total steps: %d)\n", 
           ctx->data, ctx->step_count);
    return FINISHED;
}

enum State handle_error(struct Context* ctx, int input) {
    printf("State: ERROR (data: %d, step: %d)\n", ctx->data, ctx->step_count);
    return ERROR;
}

void run_state_machine(struct Context* ctx, int* inputs, int input_count, int current_index) {
    if (ctx == NULL || inputs == NULL || current_index >= input_count) {
        return;
    }
    
    if (ctx->current_state == FINISHED || ctx->current_state == ERROR) {
        return;
    }
    
    enum State next_state = process_state(ctx, inputs[current_index]);
    ctx->current_state = next_state;
    
    run_state_machine(ctx, inputs, input_count, current_index + 1);
}

int main() {
    struct Context ctx;
    ctx.current_state = START;
    ctx.data = 0;
    ctx.step_count = 0;
    
    int inputs[15];
    int input_count = 0;
    
    printf("Enter up to 15 integers (non-integer to stop):\n");
    
    for (int i = 0; i < 15; i++) {
        int value;
        if (scanf("%d", &value) != 1) {
            break;
        }
        inputs[input_count++] = value;
    }
    
    if (input_count == 0) {
        printf("No valid inputs provided\n");
        return 1;
    }
    
    printf("\nStarting state machine...\n");
    run_state_machine(&ctx, inputs, input_count, 0);
    
    return 0;
}