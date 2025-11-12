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

enum State handle_processing(struct Context *ctx, int input) {
    if (input != 0) {
        return ERROR;
    }
    ctx->sum = 0;
    for (int i = 0; i < ctx->count; i++) {
        ctx->sum += ctx->data[i];
    }
    return FINISHED;
}

enum State handle_finished(struct Context *ctx, int input) {
    return FINISHED;
}

enum State handle_error(struct Context *ctx, int input) {
    return ERROR;
}

typedef enum State (*StateHandler)(struct Context *, int);

StateHandler get_handler(enum State state) {
    switch (state) {
        case START: return handle_start;
        case READING: return handle_reading;
        case PROCESSING: return handle_processing;
        case FINISHED: return handle_finished;
        case ERROR: return handle_error;
        default: return handle_error;
    }
}

void process_state_machine(struct Context *ctx, int input) {
    StateHandler handler = get_handler(ctx->current_state);
    enum State next_state = handler(ctx, input);
    ctx->current_state = next_state;
}

void recursive_process(struct Context *ctx, int remaining_inputs) {
    if (remaining_inputs <= 0) {
        return;
    }
    
    int input;
    printf("Enter a number (0-100): ");
    if (scanf("%d", &input) != 1) {
        printf("Invalid input\n");
        exit(1);
    }
    
    process_state_machine(ctx, input);
    
    if (ctx->current_state == FINISHED) {
        printf("Processing complete. Sum: %d\n", ctx->sum);
        return;
    } else if (ctx->current_state == ERROR) {
        printf("Error state reached\n");
        return;
    }
    
    recursive_process(ctx, remaining_inputs - 1);
}

int main(void) {
    struct Context ctx;
    ctx.current_state = START;
    
    printf("State Machine Demo\n");
    printf("Enter up to 10 numbers (0-100), then enter 0 to process\n");
    
    recursive_process(&ctx, 11);
    
    if (ctx.current_state != FINISHED && ctx.current_state != ERROR) {
        printf("Unexpected termination\n");
    }
    
    return 0;
}