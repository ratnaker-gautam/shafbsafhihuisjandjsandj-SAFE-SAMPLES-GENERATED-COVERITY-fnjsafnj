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
    if (input == 0) {
        ctx->count = 0;
        ctx->sum = 0;
        printf("State machine started. Enter numbers (0-100), -1 to process.\n");
        return READING;
    }
    return ERROR;
}

enum State handle_reading(struct Context *ctx, int input) {
    if (input == -1) {
        if (ctx->count > 0) {
            printf("Processing %d numbers...\n", ctx->count);
            return PROCESSING;
        } else {
            printf("No numbers to process.\n");
            return ERROR;
        }
    }
    if (input < 0 || input > 100) {
        printf("Invalid input. Enter numbers 0-100 or -1 to process.\n");
        return READING;
    }
    if (ctx->count >= 10) {
        printf("Buffer full. Processing...\n");
        return PROCESSING;
    }
    ctx->data[ctx->count] = input;
    ctx->count++;
    printf("Added %d. Count: %d\n", input, ctx->count);
    return READING;
}

enum State handle_processing(struct Context *ctx, int input) {
    if (input != 0) return ERROR;
    ctx->sum = 0;
    for (int i = 0; i < ctx->count; i++) {
        ctx->sum += ctx->data[i];
    }
    printf("Processed %d numbers. Sum: %d\n", ctx->count, ctx->sum);
    return FINISHED;
}

enum State handle_finished(struct Context *ctx, int input) {
    if (input == 0) {
        printf("Resetting state machine.\n");
        return START;
    }
    return FINISHED;
}

enum State handle_error(struct Context *ctx, int input) {
    if (input == 0) {
        printf("Error state cleared. Resetting.\n");
        return START;
    }
    return ERROR;
}

void run_state_machine(struct Context *ctx, int input) {
    enum State new_state = transition(ctx, input);
    if (new_state != ctx->current_state) {
        ctx->current_state = new_state;
        run_state_machine(ctx, 0);
    }
}

int main(void) {
    struct Context ctx;
    ctx.current_state = START;
    ctx.count = 0;
    ctx.sum = 0;
    
    printf("State Machine Demo\n");
    printf("Commands: 0=start/reset, 1-100=add number, -1=process, other=noop\n");
    
    int input;
    while (ctx.current_state != FINISHED) {
        printf("Current state: %d > ", ctx.current_state);
        if (scanf("%d", &input) != 1) {
            while (getchar() != '\n');
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        run_state_machine(&ctx, input);
    }
    
    printf("State machine finished successfully.\n");
    return 0;
}