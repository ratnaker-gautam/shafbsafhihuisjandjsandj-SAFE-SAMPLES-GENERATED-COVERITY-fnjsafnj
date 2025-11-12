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
    
    printf("Read %zu characters\n", ctx->buffer_len);
    return STATE_PROCESSING;
}

State handle_processing(Context* ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    for (size_t i = 0; i < ctx->buffer_len; i++) {
        if (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') {
            ctx->buffer[i] = ctx->buffer[i] - 'a' + 'A';
        }
    }
    
    printf("Processed text: %s\n", ctx->buffer);
    return STATE_WRITING;
}

State handle_writing(Context* ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    printf("Data written successfully\n");
    return STATE_IDLE;
}

State handle_error(Context* ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    printf("Error occurred (code: %d)\n", ctx->error_code);
    ctx->error_code = 0;
    return STATE_IDLE;
}

StateHandler state_handlers[MAX_STATES] = {
    handle_idle,
    handle_reading,
    handle_processing,
    handle_writing,
    handle_error
};

int main(void) {
    Context ctx;
    ctx.current_state = STATE_IDLE;
    ctx.buffer[0] = '\0';
    ctx.buffer_len = 0;
    ctx.error_code = 0;
    
    int max_iterations = 100;
    int iteration = 0;
    
    while (ctx.current_state != STATE_ERROR && iteration < max_iterations) {
        if (ctx.current_state < 0 || ctx.current_state >= MAX_STATES) {
            break;
        }
        
        StateHandler handler = state_handlers[ctx.current_state];
        if (handler == NULL) {
            break;
        }
        
        State next_state = handler(&ctx);
        
        if (next_state < 0 || next_state >= MAX_STATES) {
            break;
        }
        
        ctx.current_state = next_state;
        iteration++;
    }
    
    if (iteration >= max_iterations) {
        printf("State machine terminated: maximum iterations reached\n");
    }
    
    return 0;
}