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
    
    printf("Enter command (read/process/write/quit): ");
    char input[32];
    if (fgets(input, sizeof(input), stdin) == NULL) return STATE_ERROR;
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strcmp(input, "read") == 0) return STATE_READING;
    if (strcmp(input, "process") == 0) return STATE_PROCESSING;
    if (strcmp(input, "write") == 0) return STATE_WRITING;
    if (strcmp(input, "quit") == 0) exit(0);
    
    printf("Invalid command\n");
    return STATE_IDLE;
}

state_t handle_reading(context_t* ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    printf("Enter text to read (max 255 chars): ");
    if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) == NULL) return STATE_ERROR;
    
    ctx->buffer_pos = strlen(ctx->buffer);
    if (ctx->buffer_pos > 0 && ctx->buffer[ctx->buffer_pos - 1] == '\n') {
        ctx->buffer[--ctx->buffer_pos] = '\0';
    }
    
    printf("Read %zu characters: %s\n", ctx->buffer_pos, ctx->buffer);
    return STATE_IDLE;
}

state_t handle_processing(context_t* ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    if (ctx->buffer_pos == 0) {
        printf("No data to process\n");
        return STATE_IDLE;
    }
    
    printf("Processing data...\n");
    for (size_t i = 0; i < ctx->buffer_pos; i++) {
        if (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') {
            ctx->buffer[i] = ctx->buffer[i] - 'a' + 'A';
        }
    }
    
    printf("Processed: %s\n", ctx->buffer);
    return STATE_IDLE;
}

state_t handle_writing(context_t* ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    if (ctx->buffer_pos == 0) {
        printf("No data to write\n");
        return STATE_IDLE;
    }
    
    printf("Writing to output: %s\n", ctx->buffer);
    ctx->buffer[0] = '\0';
    ctx->buffer_pos = 0;
    return STATE_IDLE;
}

state_t handle_error(context_t* ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    printf("Error occurred. Resetting state machine.\n");
    ctx->buffer[0] = '\0';
    ctx->buffer_pos = 0;
    ctx->error_code = 0;
    return STATE_IDLE;
}

int main(void) {
    static state_handler_t handlers[STATE_COUNT] = {
        handle_idle,
        handle_reading,
        handle_processing,
        handle_writing,
        handle_error
    };
    
    context_t ctx = {
        .current_state = STATE_IDLE,
        .buffer = {0},
        .buffer_pos = 0,
        .error_code = 0
    };
    
    printf("State Machine Demo - Type 'quit' to exit\n");
    
    while (1) {
        if (ctx.current_state >= STATE_COUNT) {
            ctx.current_state = STATE_ERROR;
        }
        
        state_t next_state = handlers[ctx.current_state](&ctx);
        
        if (next_state >= STATE_COUNT) {
            ctx.current_state = STATE_ERROR;
        } else {
            ctx.current_state = next_state;
        }
    }
    
    return 0;
}