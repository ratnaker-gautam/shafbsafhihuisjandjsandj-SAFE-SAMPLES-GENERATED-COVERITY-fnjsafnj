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

enum State process_state(struct Context *ctx, int input) {
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
    if (ctx->step >= 3) {
        printf("State: PROCESSING -> FINISHED (result=%d)\n", ctx->data);
        return FINISHED;
    }
    if (input > ctx->data) {
        ctx->data += input;
        if (ctx->data > 1000) {
            return ERROR;
        }
    } else {
        ctx->data -= input;
        if (ctx->data < 0) {
            return ERROR;
        }
    }
    ctx->step++;
    printf("State: PROCESSING (step=%d, data=%d)\n", ctx->step, ctx->data);
    return PROCESSING;
}

enum State handle_finished(struct Context *ctx, int input) {
    if (input != 0) {
        return ERROR;
    }
    printf("State: FINISHED (final=%d)\n", ctx->data);
    return FINISHED;
}

enum State handle_error(struct Context *ctx, int input) {
    printf("State: ERROR\n");
    return ERROR;
}

void state_machine_recursive(struct Context *ctx, int remaining_inputs, int *inputs) {
    if (remaining_inputs <= 0) {
        return;
    }
    if (ctx->current_state == ERROR || ctx->current_state == FINISHED) {
        return;
    }
    int input = inputs[0];
    ctx->current_state = process_state(ctx, input);
    state_machine_recursive(ctx, remaining_inputs - 1, inputs + 1);
}

int main(void) {
    int num_inputs;
    printf("Enter number of inputs (1-10): ");
    if (scanf("%d", &num_inputs) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    if (num_inputs < 1 || num_inputs > 10) {
        printf("Invalid number of inputs\n");
        return 1;
    }
    int *inputs = malloc(num_inputs * sizeof(int));
    if (inputs == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }
    printf("Enter %d integers (0-100): ", num_inputs);
    for (int i = 0; i < num_inputs; i++) {
        if (scanf("%d", &inputs[i]) != 1) {
            printf("Invalid input\n");
            free(inputs);
            return 1;
        }
        if (inputs[i] < 0 || inputs[i] > 100) {
            printf("Input out of range\n");
            free(inputs);
            return 1;
        }
    }
    struct Context ctx = { START, 0, 0 };
    state_machine_recursive(&ctx, num_inputs, inputs);
    free(inputs);
    return 0;
}