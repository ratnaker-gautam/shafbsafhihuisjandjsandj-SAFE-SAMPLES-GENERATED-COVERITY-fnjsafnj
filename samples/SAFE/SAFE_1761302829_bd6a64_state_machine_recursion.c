//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>

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
    }
    return ERROR;
}

enum State handle_processing(struct Context *ctx, int input) {
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

void process_state_machine(struct Context *ctx, int input) {
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

void recursive_state_processor(struct Context *ctx, int remaining_inputs) {
    if (remaining_inputs <= 0) {
        return;
    }
    
    int input;
    if (scanf("%d", &input) != 1) {
        ctx->current_state = ERROR;
        return;
    }
    
    process_state_machine(ctx, input);
    
    if (ctx->current_state == FINISHED) {
        printf("Processing complete. Sum: %d\n", ctx->sum);
    } else if (ctx->current_state == ERROR) {
        printf("Error state reached. Reset with -1\n");
    }
    
    recursive_state_processor(ctx, remaining_inputs - 1);
}

int main(void) {
    struct Context ctx;
    ctx.current_state = START;
    ctx.count = 0;
    ctx.sum = 0;
    
    printf("State Machine Started\n");
    printf("Enter 1 to begin reading numbers (0-100)\n");
    printf("Enter 0 to process collected numbers\n");
    printf("Enter -1 to reset\n");
    printf("Maximum 10 numbers can be collected\n");
    
    recursive_state_processor(&ctx, 20);
    
    if (ctx.current_state == FINISHED) {
        printf("Final sum: %d\n", ctx.sum);
    }
    
    return 0;
}