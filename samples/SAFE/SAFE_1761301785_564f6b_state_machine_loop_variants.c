//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

enum state {
    IDLE,
    READING,
    PROCESSING,
    WRITING,
    ERROR,
    DONE
};

struct context {
    enum state current_state;
    char buffer[256];
    size_t buffer_len;
    int data_count;
    int max_items;
};

static int validate_input(const char *input, size_t len) {
    if (input == NULL || len == 0 || len >= 256) {
        return 0;
    }
    for (size_t i = 0; i < len; i++) {
        if (input[i] < 32 || input[i] > 126) {
            return 0;
        }
    }
    return 1;
}

static enum state handle_idle(struct context *ctx) {
    printf("Enter data (max 255 chars): ");
    if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) == NULL) {
        return ERROR;
    }
    ctx->buffer_len = strlen(ctx->buffer);
    if (ctx->buffer_len > 0 && ctx->buffer[ctx->buffer_len - 1] == '\n') {
        ctx->buffer[ctx->buffer_len - 1] = '\0';
        ctx->buffer_len--;
    }
    if (!validate_input(ctx->buffer, ctx->buffer_len)) {
        return ERROR;
    }
    return READING;
}

static enum state handle_reading(struct context *ctx) {
    if (ctx->data_count >= ctx->max_items) {
        return DONE;
    }
    ctx->data_count++;
    return PROCESSING;
}

static enum state handle_processing(struct context *ctx) {
    for (size_t i = 0; i < ctx->buffer_len; i++) {
        if (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') {
            ctx->buffer[i] = ctx->buffer[i] - 'a' + 'A';
        }
    }
    return WRITING;
}

static enum state handle_writing(struct context *ctx) {
    printf("Processed: %s\n", ctx->buffer);
    return IDLE;
}

static enum state handle_error(struct context *ctx) {
    printf("Invalid input detected\n");
    ctx->buffer[0] = '\0';
    ctx->buffer_len = 0;
    return IDLE;
}

int main(void) {
    struct context ctx;
    ctx.current_state = IDLE;
    ctx.buffer[0] = '\0';
    ctx.buffer_len = 0;
    ctx.data_count = 0;
    ctx.max_items = 5;

    while (ctx.current_state != DONE && ctx.current_state != ERROR) {
        switch (ctx.current_state) {
            case IDLE:
                ctx.current_state = handle_idle(&ctx);
                break;
            case READING:
                ctx.current_state = handle_reading(&ctx);
                break;
            case PROCESSING:
                ctx.current_state = handle_processing(&ctx);
                break;
            case WRITING:
                ctx.current_state = handle_writing(&ctx);
                break;
            case ERROR:
                ctx.current_state = handle_error(&ctx);
                break;
            default:
                ctx.current_state = ERROR;
                break;
        }
    }

    if (ctx.current_state == DONE) {
        printf("Processed %d items successfully\n", ctx.data_count);
    } else {
        printf("Terminated with errors\n");
    }

    return ctx.current_state == DONE ? EXIT_SUCCESS : EXIT_FAILURE;
}