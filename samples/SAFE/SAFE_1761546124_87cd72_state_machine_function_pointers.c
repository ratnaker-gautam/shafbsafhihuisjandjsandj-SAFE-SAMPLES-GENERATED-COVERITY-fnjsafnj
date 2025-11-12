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
    size_t buffer_len;
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
    if (len > 0 && ctx->buffer[len-1] == '\n') {
        ctx->buffer[len-1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("Empty input, try again.\n");
        return;
    }
    
    if (strcmp(ctx->buffer, "quit") == 0) {
        ctx->current_state = STATE_DONE;
        return;
    }
    
    ctx->buffer_len = len;
    ctx->current_state = STATE_READING;
}

void handle_reading(struct context* ctx) {
    if (ctx == NULL) return;
    
    if (ctx->buffer_len == 0 || ctx->buffer_len >= sizeof(ctx->buffer)) {
        ctx->error_code = 2;
        ctx->current_state = STATE_ERROR;
        return;
    }
    
    printf("Read %zu characters: '%s'\n", ctx->buffer_len, ctx->buffer);
    ctx->current_state = STATE_PROCESSING;
}

void handle_processing(struct context* ctx) {
    if (ctx == NULL) return;
    
    if (ctx->buffer_len == 0) {
        ctx->error_code = 3;
        ctx->current_state = STATE_ERROR;
        return;
    }
    
    for (size_t i = 0; i < ctx->buffer_len; i++) {
        if (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') {
            ctx->buffer[i] = ctx->buffer[i] - 'a' + 'A';
        }
    }
    
    printf("Processed to uppercase: '%s'\n", ctx->buffer);
    ctx->current_state = STATE_WRITING;
}

void handle_writing(struct context* ctx) {
    if (ctx == NULL) return;
    
    if (ctx->buffer_len == 0) {
        ctx->error_code = 4;
        ctx->current_state = STATE_ERROR;
        return;
    }
    
    printf("Output written: '%s'\n", ctx->buffer);
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    ctx->buffer_len = 0;
    ctx->current_state = STATE_IDLE;
}

void handle_error(struct context* ctx) {
    if (ctx == NULL) return;
    
    printf("Error %d occurred. Resetting.\n", ctx->error_code);
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    ctx->buffer_len = 0;
    ctx->error_code = 0;
    ctx->current_state = STATE_IDLE;
}

void handle_done(struct context* ctx) {
    if (ctx == NULL) return;
    printf("State machine terminated.\n");
}

int main(void) {
    struct context ctx;
    memset(&ctx, 0, sizeof(ctx));
    ctx.current_state = STATE_IDLE;
    
    state_handler handlers[] = {
        handle_idle,
        handle_reading,
        handle_processing,
        handle_writing,
        handle_error,
        handle_done
    };
    
    while (ctx.current_state != STATE_DONE) {
        if (ctx.current_state < 0 || ctx.current_state >= (int)(sizeof(handlers)/sizeof(handlers[0]))) {
            printf("Invalid state, terminating.\n");
            break;
        }
        
        handlers[ctx.current_state](&ctx);
    }
    
    return 0;
}