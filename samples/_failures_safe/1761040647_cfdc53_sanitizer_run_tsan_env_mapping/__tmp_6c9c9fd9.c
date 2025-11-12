//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State { START, PROCESSING, FINISHED, ERROR };

struct Context {
    enum State current_state;
    int data;
    int step_count;
};

enum State process_start(struct Context *ctx);
enum State process_processing(struct Context *ctx);
enum State process_finished(struct Context *ctx);
enum State process_error(struct Context *ctx);

enum State handle_state(struct Context *ctx) {
    if (ctx == NULL) return ERROR;
    
    switch (ctx->current_state) {
        case START:
            return process_start(ctx);
        case PROCESSING:
            return process_processing(ctx);
        case FINISHED:
            return process_finished(ctx);
        case ERROR:
            return process_error(ctx);
        default:
            return ERROR;
    }
}

enum State process_start(struct Context *ctx) {
    if (ctx == NULL) return ERROR;
    
    printf("Starting processing...\n");
    ctx->current_state = PROCESSING;
    ctx->data = 0;
    ctx->step_count = 0;
    
    return handle_state(ctx);
}

enum State process_processing(struct Context *ctx) {
    if (ctx == NULL) return ERROR;
    
    if (ctx->step_count >= 5) {
        ctx->current_state = FINISHED;
        return handle_state(ctx);
    }
    
    printf("Processing step %d\n", ctx->step_count + 1);
    ctx->data += (ctx->step_count + 1) * 10;
    ctx->step_count++;
    
    if (ctx->data > 1000) {
        ctx->current_state = ERROR;
        return handle_state(ctx);
    }
    
    return handle_state(ctx);
}

enum State process_finished(struct Context *ctx) {
    if (ctx == NULL) return ERROR;
    
    printf("Processing finished. Final data: %d\n", ctx->data);
    return FINISHED;
}

enum State process_error(struct Context *ctx) {
    if (ctx == NULL) return ERROR;
    
    printf("Error occurred during processing.\n");
    return ERROR;
}

int main(void) {
    struct Context ctx;
    ctx.current_state = START;
    ctx.data = 0;
    ctx.step_count = 0;
    
    enum State final_state = handle_state(&ctx);
    
    if (final_state == FINISHED) {
        printf("State machine completed successfully.\n");
        return EXIT_SUCCESS;
    } else {
        printf("State machine terminated with error.\n");
        return EXIT_FAILURE;
    }
}