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
    state_t current_state;
    char buffer[MAX_INPUT_LEN];
    size_t buffer_pos;
    int processed_count;
} context_t;

state_t handle_start(context_t *ctx, char input);
state_t handle_reading(context_t *ctx, char input);
state_t handle_processing(context_t *ctx, char input);
state_t handle_output(context_t *ctx, char input);
state_t handle_end(context_t *ctx, char input);

int main(void) {
    context_t ctx;
    ctx.current_state = STATE_START;
    ctx.buffer_pos = 0;
    ctx.processed_count = 0;
    
    char input[MAX_INPUT_LEN * 2];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(input);
    if (input_len >= sizeof(input)) {
        return EXIT_FAILURE;
    }
    
    char *input_ptr = input;
    char *end_ptr = input_ptr + input_len;
    
    while (input_ptr < end_ptr && *input_ptr != '\0') {
        state_t (*handlers[STATE_COUNT])(context_t*, char) = {
            handle_start,
            handle_reading,
            handle_processing,
            handle_output,
            handle_end
        };
        
        if (ctx.current_state >= STATE_COUNT) {
            return EXIT_FAILURE;
        }
        
        state_t new_state = handlers[ctx.current_state](&ctx, *input_ptr);
        if (new_state >= STATE_COUNT) {
            return EXIT_FAILURE;
        }
        
        ctx.current_state = new_state;
        input_ptr++;
        
        if (ctx.current_state == STATE_END) {
            break;
        }
    }
    
    if (ctx.current_state != STATE_END) {
        ctx.current_state = STATE_END;
        handle_end(&ctx, '\0');
    }
    
    return EXIT_SUCCESS;
}

state_t handle_start(context_t *ctx, char input) {
    if (ctx == NULL) {
        return STATE_END;
    }
    
    ctx->buffer_pos = 0;
    ctx->processed_count = 0;
    
    if (input == 'B' || input == 'b') {
        return STATE_READING;
    }
    
    return STATE_START;
}

state_t handle_reading(context_t *ctx, char input) {
    if (ctx == NULL) {
        return STATE_END;
    }
    
    if (input == '\n' || input == '\0') {
        return STATE_PROCESSING;
    }
    
    if (ctx->buffer_pos >= MAX_INPUT_LEN - 1) {
        return STATE_PROCESSING;
    }
    
    if ((input >= 'A' && input <= 'Z') || (input >= 'a' && input <= 'z')) {
        *(ctx->buffer + ctx->buffer_pos) = input;
        ctx->buffer_pos++;
        return STATE_READING;
    }
    
    return STATE_READING;
}

state_t handle_processing(context_t *ctx, char input) {
    if (ctx == NULL) {
        return STATE_END;
    }
    
    if (ctx->buffer_pos == 0) {
        return STATE_END;
    }
    
    *(ctx->buffer + ctx->buffer_pos) = '\0';
    
    char *buf_ptr = ctx->buffer;
    char *buf_end = buf_ptr + ctx->buffer_pos;
    
    while (buf_ptr < buf_end) {
        if (*buf_ptr >= 'a' && *buf_ptr <= 'z') {
            *buf_ptr = *buf_ptr - 'a' + 'A';
        }
        buf_ptr++;
    }
    
    ctx->processed_count++;
    return STATE_OUTPUT;
}

state_t handle_output(context_t *ctx, char input) {
    if (ctx == NULL) {
        return STATE_END;
    }
    
    if (ctx->buffer_pos > 0) {
        char *buf_ptr = ctx->buffer;
        char *buf_end = buf_ptr + ctx->buffer_pos;
        
        while (buf_ptr < buf_end) {
            putchar(*buf_ptr);
            buf_ptr++;
        }
        putchar('\n');
    }
    
    printf("Processed: %d\n", ctx->processed_count);
    
    if (input == 'E' || input == 'e') {
        return STATE_END;
    }
    
    ctx->buffer_pos = 0;
    return STATE_START;
}

state_t handle_end(context_t *ctx, char input) {
    if (ctx == NULL) {
        return STATE_END;
    }
    
    printf("Final count: %d\n", ctx->processed_count);
    return STATE_END;
}