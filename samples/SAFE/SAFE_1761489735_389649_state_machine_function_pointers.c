//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 5

typedef enum {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_WRITING,
    STATE_EXIT
} State;

typedef struct {
    State current_state;
    char buffer[MAX_INPUT_LEN];
    int buffer_len;
} Context;

typedef State (*StateHandler)(Context*);

State handle_idle(Context* ctx) {
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN - 1);
    if (fgets(ctx->buffer, MAX_INPUT_LEN, stdin) == NULL) {
        return STATE_EXIT;
    }
    
    ctx->buffer_len = strlen(ctx->buffer);
    if (ctx->buffer_len > 0 && ctx->buffer[ctx->buffer_len - 1] == '\n') {
        ctx->buffer[ctx->buffer_len - 1] = '\0';
        ctx->buffer_len--;
    }
    
    if (ctx->buffer_len == 0) {
        return STATE_EXIT;
    }
    
    return STATE_READING;
}

State handle_reading(Context* ctx) {
    if (ctx->buffer_len <= 0 || ctx->buffer_len >= MAX_INPUT_LEN) {
        return STATE_EXIT;
    }
    
    printf("Read %d characters\n", ctx->buffer_len);
    return STATE_PROCESSING;
}

State handle_processing(Context* ctx) {
    if (ctx->buffer_len <= 0 || ctx->buffer_len >= MAX_INPUT_LEN) {
        return STATE_EXIT;
    }
    
    for (int i = 0; i < ctx->buffer_len; i++) {
        if (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') {
            ctx->buffer[i] = ctx->buffer[i] - 'a' + 'A';
        }
    }
    
    printf("Processed text: %s\n", ctx->buffer);
    return STATE_WRITING;
}

State handle_writing(Context* ctx) {
    printf("Writing completed. Starting over.\n");
    return STATE_IDLE;
}

State handle_exit(Context* ctx) {
    printf("Exiting state machine\n");
    return STATE_EXIT;
}

int main() {
    Context ctx;
    ctx.current_state = STATE_IDLE;
    ctx.buffer_len = 0;
    memset(ctx.buffer, 0, MAX_INPUT_LEN);
    
    StateHandler handlers[MAX_STATES] = {
        handle_idle,
        handle_reading,
        handle_processing,
        handle_writing,
        handle_exit
    };
    
    while (ctx.current_state != STATE_EXIT) {
        if (ctx.current_state < 0 || ctx.current_state >= MAX_STATES) {
            break;
        }
        
        StateHandler handler = handlers[ctx.current_state];
        if (handler == NULL) {
            break;
        }
        
        ctx.current_state = handler(&ctx);
    }
    
    return 0;
}