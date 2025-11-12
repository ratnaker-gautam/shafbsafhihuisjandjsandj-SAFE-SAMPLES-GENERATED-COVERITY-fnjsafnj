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
    STATE_ERROR
} State;

typedef struct {
    State current_state;
    char buffer[MAX_INPUT_LEN];
    size_t buffer_len;
    int error_code;
} Context;

typedef State (*StateHandler)(Context*);

State handle_idle(Context* ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN - 1);
    
    if (fgets(ctx->buffer, MAX_INPUT_LEN, stdin) == NULL) {
        ctx->error_code = 1;
        return STATE_ERROR;
    }
    
    ctx->buffer_len = strlen(ctx->buffer);
    if (ctx->buffer_len > 0 && ctx->buffer[ctx->buffer_len - 1] == '\n') {
        ctx->buffer[ctx->buffer_len - 1] = '\0';
        ctx->buffer_len--;
    }
    
    if (ctx->buffer_len == 0) {
        return STATE_IDLE;
    }
    
    return STATE_READING;
}

State handle_reading(Context* ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    printf("Read %zu characters: '%s'\n", ctx->buffer_len, ctx->buffer);
    return STATE_PROCESSING;
}

State handle_processing(Context* ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    for (size_t i = 0; i < ctx->buffer_len; i++) {
        if (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') {
            ctx->buffer[i] = ctx->buffer[i] - 'a' + 'A';
        }
    }
    
    printf("Processed text: '%s'\n", ctx->buffer);
    return STATE_WRITING;
}

State handle_writing(Context* ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    printf("Writing completed.\n");
    memset(ctx->buffer, 0, MAX_INPUT_LEN);
    ctx->buffer_len = 0;
    return STATE_IDLE;
}

State handle_error(Context* ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    printf("Error occurred (code: %d)\n", ctx->error_code);
    ctx->error_code = 0;
    return STATE_IDLE;
}

StateHandler get_handler(State state) {
    static StateHandler handlers[] = {
        handle_idle,
        handle_reading,
        handle_processing,
        handle_writing,
        handle_error
    };
    
    if (state < 0 || state >= MAX_STATES) {
        return handle_error;
    }
    
    return handlers[state];
}

int main(void) {
    Context ctx;
    ctx.current_state = STATE_IDLE;
    ctx.buffer_len = 0;
    ctx.error_code = 0;
    memset(ctx.buffer, 0, MAX_INPUT_LEN);
    
    int iterations = 0;
    const int max_iterations = 10;
    
    while (iterations < max_iterations) {
        StateHandler handler = get_handler(ctx.current_state);
        if (handler == NULL) {
            break;
        }
        
        State next_state = handler(&ctx);
        if (next_state < 0 || next_state >= MAX_STATES) {
            ctx.current_state = STATE_ERROR;
        } else {
            ctx.current_state = next_state;
        }
        
        iterations++;
        
        if (ctx.current_state == STATE_IDLE && iterations >= 3) {
            printf("State machine completed successfully.\n");
            break;
        }
    }
    
    if (iterations >= max_iterations) {
        printf("State machine reached iteration limit.\n");
    }
    
    return 0;
}