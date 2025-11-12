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
    char *input_buffer;
    size_t input_len;
    size_t position;
    int result;
} context_t;

state_t handle_start(context_t *ctx);
state_t handle_reading(context_t *ctx);
state_t handle_processing(context_t *ctx);
state_t handle_finished(context_t *ctx);
state_t handle_error(context_t *ctx);

int main(void) {
    context_t ctx;
    ctx.current_state = STATE_START;
    ctx.input_buffer = malloc(MAX_INPUT_LEN + 1);
    ctx.input_len = 0;
    ctx.position = 0;
    ctx.result = 0;
    
    if (ctx.input_buffer == NULL) {
        return EXIT_FAILURE;
    }
    
    state_t (*state_handlers[STATE_COUNT])(context_t *) = {
        handle_start,
        handle_reading,
        handle_processing,
        handle_finished,
        handle_error
    };
    
    while (ctx.current_state != STATE_FINISHED && ctx.current_state != STATE_ERROR) {
        if (ctx.current_state >= STATE_COUNT) {
            ctx.current_state = STATE_ERROR;
            break;
        }
        ctx.current_state = state_handlers[ctx.current_state](&ctx);
    }
    
    if (ctx.current_state == STATE_FINISHED) {
        printf("Processing completed. Result: %d\n", ctx.result);
    } else {
        printf("Processing failed with error.\n");
    }
    
    free(ctx.input_buffer);
    return (ctx.current_state == STATE_FINISHED) ? EXIT_SUCCESS : EXIT_FAILURE;
}

state_t handle_start(context_t *ctx) {
    printf("Enter a string (max %d characters): ", MAX_INPUT_LEN);
    return STATE_READING;
}

state_t handle_reading(context_t *ctx) {
    if (fgets(ctx->input_buffer, MAX_INPUT_LEN + 1, stdin) == NULL) {
        return STATE_ERROR;
    }
    
    char *newline_ptr = strchr(ctx->input_buffer, '\n');
    if (newline_ptr != NULL) {
        *newline_ptr = '\0';
    }
    
    ctx->input_len = strlen(ctx->input_buffer);
    if (ctx->input_len == 0) {
        return STATE_ERROR;
    }
    
    return STATE_PROCESSING;
}

state_t handle_processing(context_t *ctx) {
    char *buffer_end = ctx->input_buffer + ctx->input_len;
    char *current_ptr = ctx->input_buffer;
    int sum = 0;
    
    while (current_ptr < buffer_end) {
        if (*current_ptr >= '0' && *current_ptr <= '9') {
            sum += (*current_ptr - '0');
        }
        current_ptr++;
    }
    
    ctx->result = sum;
    return STATE_FINISHED;
}

state_t handle_finished(context_t *ctx) {
    return STATE_FINISHED;
}

state_t handle_error(context_t *ctx) {
    return STATE_ERROR;
}