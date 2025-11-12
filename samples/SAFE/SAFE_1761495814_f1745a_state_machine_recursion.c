//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State { START, PROCESSING, VALIDATING, COMPLETED, ERROR };

struct Context {
    enum State current_state;
    int data_value;
    int step_count;
    char buffer[32];
};

enum State handle_start(struct Context *ctx) {
    if (ctx->step_count >= 10) return ERROR;
    printf("Enter initial value (0-100): ");
    if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) == NULL) return ERROR;
    char *endptr;
    long val = strtol(ctx->buffer, &endptr, 10);
    if (endptr == ctx->buffer || *endptr != '\n') return ERROR;
    if (val < 0 || val > 100) return ERROR;
    ctx->data_value = (int)val;
    ctx->step_count++;
    return PROCESSING;
}

enum State handle_processing(struct Context *ctx) {
    if (ctx->step_count >= 10) return ERROR;
    if (ctx->data_value < 0) return ERROR;
    ctx->data_value *= 2;
    if (ctx->data_value > 1000) ctx->data_value = 1000;
    ctx->step_count++;
    return VALIDATING;
}

enum State handle_validating(struct Context *ctx) {
    if (ctx->step_count >= 10) return ERROR;
    if (ctx->data_value % 2 != 0) return ERROR;
    printf("Validated value: %d\n", ctx->data_value);
    ctx->step_count++;
    return COMPLETED;
}

enum State handle_completed(struct Context *ctx) {
    printf("Processing completed successfully.\n");
    return COMPLETED;
}

enum State handle_error(struct Context *ctx) {
    printf("Error occurred during processing.\n");
    return ERROR;
}

void state_machine(struct Context *ctx) {
    if (ctx == NULL) return;
    if (ctx->step_count > 10) {
        ctx->current_state = ERROR;
        return;
    }
    
    enum State next_state;
    switch (ctx->current_state) {
        case START:
            next_state = handle_start(ctx);
            break;
        case PROCESSING:
            next_state = handle_processing(ctx);
            break;
        case VALIDATING:
            next_state = handle_validating(ctx);
            break;
        case COMPLETED:
            next_state = handle_completed(ctx);
            break;
        case ERROR:
            next_state = handle_error(ctx);
            break;
        default:
            next_state = ERROR;
            break;
    }
    
    if (next_state != ctx->current_state) {
        ctx->current_state = next_state;
        state_machine(ctx);
    }
}

int main(void) {
    struct Context ctx;
    ctx.current_state = START;
    ctx.data_value = 0;
    ctx.step_count = 0;
    memset(ctx.buffer, 0, sizeof(ctx.buffer));
    
    state_machine(&ctx);
    
    if (ctx.current_state == COMPLETED) {
        return 0;
    } else {
        return 1;
    }
}