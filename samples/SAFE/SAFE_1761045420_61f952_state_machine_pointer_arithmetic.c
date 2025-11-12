//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_LEN 100
#define STATE_COUNT 5

typedef enum {
    STATE_START,
    STATE_READING,
    STATE_PROCESSING,
    STATE_FINISHED,
    STATE_ERROR
} state_t;

typedef struct {
    state_t current_state;
    char *input_buffer;
    size_t input_len;
    size_t position;
} context_t;

state_t handle_start(context_t *ctx);
state_t handle_reading(context_t *ctx);
state_t handle_processing(context_t *ctx);
state_t handle_finished(context_t *ctx);
state_t handle_error(context_t *ctx);

state_t (*state_handlers[STATE_COUNT])(context_t *) = {
    handle_start,
    handle_reading,
    handle_processing,
    handle_finished,
    handle_error
};

state_t handle_start(context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    ctx->input_buffer = malloc(MAX_INPUT_LEN + 1);
    if (ctx->input_buffer == NULL) return STATE_ERROR;
    
    memset(ctx->input_buffer, 0, MAX_INPUT_LEN + 1);
    ctx->input_len = 0;
    ctx->position = 0;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN);
    return STATE_READING;
}

state_t handle_reading(context_t *ctx) {
    if (ctx == NULL || ctx->input_buffer == NULL) return STATE_ERROR;
    
    char *buffer_ptr = ctx->input_buffer;
    int c;
    size_t count = 0;
    
    while (count < MAX_INPUT_LEN) {
        c = getchar();
        if (c == EOF || c == '\n') break;
        
        if (c >= 32 && c <= 126) {
            *(buffer_ptr + count) = (char)c;
            count++;
        }
    }
    
    if (count == 0) return STATE_ERROR;
    
    ctx->input_len = count;
    *(buffer_ptr + ctx->input_len) = '\0';
    return STATE_PROCESSING;
}

state_t handle_processing(context_t *ctx) {
    if (ctx == NULL || ctx->input_buffer == NULL || ctx->input_len == 0) {
        return STATE_ERROR;
    }
    
    char *start_ptr = ctx->input_buffer;
    char *end_ptr = start_ptr + (ctx->input_len - 1);
    
    printf("Processing: ");
    while (start_ptr <= end_ptr) {
        if (*start_ptr >= 'a' && *start_ptr <= 'z') {
            *start_ptr = *start_ptr - 32;
        }
        putchar(*start_ptr);
        start_ptr++;
    }
    printf("\n");
    
    return STATE_FINISHED;
}

state_t handle_finished(context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    printf("Processing complete. Result length: %zu\n", ctx->input_len);
    
    if (ctx->input_buffer != NULL) {
        free(ctx->input_buffer);
        ctx->input_buffer = NULL;
    }
    
    return STATE_START;
}

state_t handle_error(context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    printf("Error occurred during processing.\n");
    
    if (ctx->input_buffer != NULL) {
        free(ctx->input_buffer);
        ctx->input_buffer = NULL;
    }
    
    return STATE_START;
}

int main(void) {
    context_t ctx;
    ctx.current_state = STATE_START;
    ctx.input_buffer = NULL;
    ctx.input_len = 0;
    ctx.position = 0;
    
    int cycles = 0;
    const int max_cycles = 10;
    
    while (cycles < max_cycles) {
        if (ctx.current_state < 0 || ctx.current_state >= STATE_COUNT) {
            break;
        }
        
        state_t next_state = state_handlers[ctx.current_state](&ctx);
        
        if (next_state == STATE_START && ctx.current_state == STATE_FINISHED) {
            cycles++;
        }
        
        ctx.current_state = next_state;
        
        if (ctx.current_state == STATE_ERROR && cycles > 0) {
            break;
        }
    }
    
    if (ctx.input_buffer != NULL) {
        free(ctx.input_buffer);
    }
    
    return 0;
}