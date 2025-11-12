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
    if (input == 'B') {
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
    if (input == 'E') {
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
        if (*ptr >= 'A' && *ptr <= 'Z') {
            count++;
        }
        ptr++;
    }
    
    ctx->result = count;
    return STATE_FINISHED;
}

state_t handle_finished(context_t *ctx) {
    printf("Processing complete. Uppercase letters: %d\n", ctx->result);
    return STATE_FINISHED;
}

state_t handle_error(context_t *ctx) {
    printf("Error: Invalid input sequence\n");
    return STATE_ERROR;
}

void process_input(context_t *ctx, const char *input) {
    const char *ptr = input;
    
    while (*ptr != '\0' && ctx->current != STATE_FINISHED && ctx->current != STATE_ERROR) {
        switch (ctx->current) {
            case STATE_START:
                ctx->current = handle_start(ctx, *ptr);
                break;
            case STATE_READING:
                ctx->current = handle_reading(ctx, *ptr);
                break;
            case STATE_PROCESSING:
                ctx->current = handle_processing(ctx);
                break;
            case STATE_FINISHED:
                ctx->current = handle_finished(ctx);
                break;
            case STATE_ERROR:
                ctx->current = handle_error(ctx);
                break;
        }
        ptr++;
    }
    
    if (ctx->current == STATE_PROCESSING) {
        ctx->current = handle_processing(ctx);
    }
    if (ctx->current == STATE_FINISHED) {
        handle_finished(ctx);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 2];
    context_t ctx;
    
    printf("Enter input sequence (start with 'B', end with 'E'): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Error: Empty input\n");
        return 1;
    }
    
    memset(&ctx, 0, sizeof(ctx));
    ctx.current = STATE_START;
    ctx.pos = 0;
    
    process_input(&ctx, input);
    
    if (ctx.current == STATE_ERROR) {
        return 1;
    }
    
    return 0;
}