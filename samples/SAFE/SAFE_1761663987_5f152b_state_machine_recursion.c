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

enum State handle_start(struct Context* ctx) {
    if (ctx->steps_remaining <= 0) {
        return ERROR;
    }
    printf("Starting process with %d steps\n", ctx->steps_remaining);
    ctx->data = 0;
    return PROCESSING;
}

enum State handle_processing(struct Context* ctx) {
    if (ctx->steps_remaining <= 0) {
        return ERROR;
    }
    
    ctx->data += ctx->steps_remaining;
    printf("Processing step %d, data: %d\n", ctx->steps_remaining, ctx->data);
    ctx->steps_remaining--;
    
    if (ctx->steps_remaining == 0) {
        return FINISHED;
    }
    return PROCESSING;
}

enum State handle_finished(struct Context* ctx) {
    printf("Process completed. Final data: %d\n", ctx->data);
    return FINISHED;
}

enum State handle_error(struct Context* ctx) {
    printf("Error occurred in state machine\n");
    return ERROR;
}

enum State process_state(struct Context* ctx) {
    if (ctx == NULL) {
        return ERROR;
    }
    
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

void run_state_machine(struct Context* ctx, int max_depth) {
    if (ctx == NULL || max_depth <= 0) {
        return;
    }
    
    enum State next_state = process_state(ctx);
    
    if (next_state != ctx->current_state) {
        ctx->current_state = next_state;
        run_state_machine(ctx, max_depth - 1);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <steps>\n", argv[0]);
        return 1;
    }
    
    char* endptr;
    long steps = strtol(argv[1], &endptr, 10);
    
    if (endptr == argv[1] || *endptr != '\0') {
        fprintf(stderr, "Invalid number: %s\n", argv[1]);
        return 1;
    }
    
    if (steps <= 0 || steps > 1000) {
        fprintf(stderr, "Steps must be between 1 and 1000\n");
        return 1;
    }
    
    struct Context ctx;
    ctx.current_state = START;
    ctx.data = 0;
    ctx.steps_remaining = (int)steps;
    
    run_state_machine(&ctx, 2000);
    
    return 0;
}