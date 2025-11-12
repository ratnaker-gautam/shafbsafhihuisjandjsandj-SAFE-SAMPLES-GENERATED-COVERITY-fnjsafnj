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

enum State handle_start(struct Context *ctx, int input) {
    if (input == 1) {
        ctx->count = 0;
        ctx->sum = 0;
        return READING;
    }
    return ERROR;
}

enum State handle_reading(struct Context *ctx, int input) {
    if (input >= 0 && input <= 100) {
        if (ctx->count < 10) {
            ctx->data[ctx->count] = input;
            ctx->count++;
            if (ctx->count == 10) {
                return PROCESSING;
            }
            return READING;
        }
    }
    return ERROR;
}

enum State handle_processing(struct Context *ctx, int input) {
    if (input == 0) {
        for (int i = 0; i < ctx->count; i++) {
            ctx->sum += ctx->data[i];
        }
        return FINISHED;
    }
    return ERROR;
}

enum State handle_finished(struct Context *ctx, int input) {
    if (input == -1) {
        return START;
    }
    return FINISHED;
}

enum State handle_error(struct Context *ctx, int input) {
    if (input == -1) {
        return START;
    }
    return ERROR;
}

void state_machine(struct Context *ctx, int input) {
    switch (ctx->current_state) {
        case START:
            ctx->current_state = handle_start(ctx, input);
            break;
        case READING:
            ctx->current_state = handle_reading(ctx, input);
            break;
        case PROCESSING:
            ctx->current_state = handle_processing(ctx, input);
            break;
        case FINISHED:
            ctx->current_state = handle_finished(ctx, input);
            break;
        case ERROR:
            ctx->current_state = handle_error(ctx, input);
            break;
    }
}

void recursive_process(struct Context *ctx, int iteration) {
    if (iteration >= 20) {
        return;
    }
    
    printf("Iteration %d, Current state: %d\n", iteration, ctx->current_state);
    
    if (ctx->current_state == FINISHED) {
        printf("Sum of numbers: %d\n", ctx->sum);
        printf("Enter -1 to restart: ");
    } else if (ctx->current_state == ERROR) {
        printf("Error occurred. Enter -1 to restart: ");
    } else if (ctx->current_state == START) {
        printf("Enter 1 to start reading numbers: ");
    } else if (ctx->current_state == READING) {
        printf("Enter number %d (0-100): ", ctx->count + 1);
    } else if (ctx->current_state == PROCESSING) {
        printf("Enter 0 to process numbers: ");
    }
    
    int input;
    if (scanf("%d", &input) != 1) {
        while (getchar() != '\n');
        ctx->current_state = ERROR;
    } else {
        state_machine(ctx, input);
    }
    
    recursive_process(ctx, iteration + 1);
}

int main(void) {
    struct Context ctx;
    ctx.current_state = START;
    ctx.count = 0;
    ctx.sum = 0;
    
    printf("State Machine Demo - Number Processor\n");
    recursive_process(&ctx, 0);
    
    return 0;
}