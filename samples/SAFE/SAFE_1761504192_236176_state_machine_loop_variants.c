//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

enum state {
    INIT,
    READING,
    PROCESSING,
    OUTPUT,
    DONE,
    ERROR
};

struct context {
    enum state current_state;
    char buffer[256];
    size_t buffer_len;
    int processed_count;
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

static void process_buffer(struct context *ctx) {
    if (ctx->buffer_len > 0) {
        for (size_t i = 0; i < ctx->buffer_len; i++) {
            if (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') {
                ctx->buffer[i] = ctx->buffer[i] - 'a' + 'A';
            }
        }
    }
}

static enum state handle_init(struct context *ctx) {
    printf("Enter maximum number of items to process (1-10): ");
    char input[16];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return ERROR;
    }
    int num = atoi(input);
    if (num < 1 || num > 10) {
        return ERROR;
    }
    ctx->max_items = num;
    ctx->processed_count = 0;
    return READING;
}

static enum state handle_reading(struct context *ctx) {
    printf("Enter text (empty line to finish): ");
    if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) == NULL) {
        return ERROR;
    }
    size_t len = strlen(ctx->buffer);
    if (len > 0 && ctx->buffer[len - 1] == '\n') {
        ctx->buffer[len - 1] = '\0';
        len--;
    }
    if (len == 0) {
        return DONE;
    }
    if (!validate_input(ctx->buffer, len)) {
        return ERROR;
    }
    ctx->buffer_len = len;
    return PROCESSING;
}

static enum state handle_processing(struct context *ctx) {
    process_buffer(ctx);
    ctx->processed_count++;
    return OUTPUT;
}

static enum state handle_output(struct context *ctx) {
    printf("Processed: %s\n", ctx->buffer);
    if (ctx->processed_count >= ctx->max_items) {
        return DONE;
    }
    return READING;
}

static enum state handle_error(struct context *ctx) {
    printf("Error occurred. Exiting.\n");
    return DONE;
}

int main(void) {
    struct context ctx = {INIT, {0}, 0, 0, 0};
    
    while (1) {
        switch (ctx.current_state) {
            case INIT:
                ctx.current_state = handle_init(&ctx);
                break;
            case READING:
                ctx.current_state = handle_reading(&ctx);
                break;
            case PROCESSING:
                ctx.current_state = handle_processing(&ctx);
                break;
            case OUTPUT:
                ctx.current_state = handle_output(&ctx);
                break;
            case ERROR:
                ctx.current_state = handle_error(&ctx);
                break;
            case DONE:
                printf("Processing complete. %d items processed.\n", ctx.processed_count);
                return 0;
        }
    }
    
    return 0;
}