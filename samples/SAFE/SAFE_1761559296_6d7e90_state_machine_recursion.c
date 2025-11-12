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
    if (ctx->step_count >= 100) return ERROR;
    
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
            return ERROR;
    }
}

enum State handle_start(struct Context *ctx, int input) {
    if (input < 0 || input > 100) return ERROR;
    
    ctx->data = input;
    printf("Starting with value: %d\n", input);
    return PROCESSING;
}

enum State handle_processing(struct Context *ctx, int input) {
    if (input < -10 || input > 10) return ERROR;
    
    if (ctx->data + input < 0) {
        printf("Value would become negative: %d + %d\n", ctx->data, input);
        return ERROR;
    }
    
    if (ctx->data + input > 1000) {
        printf("Value would exceed maximum: %d + %d\n", ctx->data, input);
        return ERROR;
    }
    
    ctx->data += input;
    printf("Processing: new value = %d\n", ctx->data);
    
    if (ctx->data >= 50 && ctx->data <= 100) {
        return FINISHED;
    }
    
    return PROCESSING;
}

enum State handle_finished(struct Context *ctx, int input) {
    printf("Final value: %d\n", ctx->data);
    return FINISHED;
}

enum State handle_error(struct Context *ctx, int input) {
    printf("Error state reached\n");
    return ERROR;
}

void run_state_machine(struct Context *ctx, int depth) {
    if (ctx == NULL || depth <= 0 || depth > 50) return;
    
    if (ctx->current_state == FINISHED || ctx->current_state == ERROR) {
        return;
    }
    
    int input;
    printf("Enter input for state %d: ", ctx->current_state);
    
    if (scanf("%d", &input) != 1) {
        ctx->current_state = ERROR;
        return;
    }
    
    if (input < -100 || input > 100) {
        ctx->current_state = ERROR;
        return;
    }
    
    enum State new_state = transition(ctx, input);
    ctx->current_state = new_state;
    
    run_state_machine(ctx, depth - 1);
}

int main(void) {
    struct Context ctx;
    ctx.current_state = START;
    ctx.data = 0;
    ctx.step_count = 0;
    
    printf("State Machine Demo\n");
    printf("Enter integers between -100 and 100\n");
    printf("Goal: reach value between 50 and 100\n\n");
    
    run_state_machine(&ctx, 20);
    
    if (ctx.current_state == FINISHED) {
        printf("Success! Completed in %d steps\n", ctx.step_count);
    } else {
        printf("Failed in state %d after %d steps\n", ctx.current_state, ctx.step_count);
    }
    
    return 0;
}