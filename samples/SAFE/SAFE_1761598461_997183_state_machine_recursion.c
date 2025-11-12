//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State {
    START,
    READING,
    PROCESSING,
    FINISHED,
    ERROR
};

struct Context {
    enum State current_state;
    int data[10];
    int count;
    int sum;
};

enum State handle_start(struct Context *ctx, int input) {
    if (input < 0 || input > 100) {
        return ERROR;
    }
    ctx->count = 0;
    ctx->sum = 0;
    return READING;
}

enum State handle_reading(struct Context *ctx, int input) {
    if (input < 0 || input > 100) {
        return ERROR;
    }
    if (input == 0) {
        if (ctx->count == 0) {
            return ERROR;
        }
        return PROCESSING;
    }
    if (ctx->count >= 10) {
        return ERROR;
    }
    ctx->data[ctx->count] = input;
    ctx->count++;
    return READING;
}

enum State handle_processing(struct Context *ctx) {
    for (int i = 0; i < ctx->count; i++) {
        ctx->sum += ctx->data[i];
    }
    return FINISHED;
}

void process_state_machine(struct Context *ctx, int input) {
    enum State next_state = ctx->current_state;
    
    switch (ctx->current_state) {
        case START:
            next_state = handle_start(ctx, input);
            break;
        case READING:
            next_state = handle_reading(ctx, input);
            break;
        case PROCESSING:
            next_state = handle_processing(ctx);
            break;
        case FINISHED:
            return;
        case ERROR:
            return;
    }
    
    if (next_state != ctx->current_state) {
        ctx->current_state = next_state;
        if (ctx->current_state == PROCESSING) {
            process_state_machine(ctx, 0);
        }
    }
}

int main(void) {
    struct Context ctx;
    ctx.current_state = START;
    
    printf("Enter numbers (0-100), 0 to finish: ");
    
    int input;
    while (ctx.current_state != FINISHED && ctx.current_state != ERROR) {
        if (scanf("%d", &input) != 1) {
            ctx.current_state = ERROR;
            break;
        }
        
        process_state_machine(&ctx, input);
    }
    
    if (ctx.current_state == ERROR) {
        printf("Error: Invalid input or state\n");
        return 1;
    }
    
    printf("Sum of %d numbers: %d\n", ctx.count, ctx.sum);
    return 0;
}