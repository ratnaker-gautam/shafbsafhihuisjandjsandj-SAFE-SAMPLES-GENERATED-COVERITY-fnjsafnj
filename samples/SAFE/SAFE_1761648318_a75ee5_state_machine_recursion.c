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
    int value;
    int step;
    char buffer[32];
};

enum State process_start(struct Context *ctx);
enum State process_processing(struct Context *ctx);
enum State process_validating(struct Context *ctx);
enum State process_completed(struct Context *ctx);
enum State process_error(struct Context *ctx);

enum State state_machine(enum State current, struct Context *ctx) {
    switch (current) {
        case START:
            return process_start(ctx);
        case PROCESSING:
            return process_processing(ctx);
        case VALIDATING:
            return process_validating(ctx);
        case COMPLETED:
            return process_completed(ctx);
        case ERROR:
            return process_error(ctx);
        default:
            return ERROR;
    }
}

enum State process_start(struct Context *ctx) {
    printf("Enter initial value (0-100): ");
    if (scanf("%d", &ctx->value) != 1) {
        return ERROR;
    }
    if (ctx->value < 0 || ctx->value > 100) {
        return ERROR;
    }
    ctx->step = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    return PROCESSING;
}

enum State process_processing(struct Context *ctx) {
    if (ctx->step >= 10) {
        return VALIDATING;
    }
    ctx->value += ctx->step * 2;
    if (ctx->value > 1000) {
        return ERROR;
    }
    ctx->step++;
    return PROCESSING;
}

enum State process_validating(struct Context *ctx) {
    if (ctx->value < 0 || ctx->value > 1000) {
        return ERROR;
    }
    int len = snprintf(ctx->buffer, sizeof(ctx->buffer), "%d", ctx->value);
    if (len < 0 || len >= (int)sizeof(ctx->buffer)) {
        return ERROR;
    }
    return COMPLETED;
}

enum State process_completed(struct Context *ctx) {
    printf("Processing completed. Final value: %d\n", ctx->value);
    printf("String representation: %s\n", ctx->buffer);
    return COMPLETED;
}

enum State process_error(struct Context *ctx) {
    printf("Error occurred during processing.\n");
    return ERROR;
}

void run_state_machine(enum State current, struct Context *ctx, int depth) {
    if (depth > 100) {
        printf("State machine depth limit exceeded.\n");
        return;
    }
    enum State next = state_machine(current, ctx);
    if (next != current) {
        run_state_machine(next, ctx, depth + 1);
    }
}

int main() {
    struct Context ctx;
    run_state_machine(START, &ctx, 0);
    return 0;
}