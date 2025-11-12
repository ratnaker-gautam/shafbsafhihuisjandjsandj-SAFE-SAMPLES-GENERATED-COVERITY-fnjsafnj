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
    STATE_FINAL,
    STATE_ERROR
} state_t;

typedef struct {
    state_t current;
    char *buffer;
    size_t position;
    size_t length;
} context_t;

state_t handle_start(context_t *ctx);
state_t handle_reading(context_t *ctx);
state_t handle_processing(context_t *ctx);
state_t handle_final(context_t *ctx);
state_t handle_error(context_t *ctx);

int main(void) {
    context_t ctx;
    char input_buffer[MAX_INPUT_LEN];
    
    ctx.buffer = input_buffer;
    ctx.position = 0;
    ctx.length = 0;
    ctx.current = STATE_START;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN - 1);
    
    if (fgets(ctx.buffer, MAX_INPUT_LEN, stdin) == NULL) {
        return EXIT_FAILURE;
    }
    
    ctx.length = strlen(ctx.buffer);
    if (ctx.length > 0 && *(ctx.buffer + ctx.length - 1) == '\n') {
        *(ctx.buffer + ctx.length - 1) = '\0';
        ctx.length--;
    }
    
    if (ctx.length == 0) {
        printf("Empty input\n");
        return EXIT_FAILURE;
    }
    
    while (ctx.current != STATE_FINAL && ctx.current != STATE_ERROR) {
        switch (ctx.current) {
            case STATE_START:
                ctx.current = handle_start(&ctx);
                break;
            case STATE_READING:
                ctx.current = handle_reading(&ctx);
                break;
            case STATE_PROCESSING:
                ctx.current = handle_processing(&ctx);
                break;
            case STATE_FINAL:
                ctx.current = handle_final(&ctx);
                break;
            case STATE_ERROR:
                ctx.current = handle_error(&ctx);
                break;
        }
    }
    
    if (ctx.current == STATE_FINAL) {
        printf("Processing completed successfully\n");
    } else {
        printf("Processing failed\n");
    }
    
    return EXIT_SUCCESS;
}

state_t handle_start(context_t *ctx) {
    if (ctx == NULL || ctx->buffer == NULL) {
        return STATE_ERROR;
    }
    
    printf("Starting state machine\n");
    ctx->position = 0;
    
    return STATE_READING;
}

state_t handle_reading(context_t *ctx) {
    if (ctx == NULL || ctx->buffer == NULL) {
        return STATE_ERROR;
    }
    
    if (ctx->position >= ctx->length) {
        return STATE_PROCESSING;
    }
    
    char current_char = *(ctx->buffer + ctx->position);
    if (current_char == '\0') {
        return STATE_PROCESSING;
    }
    
    printf("Read character: %c\n", current_char);
    ctx->position++;
    
    return STATE_READING;
}

state_t handle_processing(context_t *ctx) {
    if (ctx == NULL || ctx->buffer == NULL) {
        return STATE_ERROR;
    }
    
    printf("Processing buffer contents\n");
    
    char *start = ctx->buffer;
    char *end = ctx->buffer + ctx->length - 1;
    
    while (start < end) {
        char temp = *start;
        *start = *end;
        *end = temp;
        start++;
        end--;
    }
    
    printf("Reversed string: %s\n", ctx->buffer);
    
    return STATE_FINAL;
}

state_t handle_final(context_t *ctx) {
    if (ctx == NULL) {
        return STATE_ERROR;
    }
    
    return STATE_FINAL;
}

state_t handle_error(context_t *ctx) {
    if (ctx == NULL) {
        return STATE_ERROR;
    }
    
    return STATE_ERROR;
}