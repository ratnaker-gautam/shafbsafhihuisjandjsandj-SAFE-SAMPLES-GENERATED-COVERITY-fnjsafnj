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
    ctx->max_items = 5;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
}

static void state_reading(struct context *ctx) {
    printf("Enter text (max 255 chars): ");
    
    if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) == NULL) {
        ctx->current_state = ERROR;
        return;
    }
    
    size_t len = strlen(ctx->buffer);
    if (len > 0 && ctx->buffer[len - 1] == '\n') {
        ctx->buffer[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        ctx->current_state = DONE;
        return;
    }
    
    ctx->buffer_len = len;
    ctx->current_state = PROCESSING;
}

static void state_processing(struct context *ctx) {
    size_t i = 0;
    size_t j = 0;
    char temp[256];
    
    while (i < ctx->buffer_len) {
        if (ctx->buffer[i] != ' ') {
            temp[j++] = ctx->buffer[i];
        }
        i++;
    }
    temp[j] = '\0';
    
    if (j > 0) {
        strncpy(ctx->buffer, temp, sizeof(ctx->buffer) - 1);
        ctx->buffer[sizeof(ctx->buffer) - 1] = '\0';
        ctx->buffer_len = j;
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

static void state_error(struct context *ctx) {
    printf("Error occurred during input\n");
    ctx->current_state = DONE;
}

static void state_done(struct context *ctx) {
    printf("Processing complete. Items processed: %d\n", ctx->processed_count);
}

int main(void) {
    struct context ctx;
    state_init(&ctx);
    
    while (1) {
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
            case ERROR:
                state_error(&ctx);
                break;
            case DONE:
                state_done(&ctx);
                return 0;
        }
    }
    
    return 0;
}