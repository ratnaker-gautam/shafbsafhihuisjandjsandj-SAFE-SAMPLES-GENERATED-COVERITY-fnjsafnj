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
    size_t pos;
    int result;
} context_t;

state_t handle_start(context_t *ctx, char input) {
    if (input == 'B' || input == 'b') {
        *(ctx->buffer + ctx->pos) = input;
        ctx->pos++;
        return STATE_READING;
    }
    return STATE_ERROR;
}

state_t handle_reading(context_t *ctx, char input) {
    if (ctx->pos >= MAX_INPUT_LEN - 1) {
        return STATE_ERROR;
    }
    if (input == 'E' || input == 'e') {
        *(ctx->buffer + ctx->pos) = '\0';
        return STATE_PROCESSING;
    }
    if ((input >= 'A' && input <= 'Z') || (input >= 'a' && input <= 'z')) {
        *(ctx->buffer + ctx->pos) = input;
        ctx->pos++;
        return STATE_READING;
    }
    return STATE_ERROR;
}

state_t handle_processing(context_t *ctx) {
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
    return STATE_FINISHED;
}

state_t (*state_handlers[STATE_COUNT])(context_t *, char) = {
    handle_start,
    handle_reading,
    NULL,
    NULL,
    NULL
};

void process_input(context_t *ctx, const char *input) {
    const char *ptr = input;
    
    while (*ptr != '\0' && ctx->current != STATE_ERROR && ctx->current != STATE_FINISHED) {
        if (ctx->current == STATE_PROCESSING) {
            ctx->current = handle_processing(ctx);
            break;
        }
        
        if (ctx->current < STATE_COUNT && state_handlers[ctx->current] != NULL) {
            ctx->current = state_handlers[ctx->current](ctx, *ptr);
        } else {
            ctx->current = STATE_ERROR;
            break;
        }
        
        ptr++;
    }
    
    if (ctx->current == STATE_READING && *ptr == '\0') {
        ctx->current = STATE_ERROR;
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 2];
    context_t ctx;
    
    printf("Enter text starting with B/b and ending with E/e: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len >= MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    ctx.current = STATE_START;
    ctx.pos = 0;
    ctx.result = 0;
    memset(ctx.buffer, 0, sizeof(ctx.buffer));
    
    process_input(&ctx, input);
    
    switch (ctx.current) {
        case STATE_FINISHED:
            printf("Success! Vowel count: %d\n", ctx.result);
            break;
        case STATE_ERROR:
            printf("Error: Invalid input format\n");
            break;
        default:
            printf("Error: Incomplete input\n");
            break;
    }
    
    return 0;
}