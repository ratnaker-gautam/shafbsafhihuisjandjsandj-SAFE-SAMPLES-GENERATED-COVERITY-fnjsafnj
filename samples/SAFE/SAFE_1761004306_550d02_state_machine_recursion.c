//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: state_machine
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

enum State handle_start(struct Context* ctx, int input) {
    if (input == 1) {
        ctx->count = 0;
        ctx->sum = 0;
        return READING;
    }
    return ERROR;
}

enum State handle_reading(struct Context* ctx, int input) {
    if (input >= 0 && input <= 100) {
        if (ctx->count < 10) {
            ctx->data[ctx->count] = input;
            ctx->count++;
            return READING;
        } else {
            return PROCESSING;
        }
    }
    return ERROR;
}

enum State handle_processing(struct Context* ctx, int input) {
    if (input == 0) {
        for (int i = 0; i < ctx->count; i++) {
            ctx->sum += ctx->data[i];
        }
        return FINISHED;
    }
    return ERROR;
}

enum State handle_finished(struct Context* ctx, int input) {
    return FINISHED;
}

enum State handle_error(struct Context* ctx, int input) {
    return ERROR;
}

enum State process_input(struct Context* ctx, int input) {
    switch (ctx->current_state) {
        case START:
            return handle_start(ctx, input);
        case READING:
            return handle_reading(ctx, input);
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

void run_state_machine(struct Context* ctx, int iteration) {
    if (iteration >= 15 || ctx->current_state == FINISHED || ctx->current_state == ERROR) {
        if (ctx->current_state == FINISHED) {
            printf("Processing complete. Sum: %d\n", ctx->sum);
        } else if (ctx->current_state == ERROR) {
            printf("Error state reached.\n");
        }
        return;
    }

    printf("Enter input for state %d: ", ctx->current_state);
    int input;
    if (scanf("%d", &input) != 1) {
        printf("Invalid input.\n");
        while (getchar() != '\n');
        ctx->current_state = ERROR;
    } else {
        ctx->current_state = process_input(ctx, input);
    }

    run_state_machine(ctx, iteration + 1);
}

int main(void) {
    struct Context ctx;
    ctx.current_state = START;
    ctx.count = 0;
    ctx.sum = 0;

    printf("State Machine Started.\n");
    printf("Instructions:\n");
    printf("Start with 1 to begin reading numbers (0-100).\n");
    printf("Enter up to 10 numbers, then 0 to process.\n");
    printf("Maximum 15 iterations.\n");

    run_state_machine(&ctx, 0);

    return 0;
}