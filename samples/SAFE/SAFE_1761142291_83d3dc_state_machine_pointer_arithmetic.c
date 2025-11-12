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
    ctx.input_buffer = malloc(MAX_INPUT_LEN + 1);
    if (ctx.input_buffer == NULL) {
        return EXIT_FAILURE;
    }
    
    ctx.current_state = STATE_START;
    ctx.input_len = 0;
    ctx.position = 0;
    ctx.result = 0;
    
    while (ctx.current_state != STATE_FINISHED && ctx.current_state != STATE_ERROR) {
        switch (ctx.current_state) {
            case STATE_START:
                ctx.current_state = handle_start(&ctx);
                break;
            case STATE_READING:
                ctx.current_state = handle_reading(&ctx);
                break;
            case STATE_PROCESSING:
                ctx.current_state = handle_processing(&ctx);
                break;
            case STATE_FINISHED:
                ctx.current_state = handle_finished(&ctx);
                break;
            case STATE_ERROR:
                ctx.current_state = handle_error(&ctx);
                break;
        }
    }
    
    if (ctx.current_state == STATE_FINISHED) {
        printf("Processing complete. Result: %d\n", ctx.result);
    } else {
        printf("Error occurred during processing.\n");
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
    
    size_t len = strlen(ctx->input_buffer);
    if (len > 0 && *(ctx->input_buffer + len - 1) == '\n') {
        *(ctx->input_buffer + len - 1) = '\0';
        len--;
    }
    
    if (len == 0) {
        return STATE_ERROR;
    }
    
    ctx->input_len = len;
    return STATE_PROCESSING;
}

state_t handle_processing(context_t *ctx) {
    char *start = ctx->input_buffer;
    char *end = start + ctx->input_len;
    int sum = 0;
    
    while (start < end) {
        char current = *start;
        if (current >= '0' && current <= '9') {
            sum += (current - '0');
        }
        start++;
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