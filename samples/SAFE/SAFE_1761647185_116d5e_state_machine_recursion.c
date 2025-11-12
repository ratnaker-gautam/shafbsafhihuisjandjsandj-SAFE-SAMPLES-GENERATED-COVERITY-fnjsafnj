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

void process_state(struct Context* ctx, int input) {
    enum State next_state = ERROR;
    
    switch (ctx->current_state) {
        case START:
            next_state = handle_start(ctx, input);
            break;
        case READING:
            next_state = handle_reading(ctx, input);
            break;
        case PROCESSING:
            next_state = handle_processing(ctx, input);
            break;
        case FINISHED:
            next_state = handle_finished(ctx, input);
            break;
        case ERROR:
            next_state = handle_error(ctx, input);
            break;
    }
    
    ctx->current_state = next_state;
    
    if (ctx->current_state != FINISHED && ctx->current_state != ERROR) {
        int next_input;
        if (scanf("%d", &next_input) == 1) {
            process_state(ctx, next_input);
        } else {
            ctx->current_state = ERROR;
        }
    }
}

int main(void) {
    struct Context ctx;
    ctx.current_state = START;
    
    printf("Enter state machine inputs (integers):\n");
    printf("Start with 1, then 10 numbers 0-100, then 0 to process\n");
    
    int first_input;
    if (scanf("%d", &first_input) != 1) {
        printf("Error: Invalid input\n");
        return 1;
    }
    
    process_state(&ctx, first_input);
    
    if (ctx.current_state == FINISHED) {
        printf("Success! Sum of %d numbers: %d\n", ctx.count, ctx.sum);
        return 0;
    } else {
        printf("Error: Invalid state transition or input\n");
        return 1;
    }
}