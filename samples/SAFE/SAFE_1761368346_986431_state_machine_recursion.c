//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State { START, READING, PARSING, PROCESSING, FINISHED, ERROR };

struct Context {
    enum State current_state;
    int data[10];
    int data_count;
    int sum;
    int processed_count;
};

enum State handle_start(struct Context* ctx, int input) {
    if (input == 1) {
        ctx->data_count = 0;
        ctx->sum = 0;
        ctx->processed_count = 0;
        return READING;
    }
    return ERROR;
}

enum State handle_reading(struct Context* ctx, int input) {
    if (input >= 0 && input <= 100) {
        if (ctx->data_count < 10) {
            ctx->data[ctx->data_count] = input;
            ctx->data_count++;
            if (ctx->data_count == 10) {
                return PARSING;
            }
            return READING;
        }
    }
    return ERROR;
}

enum State handle_parsing(struct Context* ctx, int input) {
    if (input == 2) {
        ctx->sum = 0;
        ctx->processed_count = 0;
        return PROCESSING;
    }
    return ERROR;
}

enum State handle_processing(struct Context* ctx, int input) {
    if (input == 3) {
        if (ctx->processed_count < ctx->data_count) {
            ctx->sum += ctx->data[ctx->processed_count];
            ctx->processed_count++;
            if (ctx->processed_count == ctx->data_count) {
                return FINISHED;
            }
            return PROCESSING;
        }
    }
    return ERROR;
}

enum State handle_finished(struct Context* ctx, int input) {
    if (input == 4) {
        printf("Result: %d\n", ctx->sum);
        return START;
    }
    return ERROR;
}

enum State handle_error(struct Context* ctx, int input) {
    if (input == 0) {
        printf("Reset\n");
        return START;
    }
    return ERROR;
}

enum State process_input(struct Context* ctx, int input) {
    switch (ctx->current_state) {
        case START: return handle_start(ctx, input);
        case READING: return handle_reading(ctx, input);
        case PARSING: return handle_parsing(ctx, input);
        case PROCESSING: return handle_processing(ctx, input);
        case FINISHED: return handle_finished(ctx, input);
        case ERROR: return handle_error(ctx, input);
        default: return ERROR;
    }
}

void state_machine_recursive(struct Context* ctx, int remaining_inputs) {
    if (remaining_inputs <= 0) {
        return;
    }
    
    int input;
    if (scanf("%d", &input) != 1) {
        printf("Invalid input\n");
        ctx->current_state = ERROR;
        return;
    }
    
    if (input < 0 || input > 100) {
        printf("Input out of range\n");
        ctx->current_state = ERROR;
        return;
    }
    
    ctx->current_state = process_input(ctx, input);
    
    if (ctx->current_state == ERROR) {
        printf("State machine error\n");
    }
    
    state_machine_recursive(ctx, remaining_inputs - 1);
}

int main(void) {
    struct Context ctx;
    ctx.current_state = START;
    ctx.data_count = 0;
    ctx.sum = 0;
    ctx.processed_count = 0;
    
    printf("Enter inputs (max 20): ");
    int max_inputs = 20;
    state_machine_recursive(&ctx, max_inputs);
    
    return 0;
}