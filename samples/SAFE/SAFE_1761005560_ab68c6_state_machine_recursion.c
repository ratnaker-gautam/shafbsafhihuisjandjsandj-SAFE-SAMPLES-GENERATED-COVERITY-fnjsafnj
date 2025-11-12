//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State {
    START,
    PROCESSING,
    VALIDATING,
    FINISHED,
    ERROR
};

struct Context {
    enum State current_state;
    int data;
    int step;
};

enum State handle_start(struct Context *ctx);
enum State handle_processing(struct Context *ctx);
enum State handle_validating(struct Context *ctx);
enum State handle_finished(struct Context *ctx);
enum State handle_error(struct Context *ctx);

enum State transition(struct Context *ctx) {
    if (ctx == NULL) {
        return ERROR;
    }
    
    switch (ctx->current_state) {
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
    if (ctx == NULL) {
        return ERROR;
    }
    
    printf("Starting process...\n");
    ctx->data = 0;
    ctx->step = 1;
    ctx->current_state = PROCESSING;
    return transition(ctx);
}

enum State handle_processing(struct Context *ctx) {
    if (ctx == NULL) {
        return ERROR;
    }
    
    if (ctx->step > 3) {
        ctx->current_state = VALIDATING;
        return transition(ctx);
    }
    
    printf("Processing step %d\n", ctx->step);
    ctx->data += ctx->step * 10;
    ctx->step++;
    
    if (ctx->data > 1000) {
        ctx->current_state = ERROR;
        return transition(ctx);
    }
    
    return transition(ctx);
}

enum State handle_validating(struct Context *ctx) {
    if (ctx == NULL) {
        return ERROR;
    }
    
    printf("Validating data: %d\n", ctx->data);
    
    if (ctx->data >= 60 && ctx->data <= 100) {
        ctx->current_state = FINISHED;
    } else {
        ctx->current_state = ERROR;
    }
    
    return transition(ctx);
}

enum State handle_finished(struct Context *ctx) {
    if (ctx == NULL) {
        return ERROR;
    }
    
    printf("Process completed successfully. Final data: %d\n", ctx->data);
    return FINISHED;
}

enum State handle_error(struct Context *ctx) {
    if (ctx == NULL) {
        return ERROR;
    }
    
    printf("Error occurred. Data value: %d\n", ctx->data);
    return ERROR;
}

int main(void) {
    struct Context ctx;
    ctx.current_state = START;
    ctx.data = 0;
    ctx.step = 0;
    
    enum State final_state = transition(&ctx);
    
    if (final_state == FINISHED) {
        return EXIT_SUCCESS;
    } else {
        return EXIT_FAILURE;
    }
}