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
    char buffer[MAX_INPUT_LEN];
    size_t buffer_pos;
    int processed_count;
} context_t;

state_t handle_start(context_t *ctx, char input);
state_t handle_reading(context_t *ctx, char input);
state_t handle_processing(context_t *ctx, char input);
state_t handle_finished(context_t *ctx, char input);
state_t handle_error(context_t *ctx, char input);

int main(void) {
    context_t ctx;
    ctx.current_state = STATE_START;
    ctx.buffer_pos = 0;
    ctx.processed_count = 0;
    
    char input[MAX_INPUT_LEN * 2];
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && *(input + input_len - 1) == '\n') {
        *(input + input_len - 1) = '\0';
        input_len--;
    }
    
    if (input_len > MAX_INPUT_LEN) {
        printf("Input too long\n");
        return EXIT_FAILURE;
    }
    
    char *input_ptr = input;
    while (*input_ptr != '\0' && ctx.current_state != STATE_ERROR && ctx.current_state != STATE_FINISHED) {
        state_t (*handlers[STATE_COUNT])(context_t*, char) = {
            handle_start,
            handle_reading,
            handle_processing,
            handle_finished,
            handle_error
        };
        
        if (ctx.current_state >= STATE_COUNT) {
            ctx.current_state = STATE_ERROR;
            break;
        }
        
        state_t new_state = handlers[ctx.current_state](&ctx, *input_ptr);
        ctx.current_state = new_state;
        input_ptr++;
    }
    
    if (ctx.current_state == STATE_FINISHED) {
        printf("Processing complete. Processed %d characters.\n", ctx.processed_count);
        printf("Final buffer: ");
        for (size_t i = 0; i < ctx.buffer_pos; i++) {
            putchar(*(ctx.buffer + i));
        }
        putchar('\n');
    } else if (ctx.current_state == STATE_ERROR) {
        printf("Error state reached\n");
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}

state_t handle_start(context_t *ctx, char input) {
    if (ctx == NULL) {
        return STATE_ERROR;
    }
    
    if (input == 'S' || input == 's') {
        ctx->buffer_pos = 0;
        return STATE_READING;
    }
    
    return STATE_ERROR;
}

state_t handle_reading(context_t *ctx, char input) {
    if (ctx == NULL) {
        return STATE_ERROR;
    }
    
    if (ctx->buffer_pos >= MAX_INPUT_LEN) {
        return STATE_ERROR;
    }
    
    if (input == 'E' || input == 'e') {
        if (ctx->buffer_pos == 0) {
            return STATE_ERROR;
        }
        return STATE_PROCESSING;
    }
    
    if ((input >= 'A' && input <= 'Z') || (input >= 'a' && input <= 'z') || input == ' ') {
        *(ctx->buffer + ctx->buffer_pos) = input;
        ctx->buffer_pos++;
        return STATE_READING;
    }
    
    return STATE_ERROR;
}

state_t handle_processing(context_t *ctx, char input) {
    if (ctx == NULL) {
        return STATE_ERROR;
    }
    
    if (input == 'X' || input == 'x') {
        if (ctx->buffer_pos == 0) {
            return STATE_ERROR;
        }
        
        for (size_t i = 0; i < ctx->buffer_pos; i++) {
            char *current = ctx->buffer + i;
            if (*current >= 'a' && *current <= 'z') {
                *current = *current - 'a' + 'A';
            }
            ctx->processed_count++;
        }
        return STATE_FINISHED;
    }
    
    return STATE_ERROR;
}

state_t handle_finished(context_t *ctx, char input) {
    return STATE_FINISHED;
}

state_t handle_error(context_t *ctx, char input) {
    return STATE_ERROR;
}