//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_WRITING,
    STATE_ERROR,
    STATE_DONE
};

struct context {
    enum state current_state;
    char buffer[256];
    size_t buffer_len;
    int processed_count;
    int max_items;
};

void state_idle(struct context *ctx) {
    printf("Enter text to process (or 'quit' to exit): ");
    if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) == NULL) {
        ctx->current_state = STATE_ERROR;
        return;
    }
    
    ctx->buffer_len = strlen(ctx->buffer);
    if (ctx->buffer_len > 0 && ctx->buffer[ctx->buffer_len - 1] == '\n') {
        ctx->buffer[ctx->buffer_len - 1] = '\0';
        ctx->buffer_len--;
    }
    
    if (strcmp(ctx->buffer, "quit") == 0) {
        ctx->current_state = STATE_DONE;
    } else if (ctx->buffer_len > 0) {
        ctx->current_state = STATE_READING;
    }
}

void state_reading(struct context *ctx) {
    if (ctx->buffer_len == 0) {
        ctx->current_state = STATE_ERROR;
        return;
    }
    
    ctx->current_state = STATE_PROCESSING;
}

void state_processing(struct context *ctx) {
    if (ctx->buffer_len >= sizeof(ctx->buffer)) {
        ctx->current_state = STATE_ERROR;
        return;
    }
    
    for (size_t i = 0; i < ctx->buffer_len; i++) {
        if (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') {
            ctx->buffer[i] = ctx->buffer[i] - 'a' + 'A';
        }
    }
    
    ctx->processed_count++;
    ctx->current_state = STATE_WRITING;
}

void state_writing(struct context *ctx) {
    printf("Processed: %s\n", ctx->buffer);
    
    if (ctx->processed_count >= ctx->max_items) {
        ctx->current_state = STATE_DONE;
    } else {
        ctx->current_state = STATE_IDLE;
    }
}

void state_error(struct context *ctx) {
    printf("Error occurred during processing\n");
    ctx->current_state = STATE_DONE;
}

void state_done(struct context *ctx) {
    printf("Processing complete. Items processed: %d\n", ctx->processed_count);
}

int main(void) {
    struct context ctx;
    ctx.current_state = STATE_IDLE;
    ctx.buffer_len = 0;
    ctx.processed_count = 0;
    ctx.max_items = 10;
    
    memset(ctx.buffer, 0, sizeof(ctx.buffer));
    
    while (ctx.current_state != STATE_DONE) {
        switch (ctx.current_state) {
            case STATE_IDLE:
                state_idle(&ctx);
                break;
            case STATE_READING:
                state_reading(&ctx);
                break;
            case STATE_PROCESSING:
                state_processing(&ctx);
                break;
            case STATE_WRITING:
                state_writing(&ctx);
                break;
            case STATE_ERROR:
                state_error(&ctx);
                break;
            case STATE_DONE:
                break;
        }
    }
    
    return 0;
}