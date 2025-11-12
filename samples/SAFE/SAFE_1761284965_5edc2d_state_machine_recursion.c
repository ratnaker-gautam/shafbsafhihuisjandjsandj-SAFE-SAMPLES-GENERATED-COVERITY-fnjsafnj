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

void recursive_state_processor(struct Context *ctx, int iteration) {
    if (iteration >= 20) {
        return;
    }
    
    if (ctx->current_state == FINISHED) {
        printf("Processing complete. Sum: %d\n", ctx->sum);
        return;
    }
    
    if (ctx->current_state == ERROR) {
        printf("Error state detected. Reset with -1\n");
        return;
    }
    
    int input;
    printf("Enter input for state %d: ", ctx->current_state);
    
    if (scanf("%d", &input) != 1) {
        printf("Invalid input\n");
        while (getchar() != '\n');
        recursive_state_processor(ctx, iteration + 1);
        return;
    }
    
    process_state_machine(ctx, input);
    recursive_state_processor(ctx, iteration + 1);
}

int main(void) {
    struct Context ctx;
    ctx.current_state = START;
    ctx.count = 0;
    ctx.sum = 0;
    
    printf("State Machine Demo\n");
    printf("Start with 1, enter 0-100 values, finish with 0\n");
    printf("Use -1 to reset from any state\n");
    
    recursive_state_processor(&ctx, 0);
    
    return 0;
}