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

int main(void) {
    context_t ctx;
    ctx.current = STATE_START;
    ctx.pos = 0;
    ctx.result = 0;
    
    printf("Enter sequence (start with B/b, end with E/e, letters only): ");
    
    int c;
    while ((c = getchar()) != EOF && c != '\n') {
        if (ctx.current == STATE_PROCESSING || ctx.current == STATE_FINISHED || ctx.current == STATE_ERROR) {
            break;
        }
        
        if (ctx.current == STATE_START || ctx.current == STATE_READING) {
            state_t (*handler)(context_t *, char) = *(state_handlers + ctx.current);
            if (handler != NULL) {
                state_t next = handler(&ctx, (char)c);
                ctx.current = next;
            }
        }
    }
    
    if (ctx.current == STATE_READING) {
        printf("Error: Sequence not properly terminated with E/e\n");
        return 1;
    }
    
    if (ctx.current == STATE_PROCESSING) {
        state_t next = handle_processing(&ctx);
        ctx.current = next;
    }
    
    if (ctx.current == STATE_FINISHED) {
        printf("Vowel count in sequence: %d\n", ctx.result);
        return 0;
    } else {
        printf("Error: Invalid input sequence\n");
        return 1;
    }
}