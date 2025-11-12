//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_LEN 128
#define STATE_COUNT 5

typedef enum {
    STATE_START,
    STATE_READING,
    STATE_PROCESSING,
    STATE_OUTPUT,
    STATE_END
} state_t;

typedef struct {
    state_t current;
    char buffer[MAX_INPUT_LEN];
    size_t pos;
    int result;
} context_t;

state_t handle_start(context_t *ctx, char input) {
    if (input == 'B' || input == 'b') {
        ctx->pos = 0;
        *(ctx->buffer + ctx->pos) = input;
        ctx->pos++;
        return STATE_READING;
    }
    return STATE_START;
}

state_t handle_reading(context_t *ctx, char input) {
    if (ctx->pos >= MAX_INPUT_LEN - 1) {
        return STATE_PROCESSING;
    }
    if (input == '\n' || input == '\0') {
        *(ctx->buffer + ctx->pos) = '\0';
        return STATE_PROCESSING;
    }
    if ((input >= 'A' && input <= 'Z') || (input >= 'a' && input <= 'z')) {
        *(ctx->buffer + ctx->pos) = input;
        ctx->pos++;
        return STATE_READING;
    }
    return STATE_END;
}

state_t handle_processing(context_t *ctx) {
    if (ctx->pos == 0) {
        return STATE_END;
    }
    char *ptr = ctx->buffer;
    int count = 0;
    while (*ptr != '\0') {
        if (*ptr == 'a' || *ptr == 'e' || *ptr == 'i' || *ptr == 'o' || *ptr == 'u' ||
            *ptr == 'A' || *ptr == 'E' || *ptr == 'I' || *ptr == 'O' || *ptr == 'U') {
            count++;
        }
        ptr++;
    }
    ctx->result = count;
    return STATE_OUTPUT;
}

state_t handle_output(context_t *ctx) {
    printf("Vowel count: %d\n", ctx->result);
    return STATE_END;
}

state_t (*state_handlers[STATE_COUNT])(context_t *, char) = {
    handle_start,
    handle_reading,
    NULL,
    handle_output,
    NULL
};

int main(void) {
    context_t ctx;
    ctx.current = STATE_START;
    ctx.pos = 0;
    ctx.result = 0;
    memset(ctx.buffer, 0, MAX_INPUT_LEN);

    printf("Enter text starting with 'B' (max %d chars): ", MAX_INPUT_LEN - 1);
    
    int c;
    while (ctx.current != STATE_END && (c = getchar()) != EOF) {
        if (c == '\n') {
            continue;
        }
        
        state_t next_state = ctx.current;
        if (ctx.current == STATE_PROCESSING) {
            next_state = handle_processing(&ctx);
        } else if (state_handlers[ctx.current] != NULL) {
            next_state = state_handlers[ctx.current](&ctx, (char)c);
        }
        
        ctx.current = next_state;
        
        if (ctx.current == STATE_OUTPUT) {
            state_t output_next = handle_output(&ctx);
            ctx.current = output_next;
        }
    }

    if (ctx.current != STATE_END) {
        if (ctx.current == STATE_READING) {
            *(ctx.buffer + ctx.pos) = '\0';
            ctx.current = STATE_PROCESSING;
            state_t proc_next = handle_processing(&ctx);
            if (proc_next == STATE_OUTPUT) {
                handle_output(&ctx);
            }
        }
    }

    return 0;
}