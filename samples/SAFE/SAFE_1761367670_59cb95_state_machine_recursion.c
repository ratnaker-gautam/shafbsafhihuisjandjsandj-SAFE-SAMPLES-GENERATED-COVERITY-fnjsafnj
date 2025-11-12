//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State { START, PROCESSING, FINISHED, ERROR };

struct Context {
    enum State current_state;
    int data;
    int step_count;
};

enum State handle_start(struct Context *ctx, int input);
enum State handle_processing(struct Context *ctx, int input);
enum State handle_finished(struct Context *ctx, int input);
enum State handle_error(struct Context *ctx, int input);

enum State transition(struct Context *ctx, int input) {
    if (ctx == NULL) return ERROR;
    if (ctx->step_count >= 100) {
        ctx->current_state = ERROR;
        return ERROR;
    }
    ctx->step_count++;
    
    switch (ctx->current_state) {
        case START:
            return handle_start(ctx, input);
        case PROCESSING:
            return handle_processing(ctx, input);
        case FINISHED:
            return handle_finished(ctx, input);
        case ERROR:
            return handle_error(ctx, input);
        default:
            ctx->current_state = ERROR;
            return ERROR;
    }
}

enum State handle_start(struct Context *ctx, int input) {
    if (input < 0 || input > 100) {
        ctx->current_state = ERROR;
        return ERROR;
    }
    ctx->data = input;
    ctx->current_state = PROCESSING;
    return PROCESSING;
}

enum State handle_processing(struct Context *ctx, int input) {
    if (input == 0) {
        ctx->current_state = FINISHED;
        return FINISHED;
    }
    if (input < 0 || input > 50) {
        ctx->current_state = ERROR;
        return ERROR;
    }
    if (ctx->data > 1000 - input) {
        ctx->current_state = ERROR;
        return ERROR;
    }
    ctx->data += input;
    return PROCESSING;
}

enum State handle_finished(struct Context *ctx, int input) {
    if (input != 0) {
        ctx->current_state = ERROR;
        return ERROR;
    }
    return FINISHED;
}

enum State handle_error(struct Context *ctx, int input) {
    return ERROR;
}

void run_state_machine(struct Context *ctx) {
    if (ctx == NULL) return;
    
    int input;
    printf("Current state: %d, Data: %d\n", ctx->current_state, ctx->data);
    printf("Enter input (-1 to exit): ");
    
    if (scanf("%d", &input) != 1) {
        ctx->current_state = ERROR;
        return;
    }
    
    if (input == -1) {
        return;
    }
    
    enum State new_state = transition(ctx, input);
    ctx->current_state = new_state;
    
    run_state_machine(ctx);
}

int main(void) {
    struct Context ctx;
    ctx.current_state = START;
    ctx.data = 0;
    ctx.step_count = 0;
    
    printf("State Machine Started\n");
    run_state_machine(&ctx);
    
    printf("Final state: %d, Final data: %d\n", ctx.current_state, ctx.data);
    
    if (ctx.current_state == FINISHED) {
        printf("Success: Processing completed\n");
    } else if (ctx.current_state == ERROR) {
        printf("Error: Invalid state reached\n");
    } else {
        printf("Warning: Process terminated early\n");
    }
    
    return 0;
}