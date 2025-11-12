//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_WRITING,
    STATE_ERROR,
    STATE_COUNT
} state_t;

typedef struct {
    state_t current_state;
    char buffer[256];
    size_t buffer_pos;
    int error_code;
} context_t;

typedef state_t (*state_handler_t)(context_t*);

state_t handle_idle(context_t* ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    printf("Enter command (r=read, q=quit): ");
    char input[16];
    if (fgets(input, sizeof(input), stdin) == NULL) return STATE_ERROR;
    
    if (strlen(input) > 0 && input[strlen(input)-1] == '\n') {
        input[strlen(input)-1] = '\0';
    }
    
    if (strcmp(input, "r") == 0) {
        ctx->buffer_pos = 0;
        memset(ctx->buffer, 0, sizeof(ctx->buffer));
        return STATE_READING;
    } else if (strcmp(input, "q") == 0) {
        exit(0);
    }
    
    return STATE_IDLE;
}

state_t handle_reading(context_t* ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    printf("Enter text (max 255 chars): ");
    if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) == NULL) return STATE_ERROR;
    
    size_t len = strlen(ctx->buffer);
    if (len > 0 && ctx->buffer[len-1] == '\n') {
        ctx->buffer[len-1] = '\0';
        len--;
    }
    
    ctx->buffer_pos = len;
    
    if (len > 0) {
        return STATE_PROCESSING;
    }
    
    return STATE_READING;
}

state_t handle_processing(context_t* ctx) {
    if (ctx == NULL || ctx->buffer_pos >= sizeof(ctx->buffer)) return STATE_ERROR;
    
    for (size_t i = 0; i < ctx->buffer_pos; i++) {
        if (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') {
            ctx->buffer[i] = ctx->buffer[i] - 'a' + 'A';
        }
    }
    
    return STATE_WRITING;
}

state_t handle_writing(context_t* ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    printf("Processed text: %s\n", ctx->buffer);
    return STATE_IDLE;
}

state_t handle_error(context_t* ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    printf("Error occurred. Resetting state machine.\n");
    ctx->error_code = 0;
    ctx->buffer_pos = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    return STATE_IDLE;
}

int main(void) {
    context_t ctx;
    ctx.current_state = STATE_IDLE;
    ctx.buffer_pos = 0;
    ctx.error_code = 0;
    memset(ctx.buffer, 0, sizeof(ctx.buffer));
    
    state_handler_t handlers[STATE_COUNT] = {
        handle_idle,
        handle_reading,
        handle_processing,
        handle_writing,
        handle_error
    };
    
    if (STATE_COUNT != 6) {
        fprintf(stderr, "State count mismatch\n");
        return 1;
    }
    
    while (1) {
        if (ctx.current_state >= STATE_COUNT || ctx.current_state < 0) {
            ctx.current_state = STATE_ERROR;
        }
        
        state_handler_t handler = handlers[ctx.current_state];
        if (handler == NULL) {
            fprintf(stderr, "Invalid state handler\n");
            return 1;
        }
        
        state_t next_state = handler(&ctx);
        
        if (next_state >= STATE_COUNT || next_state < 0) {
            ctx.current_state = STATE_ERROR;
        } else {
            ctx.current_state = next_state;
        }
    }
    
    return 0;
}