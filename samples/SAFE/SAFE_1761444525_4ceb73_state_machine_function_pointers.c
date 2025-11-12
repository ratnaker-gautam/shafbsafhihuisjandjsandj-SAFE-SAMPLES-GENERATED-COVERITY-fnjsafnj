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
    
    printf("Enter text (max 255 chars, 'quit' to exit): ");
    if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) == NULL) {
        ctx->error_code = 1;
        return STATE_ERROR;
    }
    
    ctx->buffer_pos = 0;
    
    if (strncmp(ctx->buffer, "quit", 4) == 0) {
        return STATE_IDLE;
    }
    
    return STATE_READING;
}

state_t handle_reading(context_t* ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    size_t len = strlen(ctx->buffer);
    if (len > 0 && ctx->buffer[len - 1] == '\n') {
        ctx->buffer[len - 1] = '\0';
    }
    
    if (strlen(ctx->buffer) == 0) {
        return STATE_IDLE;
    }
    
    return STATE_PROCESSING;
}

state_t handle_processing(context_t* ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    char* str = ctx->buffer;
    size_t len = strlen(str);
    
    for (size_t i = 0; i < len / 2; i++) {
        if (i >= len - 1 - i) break;
        char temp = str[i];
        str[i] = str[len - 1 - i];
        str[len - 1 - i] = temp;
    }
    
    return STATE_WRITING;
}

state_t handle_writing(context_t* ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    printf("Reversed: %s\n", ctx->buffer);
    return STATE_IDLE;
}

state_t handle_error(context_t* ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    printf("Error occurred (code: %d)\n", ctx->error_code);
    ctx->error_code = 0;
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
        return EXIT_FAILURE;
    }
    
    int running = 1;
    while (running) {
        if (ctx.current_state < 0 || ctx.current_state >= STATE_COUNT) {
            ctx.current_state = STATE_ERROR;
        }
        
        state_t next_state = handlers[ctx.current_state](&ctx);
        
        if (next_state < 0 || next_state >= STATE_COUNT) {
            ctx.current_state = STATE_ERROR;
            ctx.error_code = 2;
        } else {
            ctx.current_state = next_state;
        }
        
        if (ctx.current_state == STATE_IDLE) {
            printf("Continue? (y/n): ");
            char response[16];
            if (fgets(response, sizeof(response), stdin) != NULL) {
                if (response[0] == 'n' || response[0] == 'N') {
                    running = 0;
                }
            }
        }
    }
    
    return EXIT_SUCCESS;
}