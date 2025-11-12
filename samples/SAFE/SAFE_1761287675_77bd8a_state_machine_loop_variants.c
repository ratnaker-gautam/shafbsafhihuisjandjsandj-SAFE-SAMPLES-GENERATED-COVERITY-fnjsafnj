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

static void state_init(struct context *ctx) {
    ctx->current_state = READING;
    ctx->buffer_len = 0;
    ctx->processed_count = 0;
    printf("State machine initialized. Enter up to %d items.\n", ctx->max_items);
}

static void state_reading(struct context *ctx) {
    if (ctx->processed_count >= ctx->max_items) {
        ctx->current_state = DONE;
        return;
    }
    
    printf("Enter item %d: ", ctx->processed_count + 1);
    
    if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) == NULL) {
        ctx->current_state = ERROR;
        return;
    }
    
    ctx->buffer_len = strlen(ctx->buffer);
    if (ctx->buffer_len > 0 && ctx->buffer[ctx->buffer_len - 1] == '\n') {
        ctx->buffer[--ctx->buffer_len] = '\0';
    }
    
    if (ctx->buffer_len == 0) {
        ctx->current_state = DONE;
        return;
    }
    
    ctx->current_state = PROCESSING;
}

static void state_processing(struct context *ctx) {
    size_t i = 0;
    while (i < ctx->buffer_len) {
        if (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') {
            ctx->buffer[i] = ctx->buffer[i] - 'a' + 'A';
        }
        i++;
    }
    ctx->current_state = OUTPUT;
}

static void state_output(struct context *ctx) {
    printf("Processed: %s\n", ctx->buffer);
    ctx->processed_count++;
    
    if (ctx->processed_count >= ctx->max_items) {
        ctx->current_state = DONE;
    } else {
        ctx->current_state = READING;
    }
}

static void state_done(struct context *ctx) {
    printf("Processing complete. %d items processed.\n", ctx->processed_count);
}

static void state_error(struct context *ctx) {
    printf("Error occurred during input.\n");
}

int main(void) {
    struct context ctx;
    ctx.max_items = 5;
    ctx.current_state = INIT;
    ctx.processed_count = 0;
    
    for (;;) {
        switch (ctx.current_state) {
            case INIT:
                state_init(&ctx);
                break;
            case READING:
                state_reading(&ctx);
                break;
            case PROCESSING:
                state_processing(&ctx);
                break;
            case OUTPUT:
                state_output(&ctx);
                break;
            case DONE:
                state_done(&ctx);
                return 0;
            case ERROR:
                state_error(&ctx);
                return 1;
        }
    }
    
    return 0;
}