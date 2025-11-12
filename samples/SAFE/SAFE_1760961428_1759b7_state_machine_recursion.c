//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

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
    if (ctx == NULL) {
        return ERROR;
    }
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
    if (ctx == NULL) {
        return ERROR;
    }
    if (input == 0) {
        ctx->count = 0;
        ctx->sum = 0;
        return READING;
    }
    return ERROR;
}

enum State handle_reading(struct Context *ctx, int input) {
    if (ctx == NULL) {
        return ERROR;
    }
    if (input == -1) {
        if (ctx->count > 0) {
            return PROCESSING;
        }
        return ERROR;
    }
    if (ctx->count >= 10) {
        return ERROR;
    }
    ctx->data[ctx->count] = input;
    ctx->count++;
    return READING;
}

enum State handle_processing(struct Context *ctx, int input) {
    if (ctx == NULL) {
        return ERROR;
    }
    if (input != 0) {
        return ERROR;
    }
    ctx->sum = 0;
    for (int i = 0; i < ctx->count; i++) {
        if (ctx->data[i] > 0 && ctx->sum > INT_MAX - ctx->data[i]) {
            return ERROR;
        }
        if (ctx->data[i] < 0 && ctx->sum < INT_MIN - ctx->data[i]) {
            return ERROR;
        }
        ctx->sum += ctx->data[i];
    }
    return FINISHED;
}

enum State handle_finished(struct Context *ctx, int input) {
    if (ctx == NULL) {
        return ERROR;
    }
    if (input == 0) {
        printf("Sum: %d\n", ctx->sum);
        return FINISHED;
    }
    return ERROR;
}

enum State handle_error(struct Context *ctx, int input) {
    printf("Invalid state transition\n");
    return ERROR;
}

void run_state_machine(struct Context *ctx, int *inputs, int input_count) {
    if (ctx == NULL || inputs == NULL || input_count <= 0) {
        return;
    }
    ctx->current_state = transition(ctx, inputs[0]);
    if (input_count > 1) {
        run_state_machine(ctx, inputs + 1, input_count - 1);
    }
}

int main(void) {
    int inputs[] = {0, 5, 10, 15, -1, 0, 0};
    int input_count = sizeof(inputs) / sizeof(inputs[0]);
    
    struct Context ctx;
    ctx.current_state = START;
    
    run_state_machine(&ctx, inputs, input_count);
    
    return 0;
}