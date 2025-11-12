//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State { START, PROCESSING, VALIDATING, FINISHED, ERROR };

struct Context {
    enum State current_state;
    int data_value;
    int step_count;
    char buffer[32];
};

enum State handle_start(struct Context* ctx, const char* input) {
    if (input == NULL || strlen(input) == 0) {
        return ERROR;
    }
    
    if (strlen(input) >= sizeof(ctx->buffer)) {
        return ERROR;
    }
    
    strncpy(ctx->buffer, input, sizeof(ctx->buffer) - 1);
    ctx->buffer[sizeof(ctx->buffer) - 1] = '\0';
    ctx->data_value = 0;
    ctx->step_count = 0;
    return PROCESSING;
}

enum State handle_processing(struct Context* ctx) {
    if (ctx->step_count >= 10) {
        return ERROR;
    }
    
    for (int i = 0; ctx->buffer[i] != '\0'; i++) {
        if (ctx->buffer[i] >= '0' && ctx->buffer[i] <= '9') {
            ctx->data_value += (ctx->buffer[i] - '0');
        }
    }
    
    ctx->step_count++;
    return VALIDATING;
}

enum State handle_validating(struct Context* ctx) {
    if (ctx->data_value < 0) {
        return ERROR;
    }
    
    if (ctx->data_value > 100) {
        ctx->data_value = ctx->data_value % 100;
    }
    
    if (ctx->data_value == 0) {
        return ERROR;
    }
    
    return FINISHED;
}

enum State handle_finished(struct Context* ctx) {
    printf("Processing completed. Final value: %d\n", ctx->data_value);
    return FINISHED;
}

enum State handle_error(struct Context* ctx) {
    printf("Error occurred during processing.\n");
    return ERROR;
}

enum State process_state(struct Context* ctx, const char* input) {
    switch (ctx->current_state) {
        case START:
            return handle_start(ctx, input);
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

void run_state_machine(struct Context* ctx, const char* input, int depth) {
    if (depth > 20) {
        ctx->current_state = ERROR;
        return;
    }
    
    enum State next_state = process_state(ctx, input);
    
    if (next_state == ctx->current_state) {
        return;
    }
    
    ctx->current_state = next_state;
    
    if (ctx->current_state == PROCESSING || ctx->current_state == VALIDATING) {
        run_state_machine(ctx, NULL, depth + 1);
    }
}

int main() {
    char user_input[64];
    struct Context ctx;
    
    printf("Enter a string to process: ");
    if (fgets(user_input, sizeof(user_input), stdin) == NULL) {
        printf("Input error.\n");
        return 1;
    }
    
    size_t len = strlen(user_input);
    if (len > 0 && user_input[len - 1] == '\n') {
        user_input[len - 1] = '\0';
    }
    
    if (strlen(user_input) == 0) {
        printf("Empty input.\n");
        return 1;
    }
    
    ctx.current_state = START;
    ctx.data_value = 0;
    ctx.step_count = 0;
    memset(ctx.buffer, 0, sizeof(ctx.buffer));
    
    run_state_machine(&ctx, user_input, 0);
    
    if (ctx.current_state == FINISHED) {
        printf("State machine completed successfully.\n");
    } else {
        printf("State machine terminated with errors.\n");
    }
    
    return 0;
}