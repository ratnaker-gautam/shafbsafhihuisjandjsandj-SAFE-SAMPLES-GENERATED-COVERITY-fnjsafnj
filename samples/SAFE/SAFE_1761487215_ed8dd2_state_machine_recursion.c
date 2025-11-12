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
    } else if (input == -1) {
        if (ctx->count > 0) {
            return PROCESSING;
        } else {
            return ERROR;
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

void run_state_machine(struct Context* ctx, int depth) {
    if (depth <= 0 || ctx->current_state == FINISHED || ctx->current_state == ERROR) {
        return;
    }
    
    printf("Current state: %d, Enter input: ", ctx->current_state);
    
    int input;
    if (scanf("%d", &input) != 1) {
        ctx->current_state = ERROR;
        return;
    }
    
    if (input < -1 || input > 100) {
        ctx->current_state = ERROR;
        return;
    }
    
    enum State new_state = process_input(ctx, input);
    ctx->current_state = new_state;
    
    if (ctx->current_state == FINISHED) {
        printf("Processing complete. Sum: %d\n", ctx->sum);
        return;
    } else if (ctx->current_state == ERROR) {
        printf("State machine error occurred.\n");
        return;
    }
    
    run_state_machine(ctx, depth - 1);
}

int main() {
    struct Context ctx;
    ctx.current_state = START;
    ctx.count = 0;
    ctx.sum = 0;
    
    printf("State Machine Demo\n");
    printf("Instructions:\n");
    printf("Start with 1 to begin reading\n");
    printf("Enter numbers 0-100 to store (max 10)\n");
    printf("Enter -1 to finish reading\n");
    printf("Enter 0 to process and get sum\n");
    
    run_state_machine(&ctx, 20);
    
    if (ctx.current_state != FINISHED && ctx.current_state != ERROR) {
        printf("State machine terminated unexpectedly.\n");
    }
    
    return 0;
}