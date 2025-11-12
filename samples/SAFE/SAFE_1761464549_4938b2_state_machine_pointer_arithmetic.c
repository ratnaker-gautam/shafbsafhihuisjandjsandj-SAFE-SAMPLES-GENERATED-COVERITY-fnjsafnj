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

state_t (*state_handlers[STATE_COUNT])(context_t*, char) = {
    handle_start,
    handle_reading,
    NULL,
    NULL,
    NULL
};

int main(void) {
    context_t ctx;
    char input[MAX_INPUT_LEN];
    
    memset(&ctx, 0, sizeof(ctx));
    ctx.current = STATE_START;
    
    printf("Enter sequence (start with B, end with E, letters only): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    
    char *input_ptr = input;
    while (*input_ptr != '\0' && *input_ptr != '\n') {
        state_t next_state;
        
        switch (ctx.current) {
            case STATE_START:
            case STATE_READING:
                next_state = state_handlers[ctx.current](&ctx, *input_ptr);
                break;
            case STATE_PROCESSING:
                next_state = handle_processing(&ctx);
                break;
            case STATE_FINISHED:
                goto done;
            case STATE_ERROR:
                printf("Invalid input sequence\n");
                return 1;
        }
        
        ctx.current = next_state;
        input_ptr++;
        
        if (ctx.current == STATE_PROCESSING) {
            next_state = handle_processing(&ctx);
            ctx.current = next_state;
            break;
        }
    }
    
done:
    if (ctx.current == STATE_FINISHED) {
        printf("Processed successfully. Uppercase letters: %d\n", ctx.result);
        return 0;
    } else {
        printf("Invalid input sequence\n");
        return 1;
    }
}