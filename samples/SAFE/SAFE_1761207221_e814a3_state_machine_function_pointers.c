//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State {
    INIT,
    READING,
    PROCESSING,
    WRITING,
    DONE,
    ERROR
};

struct Context {
    enum State current_state;
    char buffer[256];
    size_t buffer_len;
    int processed_count;
};

typedef void (*StateHandler)(struct Context*);

void handle_init(struct Context* ctx) {
    if (ctx == NULL) return;
    
    ctx->buffer_len = 0;
    ctx->processed_count = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    
    printf("Enter text (max 255 chars): ");
    if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) != NULL) {
        ctx->buffer_len = strlen(ctx->buffer);
        if (ctx->buffer_len > 0 && ctx->buffer[ctx->buffer_len - 1] == '\n') {
            ctx->buffer[ctx->buffer_len - 1] = '\0';
            ctx->buffer_len--;
        }
        ctx->current_state = READING;
    } else {
        ctx->current_state = ERROR;
    }
}

void handle_reading(struct Context* ctx) {
    if (ctx == NULL) return;
    
    if (ctx->buffer_len == 0) {
        ctx->current_state = DONE;
        return;
    }
    
    if (ctx->buffer_len > sizeof(ctx->buffer) - 1) {
        ctx->current_state = ERROR;
        return;
    }
    
    ctx->current_state = PROCESSING;
}

void handle_processing(struct Context* ctx) {
    if (ctx == NULL || ctx->buffer_len >= sizeof(ctx->buffer)) return;
    
    for (size_t i = 0; i < ctx->buffer_len; i++) {
        if (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') {
            ctx->buffer[i] = ctx->buffer[i] - 'a' + 'A';
        }
    }
    
    ctx->processed_count++;
    ctx->current_state = WRITING;
}

void handle_writing(struct Context* ctx) {
    if (ctx == NULL) return;
    
    printf("Processed text: %s\n", ctx->buffer);
    printf("Characters processed: %zu\n", ctx->buffer_len);
    
    ctx->current_state = DONE;
}

void handle_done(struct Context* ctx) {
    if (ctx == NULL) return;
    
    printf("Processing complete. Total operations: %d\n", ctx->processed_count);
    ctx->current_state = DONE;
}

void handle_error(struct Context* ctx) {
    if (ctx == NULL) return;
    
    printf("Error occurred during processing.\n");
    ctx->current_state = DONE;
}

int main(void) {
    struct Context ctx;
    ctx.current_state = INIT;
    ctx.buffer_len = 0;
    ctx.processed_count = 0;
    
    StateHandler handlers[] = {
        handle_init,
        handle_reading,
        handle_processing,
        handle_writing,
        handle_done,
        handle_error
    };
    
    while (ctx.current_state != DONE) {
        if (ctx.current_state >= INIT && ctx.current_state <= ERROR) {
            handlers[ctx.current_state](&ctx);
        } else {
            ctx.current_state = ERROR;
        }
    }
    
    return 0;
}