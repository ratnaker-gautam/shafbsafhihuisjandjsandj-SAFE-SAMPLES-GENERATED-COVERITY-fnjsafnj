//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State { START, PROCESSING, FINISHED, ERROR };

struct Context {
    enum State current_state;
    int data;
    int step;
};

enum State handle_start(struct Context *ctx, int input);
enum State handle_processing(struct Context *ctx, int input);
enum State handle_finished(struct Context *ctx, int input);
enum State handle_error(struct Context *ctx, int input);

enum State transition(struct Context *ctx, int input) {
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

enum State handle_start(struct Context *ctx, int input) {
    if (input < 0 || input > 100) {
        return ERROR;
    }
    ctx->data = input;
    ctx->step = 1;
    printf("State: START -> PROCESSING (data=%d)\n", ctx->data);
    return PROCESSING;
}

enum State handle_processing(struct Context *ctx, int input) {
    if (input < 0 || input > 100) {
        return ERROR;
    }
    if (ctx->step >= 5) {
        printf("State: PROCESSING -> FINISHED\n");
        return FINISHED;
    }
    if (input > ctx->data) {
        ctx->data = input;
    }
    ctx->step++;
    printf("State: PROCESSING (step=%d, max=%d)\n", ctx->step, ctx->data);
    return PROCESSING;
}

enum State handle_finished(struct Context *ctx, int input) {
    if (input != 0) {
        return ERROR;
    }
    printf("State: FINISHED -> START (reset)\n");
    return START;
}

enum State handle_error(struct Context *ctx, int input) {
    if (input == 0) {
        printf("State: ERROR -> START (reset)\n");
        return START;
    }
    printf("State: ERROR (awaiting reset)\n");
    return ERROR;
}

void state_machine_recursive(struct Context *ctx, int remaining_inputs) {
    if (remaining_inputs <= 0) {
        return;
    }
    
    int input;
    printf("Enter input (0-100, 0 to reset): ");
    if (scanf("%d", &input) != 1) {
        printf("Invalid input\n");
        while (getchar() != '\n');
        state_machine_recursive(ctx, remaining_inputs - 1);
        return;
    }
    
    if (input < 0 || input > 100) {
        printf("Input out of range\n");
        state_machine_recursive(ctx, remaining_inputs - 1);
        return;
    }
    
    enum State next_state = transition(ctx, input);
    ctx->current_state = next_state;
    
    state_machine_recursive(ctx, remaining_inputs - 1);
}

int main(void) {
    struct Context ctx;
    ctx.current_state = START;
    ctx.data = 0;
    ctx.step = 0;
    
    printf("State Machine Demo (5 steps max)\n");
    state_machine_recursive(&ctx, 10);
    
    printf("Final state: ");
    switch (ctx.current_state) {
        case START:
            printf("START\n");
            break;
        case PROCESSING:
            printf("PROCESSING\n");
            break;
        case FINISHED:
            printf("FINISHED (max value: %d)\n", ctx.data);
            break;
        case ERROR:
            printf("ERROR\n");
            break;
    }
    
    return 0;
}