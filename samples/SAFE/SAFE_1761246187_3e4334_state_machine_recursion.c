//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State {
    START,
    PROCESSING,
    VALIDATING,
    COMPLETED,
    ERROR
};

struct Context {
    enum State current_state;
    int data;
    int step_count;
    char buffer[32];
};

enum State handle_start(struct Context *ctx) {
    if (ctx->step_count >= 10) return ERROR;
    printf("Enter initial value (0-100): ");
    if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) == NULL) return ERROR;
    int value;
    if (sscanf(ctx->buffer, "%d", &value) != 1) return ERROR;
    if (value < 0 || value > 100) return ERROR;
    ctx->data = value;
    ctx->step_count++;
    return PROCESSING;
}

enum State handle_processing(struct Context *ctx) {
    if (ctx->step_count >= 10) return ERROR;
    if (ctx->data < 0) return ERROR;
    ctx->data *= 2;
    if (ctx->data > 1000) return ERROR;
    ctx->step_count++;
    return VALIDATING;
}

enum State handle_validating(struct Context *ctx) {
    if (ctx->step_count >= 10) return ERROR;
    if (ctx->data % 2 != 0) return ERROR;
    printf("Current value: %d\n", ctx->data);
    printf("Continue? (y/n): ");
    if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) == NULL) return ERROR;
    if (strlen(ctx->buffer) == 0) return ERROR;
    if (ctx->buffer[0] == 'y' || ctx->buffer[0] == 'Y') {
        ctx->step_count++;
        return PROCESSING;
    } else if (ctx->buffer[0] == 'n' || ctx->buffer[0] == 'N') {
        ctx->step_count++;
        return COMPLETED;
    } else {
        return ERROR;
    }
}

enum State handle_completed(struct Context *ctx) {
    printf("Final result: %d\n", ctx->data);
    return COMPLETED;
}

enum State handle_error(struct Context *ctx) {
    printf("Error occurred at step %d\n", ctx->step_count);
    return ERROR;
}

enum State (*state_handlers[])(struct Context *) = {
    handle_start,
    handle_processing,
    handle_validating,
    handle_completed,
    handle_error
};

void run_state_machine(struct Context *ctx) {
    if (ctx == NULL) return;
    if (ctx->current_state < START || ctx->current_state > ERROR) return;
    
    enum State next_state = state_handlers[ctx->current_state](ctx);
    if (next_state < START || next_state > ERROR) return;
    
    if (next_state != ctx->current_state) {
        ctx->current_state = next_state;
        run_state_machine(ctx);
    }
}

int main(void) {
    struct Context ctx;
    ctx.current_state = START;
    ctx.data = 0;
    ctx.step_count = 0;
    memset(ctx.buffer, 0, sizeof(ctx.buffer));
    
    run_state_machine(&ctx);
    
    return ctx.current_state == COMPLETED ? EXIT_SUCCESS : EXIT_FAILURE;
}