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

enum State handle_start(struct Context *ctx, int input);
enum State handle_reading(struct Context *ctx, int input);
enum State handle_processing(struct Context *ctx, int input);
enum State handle_finished(struct Context *ctx, int input);
enum State handle_error(struct Context *ctx, int input);

enum State transition(struct Context *ctx, int input) {
    switch (ctx->current_state) {
        case START: return handle_start(ctx, input);
        case READING: return handle_reading(ctx, input);
        case PROCESSING: return handle_processing(ctx, input);
        case FINISHED: return handle_finished(ctx, input);
        case ERROR: return handle_error(ctx, input);
        default: return ERROR;
    }
}

enum State handle_start(struct Context *ctx, int input) {
    if (input == 1) {
        ctx->count = 0;
        ctx->sum = 0;
        return READING;
    }
    return ERROR;
}

enum State handle_reading(struct Context *ctx, int input) {
    if (input == 0) {
        if (ctx->count > 0) {
            return PROCESSING;
        }
        return ERROR;
    }
    if (input > 0 && input < 100) {
        if (ctx->count < 10) {
            ctx->data[ctx->count] = input;
            ctx->count++;
            return READING;
        }
    }
    return ERROR;
}

enum State handle_processing(struct Context *ctx, int input) {
    if (input == 1) {
        ctx->sum = 0;
        for (int i = 0; i < ctx->count; i++) {
            if (ctx->sum > 1000 - ctx->data[i]) {
                return ERROR;
            }
            ctx->sum += ctx->data[i];
        }
        return FINISHED;
    }
    return ERROR;
}

enum State handle_finished(struct Context *ctx, int input) {
    if (input == 1) {
        printf("Sum: %d\n", ctx->sum);
        return FINISHED;
    }
    if (input == 0) {
        return START;
    }
    return ERROR;
}

enum State handle_error(struct Context *ctx, int input) {
    if (input == 0) {
        return START;
    }
    return ERROR;
}

void run_state_machine(struct Context *ctx, int input) {
    enum State next_state = transition(ctx, input);
    if (next_state != ctx->current_state) {
        ctx->current_state = next_state;
        if (ctx->current_state == ERROR) {
            printf("State machine error occurred\n");
        }
    }
}

void process_inputs(struct Context *ctx, int *inputs, int count) {
    if (count <= 0 || inputs == NULL) {
        return;
    }
    for (int i = 0; i < count; i++) {
        run_state_machine(ctx, inputs[i]);
    }
}

int main() {
    struct Context ctx;
    ctx.current_state = START;
    
    int test_inputs[] = {1, 5, 10, 15, 0, 1, 1, 0};
    int input_count = sizeof(test_inputs) / sizeof(test_inputs[0]);
    
    process_inputs(&ctx, test_inputs, input_count);
    
    return 0;
}