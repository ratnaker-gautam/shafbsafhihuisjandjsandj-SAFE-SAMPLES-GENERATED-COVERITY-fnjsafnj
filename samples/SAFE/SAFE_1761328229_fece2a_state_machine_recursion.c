//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State { START, PROCESSING, VALIDATING, FINISHED, ERROR };

struct Context {
    enum State current_state;
    int data;
    int step_count;
    int max_steps;
};

enum State handle_start(struct Context *ctx);
enum State handle_processing(struct Context *ctx);
enum State handle_validating(struct Context *ctx);
enum State handle_finished(struct Context *ctx);
enum State handle_error(struct Context *ctx);

enum State transition(enum State current, struct Context *ctx) {
    if (ctx == NULL) return ERROR;
    if (ctx->step_count >= ctx->max_steps) return ERROR;
    
    ctx->step_count++;
    
    switch (current) {
        case START:
            return handle_start(ctx);
        case PROCESSING:
            return handle_processing(ctx);
        case VALIDATING:
            return handle_validating(ctx);
        case FINISHED:
            return handle_finished(ctx);
        case ERROR:
            return handle_error(ctx);
        default:
            return ERROR;
    }
}

enum State handle_start(struct Context *ctx) {
    if (ctx == NULL) return ERROR;
    
    printf("State: START - Initializing data\n");
    ctx->data = 0;
    
    if (ctx->data < 0) return ERROR;
    return PROCESSING;
}

enum State handle_processing(struct Context *ctx) {
    if (ctx == NULL) return ERROR;
    
    printf("State: PROCESSING - Incrementing data\n");
    
    if (ctx->data > 1000) return ERROR;
    ctx->data += 10;
    
    if (ctx->data < 0) return ERROR;
    return VALIDATING;
}

enum State handle_validating(struct Context *ctx) {
    if (ctx == NULL) return ERROR;
    
    printf("State: VALIDATING - Checking data value: %d\n", ctx->data);
    
    if (ctx->data >= 50 && ctx->data <= 100) {
        return FINISHED;
    } else if (ctx->data < 100) {
        return PROCESSING;
    } else {
        return ERROR;
    }
}

enum State handle_finished(struct Context *ctx) {
    if (ctx == NULL) return ERROR;
    
    printf("State: FINISHED - Final data value: %d\n", ctx->data);
    return FINISHED;
}

enum State handle_error(struct Context *ctx) {
    if (ctx == NULL) return ERROR;
    
    printf("State: ERROR - Invalid state reached\n");
    return ERROR;
}

void run_state_machine(struct Context *ctx) {
    if (ctx == NULL) return;
    
    enum State next_state = ctx->current_state;
    
    while (next_state != FINISHED && next_state != ERROR) {
        next_state = transition(next_state, ctx);
        ctx->current_state = next_state;
        
        if (ctx->step_count >= ctx->max_steps) {
            ctx->current_state = ERROR;
            break;
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <max_steps>\n", argv[0]);
        return 1;
    }
    
    char *endptr;
    long max_steps = strtol(argv[1], &endptr, 10);
    
    if (endptr == argv[1] || *endptr != '\0') {
        fprintf(stderr, "Error: Invalid number format\n");
        return 1;
    }
    
    if (max_steps <= 0 || max_steps > 1000) {
        fprintf(stderr, "Error: Max steps must be between 1 and 1000\n");
        return 1;
    }
    
    struct Context ctx;
    ctx.current_state = START;
    ctx.data = 0;
    ctx.step_count = 0;
    ctx.max_steps = (int)max_steps;
    
    if (ctx.max_steps != max_steps) {
        fprintf(stderr, "Error: Integer overflow detected\n");
        return 1;
    }
    
    run_state_machine(&ctx);
    
    if (ctx.current_state == FINISHED) {
        printf("State machine completed successfully in %d steps\n", ctx.step_count);
        return 0;
    } else {
        printf("State machine failed after %d steps\n", ctx.step_count);
        return 1;
    }
}