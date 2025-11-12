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
    state_t current;
    char buffer[MAX_INPUT_LEN];
    size_t length;
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
    if (ctx == NULL) return STATE_ERROR;
    
    ctx->length = 0;
    ctx->processed_count = 0;
    *(ctx->buffer) = '\0';
    
    printf("State machine started. Enter text (max %d chars): ", MAX_INPUT_LEN - 1);
    return STATE_READING;
}

state_t handle_reading(context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    if (fgets(ctx->buffer, MAX_INPUT_LEN, stdin) == NULL) {
        return STATE_ERROR;
    }
    
    char *newline = strchr(ctx->buffer, '\n');
    if (newline != NULL) {
        *newline = '\0';
    }
    
    ctx->length = strlen(ctx->buffer);
    if (ctx->length == 0) {
        printf("Empty input. Try again: ");
        return STATE_READING;
    }
    
    return STATE_PROCESSING;
}

state_t handle_processing(context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    printf("Processing: ");
    char *ptr = ctx->buffer;
    char *end = ptr + ctx->length;
    
    while (ptr < end) {
        if (*ptr >= 'a' && *ptr <= 'z') {
            *ptr = *ptr - 'a' + 'A';
        }
        putchar(*ptr);
        ptr++;
    }
    putchar('\n');
    
    ctx->processed_count++;
    
    if (ctx->processed_count >= 3) {
        return STATE_FINISHED;
    }
    
    printf("Enter more text (max %d chars): ", MAX_INPUT_LEN - 1);
    return STATE_READING;
}

state_t handle_finished(context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    printf("Processing complete. Processed %d inputs.\n", ctx->processed_count);
    return STATE_FINISHED;
}

state_t handle_error(context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    printf("Error occurred. Exiting.\n");
    return STATE_ERROR;
}

int main(void) {
    context_t ctx;
    state_t current_state = STATE_START;
    
    while (current_state != STATE_FINISHED && current_state != STATE_ERROR) {
        if (current_state >= STATE_COUNT) {
            current_state = STATE_ERROR;
            continue;
        }
        
        state_t (*handler)(context_t *) = *(state_handlers + current_state);
        if (handler == NULL) {
            current_state = STATE_ERROR;
            continue;
        }
        
        current_state = handler(&ctx);
    }
    
    return (current_state == STATE_FINISHED) ? EXIT_SUCCESS : EXIT_FAILURE;
}