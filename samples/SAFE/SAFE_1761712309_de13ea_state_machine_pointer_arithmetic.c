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
    char input_buffer[MAX_INPUT_LEN + 1];
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        return STATE_ERROR;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && *(input_buffer + (len - 1)) == '\n') {
        *(input_buffer + (len - 1)) = '\0';
        len--;
    }
    
    if (len == 0) {
        return STATE_FINISHED;
    }
    
    char *input_copy = malloc(len + 1);
    if (input_copy == NULL) {
        return STATE_ERROR;
    }
    
    memcpy(input_copy, input_buffer, len + 1);
    ctx->input_ptr = input_copy;
    ctx->input_end = input_copy + len;
    
    return STATE_PROCESSING;
}

state_t handle_processing(context_t *ctx) {
    char *current = ctx->input_ptr;
    char *end = ctx->input_end;
    
    while (current < end) {
        if (*current >= 'a' && *current <= 'z') {
            *current = *current - 'a' + 'A';
        }
        current++;
        ctx->processed_count++;
    }
    
    printf("Processed result: %s\n", ctx->input_ptr);
    free(ctx->input_ptr);
    ctx->input_ptr = NULL;
    ctx->input_end = NULL;
    
    return STATE_READING;
}

state_t handle_finished(context_t *ctx) {
    printf("Processing complete. Total characters processed: %d\n", ctx->processed_count);
    return STATE_FINISHED;
}

state_t handle_error(context_t *ctx) {
    printf("An error occurred during processing.\n");
    if (ctx->input_ptr != NULL) {
        free(ctx->input_ptr);
        ctx->input_ptr = NULL;
    }
    return STATE_ERROR;
}

int main(void) {
    context_t ctx = {0};
    ctx.current_state = STATE_START;
    
    while (ctx.current_state != STATE_FINISHED && ctx.current_state != STATE_ERROR) {
        if (ctx.current_state >= 0 && ctx.current_state < STATE_COUNT) {
            state_t (*handler)(context_t *) = *(state_handlers + ctx.current_state);
            ctx.current_state = handler(&ctx);
        } else {
            ctx.current_state = STATE_ERROR;
        }
    }
    
    return ctx.current_state == STATE_FINISHED ? EXIT_SUCCESS : EXIT_FAILURE;
}