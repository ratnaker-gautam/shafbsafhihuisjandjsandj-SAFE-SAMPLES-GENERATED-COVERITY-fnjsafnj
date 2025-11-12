//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

enum State {
    INIT,
    READY,
    PROCESSING,
    FINALIZING,
    DONE,
    ERROR
};

struct Context {
    enum State current_state;
    int data_value;
    int step_count;
    char buffer[64];
};

int validate_input(const char* input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > 63) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] < '0' || input[i] > '9') return 0;
    }
    return 1;
}

enum State handle_init(struct Context* ctx) {
    ctx->data_value = 0;
    ctx->step_count = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    return READY;
}

enum State handle_ready(struct Context* ctx, const char* input) {
    if (!validate_input(input)) return ERROR;
    strncpy(ctx->buffer, input, sizeof(ctx->buffer) - 1);
    ctx->buffer[sizeof(ctx->buffer) - 1] = '\0';
    return PROCESSING;
}

enum State handle_processing(struct Context* ctx) {
    int value = 0;
    for (char* p = ctx->buffer; *p != '\0'; p++) {
        if (value > INT_MAX / 10) return ERROR;
        value = value * 10 + (*p - '0');
        if (value < 0) return ERROR;
    }
    ctx->data_value = value;
    ctx->step_count = 0;
    return FINALIZING;
}

enum State handle_finalizing(struct Context* ctx) {
    int i = 0;
    while (i < 5) {
        if (ctx->step_count > INT_MAX - ctx->data_value) return ERROR;
        ctx->data_value += ctx->step_count;
        if (ctx->step_count == INT_MAX) return ERROR;
        ctx->step_count++;
        i++;
    }
    return DONE;
}

enum State handle_done(struct Context* ctx) {
    printf("Final result: %d\n", ctx->data_value);
    return DONE;
}

enum State handle_error(struct Context* ctx) {
    printf("Error occurred during processing\n");
    return ERROR;
}

int main(void) {
    struct Context ctx;
    ctx.current_state = INIT;
    
    char input[128];
    int input_valid = 0;
    
    do {
        switch (ctx.current_state) {
            case INIT:
                ctx.current_state = handle_init(&ctx);
                break;
            case READY:
                printf("Enter a numeric string: ");
                if (fgets(input, sizeof(input), stdin) != NULL) {
                    size_t len = strlen(input);
                    if (len > 0 && input[len - 1] == '\n') {
                        input[len - 1] = '\0';
                    }
                    ctx.current_state = handle_ready(&ctx, input);
                } else {
                    ctx.current_state = ERROR;
                }
                break;
            case PROCESSING:
                ctx.current_state = handle_processing(&ctx);
                break;
            case FINALIZING:
                ctx.current_state = handle_finalizing(&ctx);
                break;
            case DONE:
                ctx.current_state = handle_done(&ctx);
                break;
            case ERROR:
                ctx.current_state = handle_error(&ctx);
                break;
        }
    } while (ctx.current_state != DONE && ctx.current_state != ERROR);
    
    return ctx.current_state == DONE ? EXIT_SUCCESS : EXIT_FAILURE;
}