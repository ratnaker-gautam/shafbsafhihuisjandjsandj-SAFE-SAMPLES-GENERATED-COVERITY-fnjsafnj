//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State { START, PROCESSING, VALIDATING, COMPLETED, ERROR };

struct Context {
    enum State current_state;
    int data_value;
    int step_count;
    char buffer[32];
};

enum State handle_start(struct Context *ctx, int input);
enum State handle_processing(struct Context *ctx, int input);
enum State handle_validating(struct Context *ctx, int input);
enum State handle_completed(struct Context *ctx, int input);
enum State handle_error(struct Context *ctx, int input);

enum State transition_state(struct Context *ctx, int input) {
    switch (ctx->current_state) {
        case START:
            return handle_start(ctx, input);
        case PROCESSING:
            return handle_processing(ctx, input);
        case VALIDATING:
            return handle_validating(ctx, input);
        case COMPLETED:
            return handle_completed(ctx, input);
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
    ctx->data_value = input;
    ctx->step_count = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    return PROCESSING;
}

enum State handle_processing(struct Context *ctx, int input) {
    if (input < 0 || input > 50) {
        return ERROR;
    }
    ctx->data_value += input;
    ctx->step_count++;
    if (ctx->step_count >= 3) {
        return VALIDATING;
    }
    return PROCESSING;
}

enum State handle_validating(struct Context *ctx, int input) {
    if (input != 1 && input != 0) {
        return ERROR;
    }
    if (input == 1) {
        if (ctx->data_value > 200) {
            return ERROR;
        }
        int len = snprintf(ctx->buffer, sizeof(ctx->buffer), "Result: %d", ctx->data_value);
        if (len < 0 || len >= (int)sizeof(ctx->buffer)) {
            return ERROR;
        }
        return COMPLETED;
    } else {
        ctx->data_value = 0;
        ctx->step_count = 0;
        return START;
    }
}

enum State handle_completed(struct Context *ctx, int input) {
    if (input == 0) {
        return START;
    }
    return COMPLETED;
}

enum State handle_error(struct Context *ctx, int input) {
    if (input == 0) {
        return START;
    }
    return ERROR;
}

void process_state_machine(struct Context *ctx, int input) {
    enum State new_state = transition_state(ctx, input);
    ctx->current_state = new_state;
}

void recursive_state_processor(struct Context *ctx, int remaining_inputs, int *inputs) {
    if (remaining_inputs <= 0) {
        return;
    }
    if (ctx->current_state == ERROR || ctx->current_state == COMPLETED) {
        return;
    }
    int current_input = inputs[0];
    process_state_machine(ctx, current_input);
    recursive_state_processor(ctx, remaining_inputs - 1, inputs + 1);
}

int main(void) {
    struct Context ctx;
    ctx.current_state = START;
    ctx.data_value = 0;
    ctx.step_count = 0;
    memset(ctx.buffer, 0, sizeof(ctx.buffer));
    
    int num_inputs;
    printf("Enter number of inputs (1-10): ");
    if (scanf("%d", &num_inputs) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    if (num_inputs < 1 || num_inputs > 10) {
        fprintf(stderr, "Invalid number of inputs\n");
        return 1;
    }
    
    int inputs[10];
    printf("Enter %d integers:\n", num_inputs);
    for (int i = 0; i < num_inputs; i++) {
        if (scanf("%d", &inputs[i]) != 1) {
            fprintf(stderr, "Invalid input\n");
            return 1;
        }
    }
    
    recursive_state_processor(&ctx, num_inputs, inputs);
    
    switch (ctx.current_state) {
        case COMPLETED:
            printf("%s\n", ctx.buffer);
            break;
        case ERROR:
            printf("State machine ended in error state\n");
            break;
        case START:
            printf("State machine reset to start\n");
            break;
        default:
            printf("State machine interrupted\n");
            break;
    }
    
    return 0;
}