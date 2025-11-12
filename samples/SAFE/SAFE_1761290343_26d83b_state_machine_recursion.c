//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State {
    START,
    READING,
    PROCESSING,
    FINISHED,
    ERROR
};

struct Context {
    enum State current_state;
    int data[10];
    int count;
    int sum;
};

enum State handle_start(struct Context *ctx, int input) {
    if (input < 0 || input > 100) {
        return ERROR;
    }
    ctx->count = 0;
    ctx->sum = 0;
    return READING;
}

enum State handle_reading(struct Context *ctx, int input) {
    if (input < 0 || input > 100) {
        return ERROR;
    }
    if (ctx->count >= 10) {
        return PROCESSING;
    }
    ctx->data[ctx->count] = input;
    ctx->count++;
    if (ctx->count == 10) {
        return PROCESSING;
    }
    return READING;
}

enum State handle_processing(struct Context *ctx) {
    if (ctx->count <= 0) {
        return ERROR;
    }
    for (int i = 0; i < ctx->count; i++) {
        ctx->sum += ctx->data[i];
    }
    return FINISHED;
}

enum State handle_error(struct Context *ctx) {
    ctx->count = 0;
    ctx->sum = 0;
    return START;
}

enum State process_state(struct Context *ctx, int input) {
    switch (ctx->current_state) {
        case START:
            return handle_start(ctx, input);
        case READING:
            return handle_reading(ctx, input);
        case PROCESSING:
            return handle_processing(ctx);
        case FINISHED:
            return FINISHED;
        case ERROR:
            return handle_error(ctx);
        default:
            return ERROR;
    }
}

void run_state_machine(struct Context *ctx, int input) {
    enum State next_state = process_state(ctx, input);
    ctx->current_state = next_state;
}

void recursive_process(struct Context *ctx, int remaining_inputs) {
    if (remaining_inputs <= 0) {
        return;
    }
    
    int input;
    if (scanf("%d", &input) != 1) {
        ctx->current_state = ERROR;
        return;
    }
    
    run_state_machine(ctx, input);
    
    if (ctx->current_state == FINISHED) {
        printf("Processing complete. Sum: %d\n", ctx->sum);
        return;
    }
    
    if (ctx->current_state == ERROR) {
        printf("Error occurred. Resetting state machine.\n");
        ctx->current_state = START;
    }
    
    recursive_process(ctx, remaining_inputs - 1);
}

int main(void) {
    struct Context ctx;
    ctx.current_state = START;
    ctx.count = 0;
    ctx.sum = 0;
    
    printf("Enter up to 10 integers between 0 and 100:\n");
    printf("State machine will process inputs recursively.\n");
    
    recursive_process(&ctx, 10);
    
    if (ctx.current_state != FINISHED && ctx.current_state != ERROR) {
        printf("Incomplete processing. Final state: %d\n", ctx.current_state);
    }
    
    return 0;
}