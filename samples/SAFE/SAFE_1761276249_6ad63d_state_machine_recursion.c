//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: state_machine
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
    if (ctx->count >= 10) {
        return ERROR;
    }
    ctx->data[ctx->count] = input;
    ctx->count++;
    return READING;
}

enum State handle_processing(struct Context *ctx, int input) {
    if (input != 0) {
        return ERROR;
    }
    ctx->sum = 0;
    for (int i = 0; i < ctx->count; i++) {
        if (ctx->data[i] > 0 && ctx->sum > INT_MAX - ctx->data[i]) {
            return ERROR;
        }
        ctx->sum += ctx->data[i];
    }
    return FINISHED;
}

enum State handle_finished(struct Context *ctx, int input) {
    if (input == -1) {
        printf("Sum: %d\n", ctx->sum);
        return FINISHED;
    }
    return ERROR;
}

enum State handle_error(struct Context *ctx, int input) {
    printf("Invalid state transition\n");
    return ERROR;
}

void run_state_machine(struct Context *ctx, int *inputs, int input_count, int depth) {
    if (depth >= input_count) {
        return;
    }
    if (depth < 0 || input_count < 0 || depth > 1000) {
        ctx->current_state = ERROR;
        return;
    }
    ctx->current_state = transition(ctx, inputs[depth]);
    run_state_machine(ctx, inputs, input_count, depth + 1);
}

int main(void) {
    struct Context ctx = {START, {0}, 0, 0};
    int inputs[20];
    int input_count = 0;
    char buffer[32];
    
    printf("Enter inputs (one per line, -2 to end):\n");
    while (input_count < 20) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }
        buffer[31] = '\0';
        char *endptr;
        long val = strtol(buffer, &endptr, 10);
        if (endptr == buffer || *endptr != '\n') {
            break;
        }
        if (val < -2 || val > 1000) {
            break;
        }
        if (val == -2) {
            break;
        }
        if (val < INT_MIN || val > INT_MAX) {
            break;
        }
        inputs[input_count] = (int)val;
        input_count++;
    }
    
    if (input_count == 0) {
        return 1;
    }
    
    run_state_machine(&ctx, inputs, input_count, 0);
    
    if (ctx.current_state != FINISHED) {
        printf("State machine ended in error state\n");
        return 1;
    }
    
    return 0;
}