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
    if (ctx == NULL) return STATE_ERROR;
    
    printf("Enter command (r=read, q=quit): ");
    char input[16];
    if (fgets(input, sizeof(input), stdin) == NULL) return STATE_ERROR;
    
    if (input[0] == 'r') {
        ctx->buffer_pos = 0;
        memset(ctx->buffer, 0, sizeof(ctx->buffer));
        return STATE_READING;
    } else if (input[0] == 'q') {
        exit(0);
    }
    
    return STATE_IDLE;
}

state_t handle_reading(context_t* ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    printf("Enter text (max 255 chars, . to end): ");
    if (fgets(ctx->buffer + ctx->buffer_pos, sizeof(ctx->buffer) - ctx->buffer_pos, stdin) == NULL) {
        return STATE_ERROR;
    }
    
    size_t len = strlen(ctx->buffer + ctx->buffer_pos);
    if (len > 0 && ctx->buffer[ctx->buffer_pos + len - 1] == '\n') {
        ctx->buffer[ctx->buffer_pos + len - 1] = '\0';
        len--;
    }
    
    if (len > 0 && ctx->buffer[ctx->buffer_pos] == '.') {
        ctx->buffer[ctx->buffer_pos] = '\0';
        return STATE_PROCESSING;
    }
    
    ctx->buffer_pos += len;
    if (ctx->buffer_pos >= sizeof(ctx->buffer) - 1) {
        ctx->buffer[sizeof(ctx->buffer) - 1] = '\0';
        return STATE_PROCESSING;
    }
    
    return STATE_READING;
}

state_t handle_processing(context_t* ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    size_t len = strlen(ctx->buffer);
    if (len == 0) {
        printf("No text to process.\n");
        return STATE_IDLE;
    }
    
    for (size_t i = 0; i < len; i++) {
        if (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') {
            ctx->buffer[i] = ctx->buffer[i] - 'a' + 'A';
        }
    }
    
    printf("Processed text: %s\n", ctx->buffer);
    return STATE_WRITING;
}

state_t handle_writing(context_t* ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    printf("Save processed text? (y/n): ");
    char input[16];
    if (fgets(input, sizeof(input), stdin) == NULL) return STATE_ERROR;
    
    if (input[0] == 'y') {
        FILE* file = fopen("output.txt", "w");
        if (file != NULL) {
            fputs(ctx->buffer, file);
            fclose(file);
            printf("Text saved to output.txt\n");
        } else {
            printf("Error saving file.\n");
        }
    }
    
    return STATE_IDLE;
}

state_t handle_error(context_t* ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    printf("An error occurred. Resetting state machine.\n");
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
    
    while (1) {
        if (ctx.current_state < 0 || ctx.current_state >= STATE_COUNT) {
            ctx.current_state = STATE_ERROR;
        }
        
        state_t next_state = handlers[ctx.current_state](&ctx);
        if (next_state < 0 || next_state >= STATE_COUNT) {
            ctx.current_state = STATE_ERROR;
        } else {
            ctx.current_state = next_state;
        }
    }
    
    return 0;
}