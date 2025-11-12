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

int validate_input(const char* input, size_t len) {
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

void state_idle(struct context* ctx) {
    printf("Enter data (or 'quit' to exit): ");
    if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) != NULL) {
        ctx->buffer_len = strlen(ctx->buffer);
        if (ctx->buffer_len > 0 && ctx->buffer[ctx->buffer_len - 1] == '\n') {
            ctx->buffer[--ctx->buffer_len] = '\0';
        }
        if (ctx->buffer_len == 0) {
            ctx->current_state = ERROR;
        } else if (strcmp(ctx->buffer, "quit") == 0) {
            ctx->current_state = DONE;
        } else {
            ctx->current_state = READING;
        }
    } else {
        ctx->current_state = ERROR;
    }
}

void state_reading(struct context* ctx) {
    if (!validate_input(ctx->buffer, ctx->buffer_len)) {
        ctx->current_state = ERROR;
        return;
    }
    ctx->current_state = PROCESSING;
}

void state_processing(struct context* ctx) {
    if (ctx->data_count >= ctx->max_items) {
        ctx->current_state = DONE;
        return;
    }
    for (size_t i = 0; i < ctx->buffer_len; i++) {
        if (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') {
            ctx->buffer[i] = ctx->buffer[i] - 'a' + 'A';
        }
    }
    ctx->data_count++;
    ctx->current_state = WRITING;
}

void state_writing(struct context* ctx) {
    printf("Processed: %s\n", ctx->buffer);
    ctx->current_state = IDLE;
}

void state_error(struct context* ctx) {
    printf("Error: Invalid input detected\n");
    ctx->current_state = IDLE;
}

int main(void) {
    struct context ctx;
    ctx.current_state = IDLE;
    ctx.buffer_len = 0;
    ctx.data_count = 0;
    ctx.max_items = 5;

    while (ctx.current_state != DONE && ctx.current_state != ERROR) {
        switch (ctx.current_state) {
            case IDLE:
                state_idle(&ctx);
                break;
            case READING:
                state_reading(&ctx);
                break;
            case PROCESSING:
                state_processing(&ctx);
                break;
            case WRITING:
                state_writing(&ctx);
                break;
            case ERROR:
                state_error(&ctx);
                break;
            default:
                ctx.current_state = ERROR;
                break;
        }
    }

    if (ctx.current_state == ERROR) {
        printf("Terminating due to error\n");
        return EXIT_FAILURE;
    }

    printf("Processed %d items successfully\n", ctx.data_count);
    return EXIT_SUCCESS;
}