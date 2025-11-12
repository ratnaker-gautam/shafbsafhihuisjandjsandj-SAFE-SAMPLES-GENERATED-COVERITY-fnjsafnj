//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: function_pointers ; Variation: state_machine
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
    printf("Enter command (r=read, q=quit): ");
    char input[16];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        ctx->error_code = 1;
        return STATE_ERROR;
    }
    
    if (strlen(input) == 0 || input[0] == '\n') {
        return STATE_IDLE;
    }
    
    switch (input[0]) {
        case 'r':
        case 'R':
            ctx->buffer_pos = 0;
            memset(ctx->buffer, 0, sizeof(ctx->buffer));
            return STATE_READING;
        case 'q':
        case 'Q':
            return STATE_COUNT;
        default:
            printf("Invalid command\n");
            return STATE_IDLE;
    }
}

state_t handle_reading(context_t* ctx) {
    printf("Enter text (max 255 chars, empty line to finish): ");
    if (fgets(ctx->buffer + ctx->buffer_pos, sizeof(ctx->buffer) - ctx->buffer_pos, stdin) == NULL) {
        ctx->error_code = 2;
        return STATE_ERROR;
    }
    
    size_t len = strlen(ctx->buffer + ctx->buffer_pos);
    if (len > 0 && ctx->buffer[ctx->buffer_pos + len - 1] == '\n') {
        ctx->buffer[ctx->buffer_pos + len - 1] = '\0';
    }
    
    if (strlen(ctx->buffer) == 0) {
        printf("No input received\n");
        return STATE_IDLE;
    }
    
    ctx->buffer_pos = strlen(ctx->buffer);
    return STATE_PROCESSING;
}

state_t handle_processing(context_t* ctx) {
    if (ctx->buffer_pos == 0 || ctx->buffer_pos >= sizeof(ctx->buffer)) {
        ctx->error_code = 3;
        return STATE_ERROR;
    }
    
    for (size_t i = 0; i < ctx->buffer_pos; i++) {
        if (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') {
            ctx->buffer[i] = ctx->buffer[i] - 'a' + 'A';
        }
    }
    
    return STATE_WRITING;
}

state_t handle_writing(context_t* ctx) {
    printf("Processed text: %s\n", ctx->buffer);
    return STATE_IDLE;
}

state_t handle_error(context_t* ctx) {
    printf("Error %d occurred\n", ctx->error_code);
    ctx->error_code = 0;
    return STATE_IDLE;
}

int main(void) {
    state_handler_t handlers[STATE_COUNT] = {
        handle_idle,
        handle_reading,
        handle_processing,
        handle_writing,
        handle_error
    };
    
    context_t ctx = {
        .current_state = STATE_IDLE,
        .buffer_pos = 0,
        .error_code = 0
    };
    memset(ctx.buffer, 0, sizeof(ctx.buffer));
    
    while (ctx.current_state < STATE_COUNT) {
        if (ctx.current_state >= STATE_COUNT || ctx.current_state < 0) {
            break;
        }
        
        state_handler_t handler = handlers[ctx.current_state];
        if (handler == NULL) {
            break;
        }
        
        state_t next_state = handler(&ctx);
        if (next_state >= STATE_COUNT || next_state < 0) {
            break;
        }
        
        ctx.current_state = next_state;
    }
    
    printf("Program terminated\n");
    return 0;
}