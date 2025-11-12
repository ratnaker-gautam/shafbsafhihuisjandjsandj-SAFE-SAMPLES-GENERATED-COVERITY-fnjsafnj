//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
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
    char *input_ptr;
    char *input_end;
    int processed_count;
} context_t;

state_t handle_start(context_t *ctx);
state_t handle_reading(context_t *ctx);
state_t handle_processing(context_t *ctx);
state_t handle_finished(context_t *ctx);
state_t handle_error(context_t *ctx);

int main(void) {
    char input_buffer[MAX_INPUT_LEN + 1] = {0};
    context_t ctx = {0};
    
    printf("Enter text to process (max %d chars): ", MAX_INPUT_LEN);
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && *(input_buffer + input_len - 1) == '\n') {
        *(input_buffer + input_len - 1) = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        fprintf(stderr, "Empty input\n");
        return EXIT_FAILURE;
    }
    
    ctx.current_state = STATE_START;
    ctx.input_ptr = input_buffer;
    ctx.input_end = input_buffer + input_len;
    ctx.processed_count = 0;
    
    while (ctx.current_state != STATE_FINISHED && ctx.current_state != STATE_ERROR) {
        state_t (*handlers[STATE_COUNT])(context_t *) = {
            handle_start,
            handle_reading,
            handle_processing,
            handle_finished,
            handle_error
        };
        
        if (ctx.current_state >= 0 && ctx.current_state < STATE_COUNT) {
            ctx.current_state = handlers[ctx.current_state](&ctx);
        } else {
            ctx.current_state = STATE_ERROR;
        }
    }
    
    if (ctx.current_state == STATE_ERROR) {
        fprintf(stderr, "Processing error occurred\n");
        return EXIT_FAILURE;
    }
    
    printf("Successfully processed %d characters\n", ctx.processed_count);
    return EXIT_SUCCESS;
}

state_t handle_start(context_t *ctx) {
    if (ctx == NULL || ctx->input_ptr == NULL || ctx->input_end == NULL) {
        return STATE_ERROR;
    }
    
    if (ctx->input_ptr >= ctx->input_end) {
        return STATE_ERROR;
    }
    
    printf("Starting processing...\n");
    return STATE_READING;
}

state_t handle_reading(context_t *ctx) {
    if (ctx == NULL || ctx->input_ptr == NULL || ctx->input_end == NULL) {
        return STATE_ERROR;
    }
    
    if (ctx->input_ptr >= ctx->input_end) {
        return STATE_PROCESSING;
    }
    
    char current_char = *(ctx->input_ptr);
    if (current_char == '\0') {
        return STATE_PROCESSING;
    }
    
    ctx->input_ptr++;
    return STATE_READING;
}

state_t handle_processing(context_t *ctx) {
    if (ctx == NULL || ctx->input_ptr == NULL || ctx->input_end == NULL) {
        return STATE_ERROR;
    }
    
    char *process_ptr = ctx->input_end - 1;
    char *start_ptr = ctx->input_ptr - (ctx->input_end - ctx->input_ptr);
    
    if (process_ptr < start_ptr) {
        return STATE_ERROR;
    }
    
    while (process_ptr >= start_ptr) {
        if (*process_ptr >= 'a' && *process_ptr <= 'z') {
            *process_ptr = *process_ptr - 'a' + 'A';
            ctx->processed_count++;
        } else if (*process_ptr >= 'A' && *process_ptr <= 'Z') {
            *process_ptr = *process_ptr - 'A' + 'a';
            ctx->processed_count++;
        }
        
        if (process_ptr == start_ptr) {
            break;
        }
        process_ptr--;
    }
    
    return STATE_FINISHED;
}

state_t handle_finished(context_t *ctx) {
    return STATE_FINISHED;
}

state_t handle_error(context_t *ctx) {
    return STATE_ERROR;
}