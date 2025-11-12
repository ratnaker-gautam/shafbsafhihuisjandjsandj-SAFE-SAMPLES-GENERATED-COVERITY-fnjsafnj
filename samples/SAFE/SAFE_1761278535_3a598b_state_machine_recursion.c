//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State { START, READING, PROCESSING, FINISHED, ERROR };

struct Context {
    enum State current_state;
    int data[10];
    int count;
    int sum;
};

enum State handle_start(struct Context *ctx, int input) {
    if (input == 1) {
        ctx->count = 0;
        ctx->sum = 0;
        return READING;
    }
    return ERROR;
}

enum State handle_reading(struct Context *ctx, int input) {
    if (input >= 0 && input <= 100) {
        if (ctx->count < 10) {
            ctx->data[ctx->count] = input;
            ctx->count++;
            if (ctx->count == 10) {
                return PROCESSING;
            }
            return READING;
        }
    }
    return ERROR;
}

enum State handle_processing(struct Context *ctx, int input) {
    if (input == 0) {
        for (int i = 0; i < ctx->count; i++) {
            ctx->sum += ctx->data[i];
        }
        return FINISHED;
    }
    return ERROR;
}

enum State handle_finished(struct Context *ctx, int input) {
    if (input == -1) {
        return START;
    }
    return FINISHED;
}

enum State handle_error(struct Context *ctx, int input) {
    if (input == -1) {
        return START;
    }
    return ERROR;
}

void state_machine(struct Context *ctx, int input) {
    switch (ctx->current_state) {
        case START:
            ctx->current_state = handle_start(ctx, input);
            break;
        case READING:
            ctx->current_state = handle_reading(ctx, input);
            break;
        case PROCESSING:
            ctx->current_state = handle_processing(ctx, input);
            break;
        case FINISHED:
            ctx->current_state = handle_finished(ctx, input);
            break;
        case ERROR:
            ctx->current_state = handle_error(ctx, input);
            break;
    }
}

void process_inputs(struct Context *ctx, int *inputs, int count, int index) {
    if (index >= count) {
        return;
    }
    
    state_machine(ctx, inputs[index]);
    
    if (ctx->current_state == FINISHED) {
        printf("Processing complete. Sum: %d\n", ctx->sum);
        return;
    } else if (ctx->current_state == ERROR) {
        printf("Error state reached. Resetting with -1.\n");
        return;
    }
    
    process_inputs(ctx, inputs, count, index + 1);
}

int main(void) {
    struct Context ctx = {START, {0}, 0, 0};
    
    int test_inputs[] = {1, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 0, -1};
    int input_count = sizeof(test_inputs) / sizeof(test_inputs[0]);
    
    printf("Starting state machine test...\n");
    process_inputs(&ctx, test_inputs, input_count, 0);
    
    if (ctx.current_state == FINISHED) {
        printf("Final sum: %d\n", ctx.sum);
    } else {
        printf("Unexpected final state: %d\n", ctx.current_state);
    }
    
    return 0;
}