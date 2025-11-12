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

void run_state_machine(struct Context* ctx, int step) {
    if (step >= 15 || ctx->current_state == FINISHED || ctx->current_state == ERROR) {
        return;
    }

    printf("Step %d - Current state: ", step);
    switch (ctx->current_state) {
        case START: printf("START"); break;
        case READING: printf("READING"); break;
        case PROCESSING: printf("PROCESSING"); break;
        case FINISHED: printf("FINISHED"); break;
        case ERROR: printf("ERROR"); break;
    }
    printf("\n");

    if (ctx->current_state == READING) {
        printf("Enter a number (0-100) to store, or any other to error: ");
    } else if (ctx->current_state == START) {
        printf("Enter 1 to begin: ");
    } else if (ctx->current_state == PROCESSING) {
        printf("Enter 0 to process: ");
    } else {
        printf("Enter any number: ");
    }

    int input;
    if (scanf("%d", &input) != 1) {
        ctx->current_state = ERROR;
        printf("Invalid input\n");
        return;
    }

    enum State new_state = process_input(ctx, input);
    ctx->current_state = new_state;

    if (ctx->current_state == FINISHED) {
        printf("Processing complete. Sum: %d\n", ctx->sum);
    } else if (ctx->current_state == ERROR) {
        printf("State machine entered error state\n");
    }

    run_state_machine(ctx, step + 1);
}

int main(void) {
    struct Context ctx;
    ctx.current_state = START;
    ctx.count = 0;
    ctx.sum = 0;

    printf("State Machine Demo - Process up to 10 numbers\n");
    run_state_machine(&ctx, 0);

    return 0;
}