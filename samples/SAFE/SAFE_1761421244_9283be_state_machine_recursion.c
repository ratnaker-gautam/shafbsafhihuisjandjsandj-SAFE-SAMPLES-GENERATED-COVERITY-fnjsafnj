//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State { START, PROCESSING, FINISHED, ERROR };

struct Context {
    enum State current_state;
    int data;
    int steps_remaining;
};

enum State handle_start(struct Context *ctx) {
    if (ctx->steps_remaining <= 0) {
        return ERROR;
    }
    printf("Starting process with %d steps\n", ctx->steps_remaining);
    ctx->data = 0;
    return PROCESSING;
}

enum State handle_processing(struct Context *ctx) {
    if (ctx->steps_remaining <= 0) {
        return ERROR;
    }
    ctx->data += ctx->steps_remaining;
    printf("Processing step %d, data: %d\n", ctx->steps_remaining, ctx->data);
    ctx->steps_remaining--;
    if (ctx->steps_remaining > 0) {
        return PROCESSING;
    } else {
        return FINISHED;
    }
}

enum State handle_finished(struct Context *ctx) {
    printf("Process finished. Final data: %d\n", ctx->data);
    return FINISHED;
}

enum State handle_error(struct Context *ctx) {
    printf("Error occurred in state machine\n");
    return ERROR;
}

enum State process_state(struct Context *ctx) {
    switch (ctx->current_state) {
        case START:
            return handle_start(ctx);
        case PROCESSING:
            return handle_processing(ctx);
        case FINISHED:
            return handle_finished(ctx);
        case ERROR:
            return handle_error(ctx);
        default:
            return ERROR;
    }
}

void run_state_machine(struct Context *ctx) {
    if (ctx == NULL) {
        return;
    }
    
    enum State next_state = process_state(ctx);
    if (next_state != ctx->current_state) {
        ctx->current_state = next_state;
        run_state_machine(ctx);
    }
}

int main() {
    int input_steps;
    printf("Enter number of processing steps (1-20): ");
    
    if (scanf("%d", &input_steps) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (input_steps < 1 || input_steps > 20) {
        printf("Steps must be between 1 and 20\n");
        return 1;
    }
    
    struct Context ctx;
    ctx.current_state = START;
    ctx.steps_remaining = input_steps;
    ctx.data = 0;
    
    run_state_machine(&ctx);
    
    return 0;
}