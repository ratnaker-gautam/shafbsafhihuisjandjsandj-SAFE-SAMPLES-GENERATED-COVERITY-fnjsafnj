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

void run_state_machine(struct Context *ctx, int depth) {
    if (depth <= 0) {
        return;
    }
    
    int input;
    if (scanf("%d", &input) != 1) {
        ctx->current_state = ERROR;
        return;
    }
    
    if (input < -1 || input > 100) {
        ctx->current_state = ERROR;
        return;
    }
    
    ctx->current_state = process_input(ctx, input);
    
    switch (ctx->current_state) {
        case START:
            printf("State: START\n");
            break;
        case READING:
            printf("State: READING (count=%d)\n", ctx->count);
            break;
        case PROCESSING:
            printf("State: PROCESSING\n");
            break;
        case FINISHED:
            printf("State: FINISHED (sum=%d)\n", ctx->sum);
            break;
        case ERROR:
            printf("State: ERROR\n");
            break;
    }
    
    run_state_machine(ctx, depth - 1);
}

int main(void) {
    struct Context ctx;
    ctx.current_state = START;
    ctx.count = 0;
    ctx.sum = 0;
    
    printf("Enter inputs (max 20):\n");
    printf("1 to start, 0-100 for data, 0 to process, -1 to reset\n");
    
    run_state_machine(&ctx, 20);
    
    return 0;
}