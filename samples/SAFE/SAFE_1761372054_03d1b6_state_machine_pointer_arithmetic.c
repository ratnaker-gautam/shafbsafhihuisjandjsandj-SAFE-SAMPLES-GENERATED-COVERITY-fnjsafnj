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
    char *buffer;
    size_t pos;
    size_t len;
} context_t;

static state_t handle_start(context_t *ctx, char input) {
    if (input == 'B') {
        *(ctx->buffer + ctx->pos) = input;
        ctx->pos++;
        return STATE_READING;
    }
    return STATE_ERROR;
}

static state_t handle_reading(context_t *ctx, char input) {
    if (input == 'E' && ctx->pos > 0) {
        *(ctx->buffer + ctx->pos) = '\0';
        return STATE_PROCESSING;
    } else if (ctx->pos < ctx->len - 1 && input >= 'A' && input <= 'Z') {
        *(ctx->buffer + ctx->pos) = input;
        ctx->pos++;
        return STATE_READING;
    }
    return STATE_ERROR;
}

static state_t handle_processing(context_t *ctx) {
    char *start = ctx->buffer;
    char *end = ctx->buffer + ctx->pos - 1;
    
    while (start < end) {
        char temp = *start;
        *start = *end;
        *end = temp;
        start++;
        end--;
    }
    return STATE_FINISHED;
}

static state_t handle_finished(context_t *ctx) {
    printf("Processed: %s\n", ctx->buffer);
    return STATE_START;
}

static state_t handle_error(context_t *ctx) {
    printf("Error: Invalid input sequence\n");
    return STATE_START;
}

static void reset_context(context_t *ctx) {
    ctx->current = STATE_START;
    ctx->pos = 0;
    memset(ctx->buffer, 0, ctx->len);
}

int main(void) {
    context_t ctx;
    char input[MAX_INPUT_LEN + 1];
    
    ctx.buffer = malloc(MAX_INPUT_LEN + 1);
    if (ctx.buffer == NULL) {
        return EXIT_FAILURE;
    }
    ctx.len = MAX_INPUT_LEN + 1;
    
    reset_context(&ctx);
    
    printf("Enter sequences starting with 'B' and ending with 'E':\n");
    printf("Type 'QUIT' to exit.\n");
    
    while (fgets(input, sizeof(input), stdin) != NULL) {
        if (strncmp(input, "QUIT", 4) == 0) {
            break;
        }
        
        char *ptr = input;
        while (*ptr != '\0' && *ptr != '\n') {
            switch (ctx.current) {
                case STATE_START:
                    ctx.current = handle_start(&ctx, *ptr);
                    break;
                case STATE_READING:
                    ctx.current = handle_reading(&ctx, *ptr);
                    break;
                case STATE_PROCESSING:
                    ctx.current = handle_processing(&ctx);
                    break;
                case STATE_FINISHED:
                    ctx.current = handle_finished(&ctx);
                    reset_context(&ctx);
                    break;
                case STATE_ERROR:
                    ctx.current = handle_error(&ctx);
                    reset_context(&ctx);
                    break;
            }
            ptr++;
        }
        
        if (ctx.current == STATE_PROCESSING) {
            ctx.current = handle_processing(&ctx);
        }
        if (ctx.current == STATE_FINISHED) {
            ctx.current = handle_finished(&ctx);
            reset_context(&ctx);
        }
        if (ctx.current == STATE_ERROR) {
            ctx.current = handle_error(&ctx);
            reset_context(&ctx);
        }
    }
    
    free(ctx.buffer);
    return EXIT_SUCCESS;
}