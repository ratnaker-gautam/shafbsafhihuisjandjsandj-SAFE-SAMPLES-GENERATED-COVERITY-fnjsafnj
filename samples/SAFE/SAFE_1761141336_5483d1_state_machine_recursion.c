//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

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

enum State transition(struct Context* ctx, int input) {
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
    
    if (input == 0) {
        printf("State: PROCESSING -> FINISHED (received termination)\n");
        return FINISHED;
    }
    
    if (input < -50 || input > 50) {
        printf("State: PROCESSING -> ERROR (invalid processing input)\n");
        return ERROR;
    }
    
    if (input > 0) {
        if (ctx->data > INT_MAX - input) {
            printf("State: PROCESSING -> ERROR (overflow)\n");
            return ERROR;
        }
        ctx->data += input;
    } else {
        if (ctx->data < INT_MIN - input) {
            printf("State: PROCESSING -> ERROR (underflow)\n");
            return ERROR;
        }
        ctx->data += input;
    }
    
    ctx->step_count++;
    printf("State: PROCESSING -> PROCESSING (data: %d, step: %d)\n", 
           ctx->data, ctx->step_count);
    return PROCESSING;
}

enum State handle_finished(struct Context* ctx, int input) {
    printf("State: FINISHED (final data: %d, total steps: %d)\n", 
           ctx->data, ctx->step_count);
    return FINISHED;
}

enum State handle_error(struct Context* ctx, int input) {
    printf("State: ERROR (data: %d, step: %d)\n", 
           ctx->data, ctx->step_count);
    return ERROR;
}

void state_machine_recursive(struct Context* ctx, int remaining_inputs, int* inputs) {
    if (ctx == NULL || inputs == NULL || remaining_inputs < 0) {
        return;
    }
    
    if (remaining_inputs == 0) {
        return;
    }
    
    int current_input = inputs[0];
    enum State next_state = transition(ctx, current_input);
    ctx->current_state = next_state;
    
    if (next_state == FINISHED || next_state == ERROR) {
        return;
    }
    
    state_machine_recursive(ctx, remaining_inputs - 1, inputs + 1);
}

int main() {
    struct Context ctx = {START, 0, 0};
    
    int test_inputs[] = {42, 10, -5, 8, -3, 0};
    int input_count = sizeof(test_inputs) / sizeof(test_inputs[0]);
    
    printf("Starting state machine with %d inputs\n", input_count);
    state_machine_recursive(&ctx, input_count, test_inputs);
    
    printf("Final state: ");
    switch (ctx.current_state) {
        case START: printf("START\n"); break;
        case PROCESSING: printf("PROCESSING\n"); break;
        case FINISHED: printf("FINISHED\n"); break;
        case ERROR: printf("ERROR\n"); break;
    }
    
    return 0;
}