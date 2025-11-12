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

enum State handle_start(struct Context* ctx, int input);
enum State handle_reading(struct Context* ctx, int input);
enum State handle_processing(struct Context* ctx, int input);
enum State handle_finished(struct Context* ctx, int input);
enum State handle_error(struct Context* ctx, int input);

enum State transition(struct Context* ctx, int input) {
    switch(ctx->current_state) {
        case START: return handle_start(ctx, input);
        case READING: return handle_reading(ctx, input);
        case PROCESSING: return handle_processing(ctx, input);
        case FINISHED: return handle_finished(ctx, input);
        case ERROR: return handle_error(ctx, input);
        default: return ERROR;
    }
}

enum State handle_start(struct Context* ctx, int input) {
    if (input == 0) {
        ctx->count = 0;
        ctx->sum = 0;
        printf("Starting data collection\n");
        return READING;
    }
    return ERROR;
}

enum State handle_reading(struct Context* ctx, int input) {
    if (input > 0) {
        if (ctx->count < 10) {
            ctx->data[ctx->count] = input;
            ctx->count++;
            printf("Added value: %d\n", input);
            if (ctx->count == 10) {
                return PROCESSING;
            }
            return READING;
        }
    } else if (input == 0) {
        if (ctx->count > 0) {
            return PROCESSING;
        }
    }
    return ERROR;
}

enum State handle_processing(struct Context* ctx, int input) {
    if (input != 0) return ERROR;
    
    ctx->sum = 0;
    for (int i = 0; i < ctx->count; i++) {
        if (ctx->sum > 1000000 - ctx->data[i]) {
            return ERROR;
        }
        ctx->sum += ctx->data[i];
    }
    
    printf("Processed %d numbers, sum: %d\n", ctx->count, ctx->sum);
    return FINISHED;
}

enum State handle_finished(struct Context* ctx, int input) {
    if (input == -1) {
        printf("Resetting state machine\n");
        return START;
    }
    return FINISHED;
}

enum State handle_error(struct Context* ctx, int input) {
    if (input == -1) {
        printf("Resetting from error state\n");
        return START;
    }
    return ERROR;
}

void process_inputs(struct Context* ctx, int* inputs, int count, int index) {
    if (index >= count) return;
    
    int input = inputs[index];
    enum State new_state = transition(ctx, input);
    ctx->current_state = new_state;
    
    process_inputs(ctx, inputs, count, index + 1);
}

int main(void) {
    struct Context ctx = {START, {0}, 0, 0};
    
    int test_inputs[] = {0, 5, 10, 15, 20, 25, 0, -1};
    int input_count = sizeof(test_inputs) / sizeof(test_inputs[0]);
    
    printf("Running state machine test\n");
    process_inputs(&ctx, test_inputs, input_count, 0);
    
    printf("Final state: ");
    switch(ctx.current_state) {
        case START: printf("START\n"); break;
        case READING: printf("READING\n"); break;
        case PROCESSING: printf("PROCESSING\n"); break;
        case FINISHED: printf("FINISHED\n"); break;
        case ERROR: printf("ERROR\n"); break;
    }
    
    return 0;
}