//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    size_t buffer_pos;
    int error_code;
};

typedef void (*state_handler)(struct context*);

void handle_idle(struct context* ctx) {
    if (ctx == NULL) return;
    
    printf("Enter text (max 255 chars, 'quit' to exit): ");
    if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) == NULL) {
        ctx->error_code = 1;
        ctx->current_state = STATE_ERROR;
        return;
    }
    
    size_t len = strlen(ctx->buffer);
    if (len > 0 && ctx->buffer[len - 1] == '\n') {
        ctx->buffer[len - 1] = '\0';
    }
    
    if (strcmp(ctx->buffer, "quit") == 0) {
        ctx->current_state = STATE_DONE;
    } else if (strlen(ctx->buffer) > 0) {
        ctx->buffer_pos = 0;
        ctx->current_state = STATE_READING;
    }
}

void handle_reading(struct context* ctx) {
    if (ctx == NULL) return;
    
    size_t len = strlen(ctx->buffer);
    if (ctx->buffer_pos >= len) {
        ctx->current_state = STATE_PROCESSING;
        return;
    }
    
    ctx->buffer_pos++;
    if (ctx->buffer_pos >= len) {
        ctx->current_state = STATE_PROCESSING;
    }
}

void handle_processing(struct context* ctx) {
    if (ctx == NULL) return;
    
    size_t len = strlen(ctx->buffer);
    for (size_t i = 0; i < len; i++) {
        if (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') {
            ctx->buffer[i] = ctx->buffer[i] - 'a' + 'A';
        }
    }
    
    ctx->current_state = STATE_WRITING;
}

void handle_writing(struct context* ctx) {
    if (ctx == NULL) return;
    
    printf("Processed: %s\n", ctx->buffer);
    ctx->current_state = STATE_IDLE;
}

void handle_error(struct context* ctx) {
    if (ctx == NULL) return;
    
    printf("Error occurred (code: %d)\n", ctx->error_code);
    ctx->current_state = STATE_DONE;
}

void handle_done(struct context* ctx) {
    if (ctx == NULL) return;
    printf("State machine terminated.\n");
}

int main(void) {
    struct context ctx;
    ctx.current_state = STATE_IDLE;
    ctx.buffer[0] = '\0';
    ctx.buffer_pos = 0;
    ctx.error_code = 0;
    
    state_handler handlers[] = {
        handle_idle,
        handle_reading,
        handle_processing,
        handle_writing,
        handle_error,
        handle_done
    };
    
    if (sizeof(handlers) / sizeof(handlers[0]) != STATE_DONE + 1) {
        return EXIT_FAILURE;
    }
    
    while (ctx.current_state != STATE_DONE) {
        if (ctx.current_state < 0 || ctx.current_state > STATE_DONE) {
            ctx.current_state = STATE_ERROR;
        }
        
        handlers[ctx.current_state](&ctx);
    }
    
    return EXIT_SUCCESS;
}