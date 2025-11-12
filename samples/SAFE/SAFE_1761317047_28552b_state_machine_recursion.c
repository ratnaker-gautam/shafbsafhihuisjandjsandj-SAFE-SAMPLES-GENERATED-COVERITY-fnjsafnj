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
    printf("State: START - Initializing with data: %d\n", ctx->data);
    ctx->steps_remaining--;
    return PROCESSING;
}

enum State handle_processing(struct Context *ctx) {
    if (ctx->steps_remaining <= 0) {
        return ERROR;
    }
    if (ctx->data < 0) {
        return ERROR;
    }
    printf("State: PROCESSING - Data: %d, Steps left: %d\n", ctx->data, ctx->steps_remaining);
    ctx->data *= 2;
    ctx->steps_remaining--;
    if (ctx->steps_remaining == 0) {
        return FINISHED;
    }
    return PROCESSING;
}

enum State handle_finished(struct Context *ctx) {
    printf("State: FINISHED - Final data: %d\n", ctx->data);
    return FINISHED;
}

enum State handle_error(struct Context *ctx) {
    printf("State: ERROR - Invalid state detected\n");
    return ERROR;
}

enum State transition(enum State current, struct Context *ctx) {
    switch (current) {
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
    
    enum State next_state = transition(ctx->current_state, ctx);
    if (next_state == ctx->current_state && next_state != PROCESSING) {
        return;
    }
    
    ctx->current_state = next_state;
    run_state_machine(ctx);
}

int main() {
    struct Context ctx;
    char input_buffer[32];
    int user_data, user_steps;
    
    printf("Enter initial data (integer): ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    if (sscanf(input_buffer, "%d", &user_data) != 1) {
        printf("Invalid data input\n");
        return 1;
    }
    
    printf("Enter number of processing steps (1-10): ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    if (sscanf(input_buffer, "%d", &user_steps) != 1) {
        printf("Invalid steps input\n");
        return 1;
    }
    
    if (user_steps < 1 || user_steps > 10) {
        printf("Steps must be between 1 and 10\n");
        return 1;
    }
    
    ctx.current_state = START;
    ctx.data = user_data;
    ctx.steps_remaining = user_steps;
    
    run_state_machine(&ctx);
    
    return 0;
}