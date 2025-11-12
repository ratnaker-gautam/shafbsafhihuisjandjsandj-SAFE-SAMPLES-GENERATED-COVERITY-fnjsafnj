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
    char buffer[MAX_INPUT_LEN];
    size_t buffer_pos;
    int processed_count;
} context_t;

state_t handle_start(context_t *ctx, char input);
state_t handle_reading(context_t *ctx, char input);
state_t handle_processing(context_t *ctx, char input);
state_t handle_finished(context_t *ctx, char input);
state_t handle_error(context_t *ctx, char input);

state_t (*state_handlers[STATE_COUNT])(context_t *, char) = {
    handle_start,
    handle_reading,
    handle_processing,
    handle_finished,
    handle_error
};

state_t handle_start(context_t *ctx, char input) {
    if (input == 'B') {
        ctx->buffer_pos = 0;
        *(ctx->buffer + ctx->buffer_pos) = input;
        ctx->buffer_pos++;
        return STATE_READING;
    }
    return STATE_ERROR;
}

state_t handle_reading(context_t *ctx, char input) {
    if (ctx->buffer_pos >= MAX_INPUT_LEN - 1) {
        return STATE_ERROR;
    }
    
    if (input == 'E') {
        *(ctx->buffer + ctx->buffer_pos) = '\0';
        return STATE_PROCESSING;
    }
    
    if ((input >= 'A' && input <= 'Z') || (input >= 'a' && input <= 'z')) {
        *(ctx->buffer + ctx->buffer_pos) = input;
        ctx->buffer_pos++;
        return STATE_READING;
    }
    
    return STATE_ERROR;
}

state_t handle_processing(context_t *ctx, char input) {
    if (input != 'P') {
        return STATE_ERROR;
    }
    
    char *ptr = ctx->buffer;
    while (*ptr != '\0') {
        if (*ptr >= 'a' && *ptr <= 'z') {
            *ptr = *ptr - 32;
        }
        ptr++;
    }
    
    ctx->processed_count++;
    return STATE_FINISHED;
}

state_t handle_finished(context_t *ctx, char input) {
    if (input == 'R') {
        ctx->buffer_pos = 0;
        return STATE_START;
    }
    return STATE_ERROR;
}

state_t handle_error(context_t *ctx, char input) {
    return STATE_ERROR;
}

void process_input_sequence(const char *input) {
    if (input == NULL) {
        return;
    }
    
    context_t ctx;
    ctx.current_state = STATE_START;
    ctx.buffer_pos = 0;
    ctx.processed_count = 0;
    
    const char *ptr = input;
    while (*ptr != '\0' && ctx.current_state != STATE_ERROR) {
        state_t next_state = state_handlers[ctx.current_state](&ctx, *ptr);
        ctx.current_state = next_state;
        ptr++;
    }
    
    if (ctx.current_state == STATE_FINISHED) {
        printf("Processed: %s\n", ctx.buffer);
        printf("Total processed: %d\n", ctx.processed_count);
    } else if (ctx.current_state == STATE_ERROR) {
        printf("Error in processing\n");
    }
}

int main(void) {
    char input[MAX_INPUT_LEN * 2];
    
    printf("Enter input sequence: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 0 || len >= sizeof(input)) {
        return EXIT_FAILURE;
    }
    
    process_input_sequence(input);
    
    return EXIT_SUCCESS;
}