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

enum State handle_start(struct Context *ctx, int input);
enum State handle_processing(struct Context *ctx, int input);
enum State handle_validating(struct Context *ctx, int input);
enum State handle_completed(struct Context *ctx, int input);
enum State handle_error(struct Context *ctx, int input);

enum State transition_state(struct Context *ctx, int input) {
    switch (ctx->current_state) {
        case START:
            return handle_start(ctx, input);
        case PROCESSING:
            return handle_processing(ctx, input);
        case VALIDATING:
            return handle_validating(ctx, input);
        case COMPLETED:
            return handle_completed(ctx, input);
        case ERROR:
            return handle_error(ctx, input);
        default:
            return ERROR;
    }
}

enum State handle_start(struct Context *ctx, int input) {
    if (input < 0 || input > 100) {
        return ERROR;
    }
    ctx->data_value = input;
    ctx->step_count = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    return PROCESSING;
}

enum State handle_processing(struct Context *ctx, int input) {
    if (input < 0 || input > 50) {
        return ERROR;
    }
    if (ctx->step_count >= 10) {
        return ERROR;
    }
    ctx->data_value += input;
    ctx->step_count++;
    if (ctx->step_count >= 3) {
        return VALIDATING;
    }
    return PROCESSING;
}

enum State handle_validating(struct Context *ctx, int input) {
    if (input != 0 && input != 1) {
        return ERROR;
    }
    if (input == 1) {
        if (ctx->data_value > 200) {
            return ERROR;
        }
        int len = snprintf(ctx->buffer, sizeof(ctx->buffer), "Result: %d", ctx->data_value);
        if (len < 0 || len >= (int)sizeof(ctx->buffer)) {
            return ERROR;
        }
        return COMPLETED;
    } else {
        ctx->data_value = 0;
        ctx->step_count = 0;
        return START;
    }
}

enum State handle_completed(struct Context *ctx, int input) {
    if (input == 0) {
        return START;
    }
    return COMPLETED;
}

enum State handle_error(struct Context *ctx, int input) {
    if (input == 0) {
        return START;
    }
    return ERROR;
}

void process_state_machine(struct Context *ctx, int input) {
    enum State new_state = transition_state(ctx, input);
    ctx->current_state = new_state;
}

void recursive_state_processor(struct Context *ctx, int depth) {
    if (depth <= 0) {
        return;
    }
    if (ctx->current_state == COMPLETED || ctx->current_state == ERROR) {
        return;
    }
    int input;
    printf("Current state: %d, Enter input: ", ctx->current_state);
    if (scanf("%d", &input) != 1) {
        ctx->current_state = ERROR;
        return;
    }
    process_state_machine(ctx, input);
    recursive_state_processor(ctx, depth - 1);
}

int main() {
    struct Context ctx;
    ctx.current_state = START;
    ctx.data_value = 0;
    ctx.step_count = 0;
    memset(ctx.buffer, 0, sizeof(ctx.buffer));
    
    printf("State Machine Demo (max 10 steps)\n");
    recursive_state_processor(&ctx, 10);
    
    if (ctx.current_state == COMPLETED) {
        printf("%s\n", ctx.buffer);
    } else if (ctx.current_state == ERROR) {
        printf("Operation failed\n");
    } else {
        printf("Operation incomplete\n");
    }
    
    return 0;
}