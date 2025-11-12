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
            if (ctx->count == 10) {
                return PROCESSING;
            }
            return READING;
        }
    }
    return ERROR;
}

enum State handle_processing(struct Context* ctx, int input) {
    if (input == 0) {
        ctx->sum = 0;
        for (int i = 0; i < ctx->count; i++) {
            if (ctx->sum > 1000 - ctx->data[i]) {
                return ERROR;
            }
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

void run_state_machine(struct Context* ctx, int* inputs, int input_count, int current_index) {
    if (current_index >= input_count) {
        return;
    }
    
    enum State next_state = process_input(ctx, inputs[current_index]);
    ctx->current_state = next_state;
    
    run_state_machine(ctx, inputs, input_count, current_index + 1);
}

int main() {
    struct Context ctx;
    ctx.current_state = START;
    
    int inputs[20];
    int input_count = 0;
    
    printf("Enter inputs (max 20, -1 to stop):\n");
    for (int i = 0; i < 20; i++) {
        int value;
        if (scanf("%d", &value) != 1) {
            break;
        }
        if (value == -1) {
            break;
        }
        inputs[input_count] = value;
        input_count++;
        if (input_count >= 20) {
            break;
        }
    }
    
    if (input_count == 0) {
        printf("No inputs provided\n");
        return 1;
    }
    
    run_state_machine(&ctx, inputs, input_count, 0);
    
    switch (ctx.current_state) {
        case FINISHED:
            printf("Success! Sum: %d\n", ctx.sum);
            break;
        case ERROR:
            printf("Error occurred during processing\n");
            break;
        default:
            printf("Processing incomplete\n");
            break;
    }
    
    return 0;
}