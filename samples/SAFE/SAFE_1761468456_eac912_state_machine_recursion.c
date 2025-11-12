//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State { START, PROCESSING, VALIDATING, FINISHED, ERROR };

struct Context {
    enum State current_state;
    int data[10];
    int count;
    int sum;
};

enum State handle_start(struct Context *ctx, int input);
enum State handle_processing(struct Context *ctx, int input);
enum State handle_validating(struct Context *ctx, int input);
enum State handle_finished(struct Context *ctx, int input);
enum State handle_error(struct Context *ctx, int input);

enum State transition(struct Context *ctx, int input) {
    switch (ctx->current_state) {
        case START:
            return handle_start(ctx, input);
        case PROCESSING:
            return handle_processing(ctx, input);
        case VALIDATING:
            return handle_validating(ctx, input);
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
    ctx->count = 0;
    ctx->sum = 0;
    ctx->data[ctx->count++] = input;
    return PROCESSING;
}

enum State handle_processing(struct Context *ctx, int input) {
    if (input < 0 || input > 100) {
        return ERROR;
    }
    if (ctx->count >= 10) {
        return VALIDATING;
    }
    ctx->data[ctx->count++] = input;
    return PROCESSING;
}

enum State handle_validating(struct Context *ctx, int input) {
    if (input != 0) {
        return ERROR;
    }
    for (int i = 0; i < ctx->count; i++) {
        ctx->sum += ctx->data[i];
    }
    if (ctx->sum > 500) {
        return ERROR;
    }
    return FINISHED;
}

enum State handle_finished(struct Context *ctx, int input) {
    if (input == 0) {
        return FINISHED;
    }
    return ERROR;
}

enum State handle_error(struct Context *ctx, int input) {
    return ERROR;
}

void process_state_machine(struct Context *ctx, int input) {
    enum State next_state = transition(ctx, input);
    ctx->current_state = next_state;
}

void recursive_process(struct Context *ctx, int remaining_inputs, int *inputs) {
    if (remaining_inputs <= 0) {
        return;
    }
    if (ctx->current_state == ERROR || ctx->current_state == FINISHED) {
        return;
    }
    process_state_machine(ctx, inputs[0]);
    recursive_process(ctx, remaining_inputs - 1, inputs + 1);
}

int main(void) {
    struct Context ctx;
    ctx.current_state = START;
    
    int inputs[20];
    int input_count = 0;
    
    printf("Enter up to 20 integers (0-100), end with -1:\n");
    
    while (input_count < 20) {
        int value;
        if (scanf("%d", &value) != 1) {
            break;
        }
        if (value == -1) {
            break;
        }
        if (value < -1 || value > 100) {
            printf("Invalid input: %d\n", value);
            return 1;
        }
        inputs[input_count++] = value;
    }
    
    if (input_count == 0) {
        printf("No inputs provided\n");
        return 1;
    }
    
    recursive_process(&ctx, input_count, inputs);
    
    switch (ctx.current_state) {
        case FINISHED:
            printf("Processing completed successfully. Sum: %d\n", ctx.sum);
            break;
        case ERROR:
            printf("Processing failed with error\n");
            break;
        default:
            printf("Processing incomplete\n");
            break;
    }
    
    return 0;
}