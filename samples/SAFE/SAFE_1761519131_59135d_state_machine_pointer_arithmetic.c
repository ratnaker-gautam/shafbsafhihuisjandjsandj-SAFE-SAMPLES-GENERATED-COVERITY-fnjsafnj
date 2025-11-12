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
    STATE_FINALIZING,
    STATE_DONE
} state_t;

typedef struct {
    state_t current_state;
    char buffer[MAX_INPUT_LEN];
    size_t buffer_pos;
    size_t processed_chars;
} context_t;

state_t handle_start(context_t *ctx, char input);
state_t handle_reading(context_t *ctx, char input);
state_t handle_processing(context_t *ctx, char input);
state_t handle_finalizing(context_t *ctx, char input);
state_t handle_done(context_t *ctx, char input);

int main(void) {
    context_t ctx;
    ctx.current_state = STATE_START;
    ctx.buffer_pos = 0;
    ctx.processed_chars = 0;
    memset(ctx.buffer, 0, sizeof(ctx.buffer));

    printf("Enter text (max %d chars): ", MAX_INPUT_LEN - 1);
    
    char input_char;
    while ((input_char = getchar()) != EOF && ctx.current_state != STATE_DONE) {
        if (ctx.buffer_pos >= MAX_INPUT_LEN - 1) {
            break;
        }

        state_t (*handlers[STATE_COUNT])(context_t*, char) = {
            handle_start,
            handle_reading,
            handle_processing,
            handle_finalizing,
            handle_done
        };

        if (ctx.current_state < STATE_COUNT) {
            state_t new_state = handlers[ctx.current_state](&ctx, input_char);
            ctx.current_state = new_state;
        }
    }

    if (ctx.current_state == STATE_DONE) {
        printf("Processing complete. Processed %zu characters.\n", ctx.processed_chars);
        printf("Final buffer: %s\n", ctx.buffer);
    } else {
        printf("Processing interrupted or buffer full.\n");
    }

    return 0;
}

state_t handle_start(context_t *ctx, char input) {
    if (input == '\n') {
        return STATE_START;
    }
    if (ctx->buffer_pos < MAX_INPUT_LEN - 1) {
        *(ctx->buffer + ctx->buffer_pos) = input;
        ctx->buffer_pos++;
        ctx->processed_chars++;
        return STATE_READING;
    }
    return STATE_DONE;
}

state_t handle_reading(context_t *ctx, char input) {
    if (input == '\n') {
        return STATE_PROCESSING;
    }
    if (ctx->buffer_pos < MAX_INPUT_LEN - 1) {
        *(ctx->buffer + ctx->buffer_pos) = input;
        ctx->buffer_pos++;
        ctx->processed_chars++;
        return STATE_READING;
    }
    return STATE_PROCESSING;
}

state_t handle_processing(context_t *ctx, char input) {
    char *ptr = ctx->buffer;
    char *end = ptr + ctx->buffer_pos;
    
    while (ptr < end) {
        if (*ptr >= 'a' && *ptr <= 'z') {
            *ptr = *ptr - 'a' + 'A';
        }
        ptr++;
    }
    
    return STATE_FINALIZING;
}

state_t handle_finalizing(context_t *ctx, char input) {
    if (ctx->buffer_pos > 0) {
        char *ptr = ctx->buffer;
        char *end = ptr + ctx->buffer_pos;
        
        while (ptr < end - 1) {
            if (*ptr == ' ' && *(ptr + 1) == ' ') {
                char *src = ptr + 1;
                char *dst = ptr;
                while (src < end) {
                    *dst = *src;
                    dst++;
                    src++;
                }
                ctx->buffer_pos--;
                end--;
            } else {
                ptr++;
            }
        }
    }
    
    return STATE_DONE;
}

state_t handle_done(context_t *ctx, char input) {
    return STATE_DONE;
}