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
    char input[MAX_INPUT_LEN + 1] = {0};
    
    printf("Enter a string (max %d chars): ", MAX_INPUT_LEN);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(input);
    if (len > 0 && *(input + len - 1) == '\n') {
        *(input + len - 1) = '\0';
        len--;
    }
    
    if (len == 0) {
        fprintf(stderr, "Empty input\n");
        return EXIT_FAILURE;
    }
    
    ctx.input_buffer = input;
    ctx.input_len = len;
    ctx.position = 0;
    ctx.result = 0;
    ctx.current_state = STATE_START;
    
    state_t (*state_handlers[STATE_COUNT])(context_t *) = {
        handle_start,
        handle_reading,
        handle_processing,
        handle_finished,
        handle_error
    };
    
    while (ctx.current_state != STATE_FINISHED && ctx.current_state != STATE_ERROR) {
        if (ctx.current_state >= STATE_COUNT) {
            fprintf(stderr, "Invalid state\n");
            return EXIT_FAILURE;
        }
        ctx.current_state = state_handlers[ctx.current_state](&ctx);
    }
    
    if (ctx.current_state == STATE_FINISHED) {
        printf("Processing complete. Result: %d\n", ctx.result);
        return EXIT_SUCCESS;
    } else {
        fprintf(stderr, "Processing failed\n");
        return EXIT_FAILURE;
    }
}

state_t handle_start(context_t *ctx) {
    if (ctx == NULL || ctx->input_buffer == NULL) {
        return STATE_ERROR;
    }
    printf("Starting processing...\n");
    return STATE_READING;
}

state_t handle_reading(context_t *ctx) {
    if (ctx == NULL || ctx->input_buffer == NULL) {
        return STATE_ERROR;
    }
    
    if (ctx->position >= ctx->input_len) {
        return STATE_PROCESSING;
    }
    
    char current_char = *(ctx->input_buffer + ctx->position);
    printf("Read character: %c\n", current_char);
    ctx->position++;
    
    if (ctx->position < ctx->input_len) {
        return STATE_READING;
    } else {
        return STATE_PROCESSING;
    }
}

state_t handle_processing(context_t *ctx) {
    if (ctx == NULL || ctx->input_buffer == NULL) {
        return STATE_ERROR;
    }
    
    ctx->position = 0;
    while (ctx->position < ctx->input_len) {
        char current_char = *(ctx->input_buffer + ctx->position);
        if (current_char >= '0' && current_char <= '9') {
            ctx->result += (current_char - '0');
        }
        ctx->position++;
    }
    
    return STATE_FINISHED;
}

state_t handle_finished(context_t *ctx) {
    return STATE_FINISHED;
}

state_t handle_error(context_t *ctx) {
    return STATE_ERROR;
}