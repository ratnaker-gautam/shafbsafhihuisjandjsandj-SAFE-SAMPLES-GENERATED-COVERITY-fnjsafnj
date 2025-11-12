//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_LEN 256
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
    char *input_ptr;
    char *input_end;
    int processed_count;
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
    ctx->processed_count = 0;
    return STATE_READING;
}

state_t handle_reading(context_t *ctx) {
    if (ctx->input_ptr >= ctx->input_end) {
        return STATE_FINISHED;
    }
    
    char current = *(ctx->input_ptr);
    if (current == '\0') {
        return STATE_FINISHED;
    }
    
    if (current < 32 || current > 126) {
        return STATE_ERROR;
    }
    
    return STATE_PROCESSING;
}

state_t handle_processing(context_t *ctx) {
    if (ctx->input_ptr >= ctx->input_end) {
        return STATE_ERROR;
    }
    
    char current = *(ctx->input_ptr);
    if (current >= 'a' && current <= 'z') {
        *(ctx->input_ptr) = current - 32;
    } else if (current >= 'A' && current <= 'Z') {
        *(ctx->input_ptr) = current + 32;
    }
    
    ctx->processed_count++;
    ctx->input_ptr++;
    
    return STATE_READING;
}

state_t handle_finished(context_t *ctx) {
    return STATE_FINISHED;
}

state_t handle_error(context_t *ctx) {
    return STATE_ERROR;
}

int main(void) {
    char input_buffer[MAX_INPUT_LEN + 1];
    
    printf("Enter text to process (max %d characters): ", MAX_INPUT_LEN);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return EXIT_FAILURE;
    }
    
    context_t ctx;
    ctx.current_state = STATE_START;
    ctx.input_ptr = input_buffer;
    ctx.input_end = input_buffer + input_len;
    ctx.processed_count = 0;
    
    while (ctx.current_state != STATE_FINISHED && ctx.current_state != STATE_ERROR) {
        if (ctx.current_state < 0 || ctx.current_state >= STATE_COUNT) {
            fprintf(stderr, "Invalid state encountered\n");
            return EXIT_FAILURE;
        }
        
        state_t next_state = state_handlers[ctx.current_state](&ctx);
        ctx.current_state = next_state;
    }
    
    if (ctx.current_state == STATE_ERROR) {
        fprintf(stderr, "Error processing input\n");
        return EXIT_FAILURE;
    }
    
    printf("Processed text: %s\n", input_buffer);
    printf("Characters processed: %d\n", ctx.processed_count);
    
    return EXIT_SUCCESS;
}