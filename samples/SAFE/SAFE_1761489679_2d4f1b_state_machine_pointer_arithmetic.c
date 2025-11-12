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

void reset_context(context_t *ctx) {
    ctx->current = STATE_START;
    memset(ctx->buffer, 0, MAX_INPUT_LEN);
    ctx->pos = 0;
    ctx->result = 0;
}

int main(void) {
    context_t ctx;
    reset_context(&ctx);
    
    printf("Enter sequence starting with B/b, ending with E/e (letters only): ");
    
    char input;
    while (ctx.current != STATE_FINISHED && ctx.current != STATE_ERROR) {
        int read_result = scanf("%c", &input);
        if (read_result != 1) {
            ctx.current = STATE_ERROR;
            break;
        }
        
        if (input == '\n') {
            continue;
        }
        
        switch (ctx.current) {
            case STATE_START:
                ctx.current = handle_start(&ctx, input);
                break;
            case STATE_READING:
                ctx.current = handle_reading(&ctx, input);
                break;
            case STATE_PROCESSING:
                ctx.current = handle_processing(&ctx);
                break;
            default:
                ctx.current = STATE_ERROR;
                break;
        }
    }
    
    if (ctx.current == STATE_FINISHED) {
        printf("Vowel count: %d\n", ctx.result);
        return EXIT_SUCCESS;
    } else {
        printf("Error: Invalid input sequence\n");
        return EXIT_FAILURE;
    }
}