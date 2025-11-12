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
    if (ctx->step >= 3) {
        printf("State: PROCESSING -> FINISHED (result=%d)\n", ctx->data);
        return FINISHED;
    }
    if (input > ctx->data) {
        ctx->data += input;
        if (ctx->data < 0) {
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
    if (input < -100 || input > 100) {
        ctx->current_state = ERROR;
        return;
    }
    ctx->current_state = transition(ctx, input);
    state_machine_recursive(ctx, remaining_inputs - 1, inputs + 1);
}

int main(void) {
    int inputs[10];
    int count = 0;
    printf("Enter up to 10 integers between -100 and 100 (non-integer to stop):\n");
    while (count < 10) {
        int result = scanf("%d", &inputs[count]);
        if (result != 1) {
            break;
        }
        if (inputs[count] < -100 || inputs[count] > 100) {
            printf("Input out of range. Stopping.\n");
            break;
        }
        count++;
    }
    if (count == 0) {
        printf("No valid inputs provided.\n");
        return 1;
    }
    struct Context ctx = { START, 0, 0 };
    state_machine_recursive(&ctx, count, inputs);
    return 0;
}