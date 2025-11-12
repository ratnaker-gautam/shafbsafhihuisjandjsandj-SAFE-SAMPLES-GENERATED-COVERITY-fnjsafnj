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
            return READING;
        } else {
            return PROCESSING;
        }
    } else if (input == -1) {
        if (ctx->count > 0) {
            return PROCESSING;
        } else {
            return ERROR;
        }
    }
    return ERROR;
}

enum State handle_processing(struct Context *ctx, int input) {
    if (input != 0) {
        return ERROR;
    }
    for (int i = 0; i < ctx->count; i++) {
        ctx->sum += ctx->data[i];
    }
    return FINISHED;
}

enum State handle_finished(struct Context *ctx, int input) {
    return FINISHED;
}

enum State handle_error(struct Context *ctx, int input) {
    return ERROR;
}

enum State process_input(struct Context *ctx, int input) {
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

void run_state_machine(struct Context *ctx, int input) {
    enum State new_state = process_input(ctx, input);
    ctx->current_state = new_state;
    
    if (ctx->current_state == FINISHED) {
        printf("Processing complete. Sum: %d\n", ctx->sum);
    } else if (ctx->current_state == ERROR) {
        printf("Error state reached.\n");
    } else if (ctx->current_state == PROCESSING) {
        run_state_machine(ctx, 0);
    }
}

int main(void) {
    struct Context ctx;
    ctx.current_state = START;
    
    printf("State Machine: Enter numbers 0-100 to collect, -1 to process, invalid to error\n");
    
    int input;
    while (ctx.current_state != FINISHED && ctx.current_state != ERROR) {
        if (scanf("%d", &input) != 1) {
            break;
        }
        run_state_machine(&ctx, input);
    }
    
    if (ctx.current_state != FINISHED && ctx.current_state != ERROR) {
        printf("Invalid input or early termination.\n");
    }
    
    return 0;
}