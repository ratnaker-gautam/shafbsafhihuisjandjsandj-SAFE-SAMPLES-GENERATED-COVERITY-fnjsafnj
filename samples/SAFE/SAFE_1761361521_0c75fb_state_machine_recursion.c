//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State { START, PROCESSING, FINISHED, ERROR };

struct Context {
    enum State current_state;
    int data;
    int step;
};

enum State process_start(struct Context *ctx);
enum State process_processing(struct Context *ctx);
enum State process_finished(struct Context *ctx);
enum State process_error(struct Context *ctx);

enum State process_start(struct Context *ctx) {
    if (ctx == NULL) return ERROR;
    if (ctx->current_state != START) return ERROR;
    
    printf("State: START - Enter initial data (0-100): ");
    if (scanf("%d", &ctx->data) != 1) return ERROR;
    if (ctx->data < 0 || ctx->data > 100) return ERROR;
    
    ctx->step = 0;
    return PROCESSING;
}

enum State process_processing(struct Context *ctx) {
    if (ctx == NULL) return ERROR;
    if (ctx->current_state != PROCESSING) return ERROR;
    
    printf("State: PROCESSING - Step %d, Data: %d\n", ctx->step, ctx->data);
    
    if (ctx->step >= 5) {
        return FINISHED;
    }
    
    ctx->data = ctx->data * 2;
    if (ctx->data < 0) return ERROR;
    ctx->step++;
    
    return PROCESSING;
}

enum State process_finished(struct Context *ctx) {
    if (ctx == NULL) return ERROR;
    if (ctx->current_state != FINISHED) return ERROR;
    
    printf("State: FINISHED - Final result: %d\n", ctx->data);
    return FINISHED;
}

enum State process_error(struct Context *ctx) {
    if (ctx == NULL) return ERROR;
    printf("State: ERROR - Invalid state or input\n");
    return ERROR;
}

enum State state_machine(struct Context *ctx) {
    if (ctx == NULL) return ERROR;
    
    switch (ctx->current_state) {
        case START:
            ctx->current_state = process_start(ctx);
            break;
        case PROCESSING:
            ctx->current_state = process_processing(ctx);
            break;
        case FINISHED:
            ctx->current_state = process_finished(ctx);
            break;
        case ERROR:
            ctx->current_state = process_error(ctx);
            break;
        default:
            ctx->current_state = ERROR;
            break;
    }
    
    if (ctx->current_state != FINISHED && ctx->current_state != ERROR) {
        return state_machine(ctx);
    }
    
    return ctx->current_state;
}

int main(void) {
    struct Context ctx;
    ctx.current_state = START;
    ctx.data = 0;
    ctx.step = 0;
    
    enum State final_state = state_machine(&ctx);
    
    if (final_state == FINISHED) {
        printf("Processing completed successfully.\n");
        return EXIT_SUCCESS;
    } else {
        printf("Processing failed.\n");
        return EXIT_FAILURE;
    }
}