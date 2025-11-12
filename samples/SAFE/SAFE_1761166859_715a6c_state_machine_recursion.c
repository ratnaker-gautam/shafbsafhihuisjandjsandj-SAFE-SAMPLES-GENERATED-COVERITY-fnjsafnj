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
            return READING;
        } else {
            return PARSING;
        }
    } else if (input == -1) {
        if (ctx->data_count > 0) {
            return PARSING;
        } else {
            return ERROR;
        }
    }
    return ERROR;
}

enum State handle_parsing(struct Context* ctx, int input) {
    if (input == 0) {
        if (ctx->data_count > 0) {
            ctx->processed_count = 0;
            ctx->sum = 0;
            return PROCESSING;
        } else {
            return ERROR;
        }
    }
    return ERROR;
}

enum State handle_processing(struct Context* ctx, int input) {
    if (input == 0) {
        if (ctx->processed_count < ctx->data_count) {
            ctx->sum += ctx->data[ctx->processed_count];
            ctx->processed_count++;
            return PROCESSING;
        } else {
            return FINISHED;
        }
    }
    return ERROR;
}

enum State handle_finished(struct Context* ctx, int input) {
    if (input == -2) {
        return START;
    }
    return FINISHED;
}

enum State handle_error(struct Context* ctx, int input) {
    if (input == -3) {
        return START;
    }
    return ERROR;
}

enum State process_input(struct Context* ctx, int input) {
    switch (ctx->current_state) {
        case START:
            return handle_start(ctx, input);
        case READING:
            return handle_reading(ctx, input);
        case PARSING:
            return handle_parsing(ctx, input);
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

void run_state_machine(struct Context* ctx, int* inputs, int input_count, int depth) {
    if (depth >= input_count) {
        return;
    }
    
    if (depth < 0 || depth >= 1000) {
        ctx->current_state = ERROR;
        return;
    }
    
    enum State next_state = process_input(ctx, inputs[depth]);
    ctx->current_state = next_state;
    
    run_state_machine(ctx, inputs, input_count, depth + 1);
}

int main() {
    struct Context ctx;
    ctx.current_state = START;
    
    int test_inputs[] = {1, 5, 10, 15, 20, -1, 0, 0, 0, 0, 0, -2};
    int input_count = sizeof(test_inputs) / sizeof(test_inputs[0]);
    
    run_state_machine(&ctx, test_inputs, input_count, 0);
    
    if (ctx.current_state == FINISHED) {
        printf("Processing completed. Sum: %d\n", ctx.sum);
    } else if (ctx.current_state == ERROR) {
        printf("State machine entered error state.\n");
    } else {
        printf("State machine in state: %d\n", ctx.current_state);
    }
    
    return 0;
}