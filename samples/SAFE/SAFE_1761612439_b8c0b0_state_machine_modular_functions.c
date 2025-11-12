//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

enum state {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_DONE,
    STATE_ERROR
};

struct context {
    enum state current_state;
    char buffer[256];
    size_t buffer_pos;
    int word_count;
};

void context_init(struct context *ctx) {
    if (ctx == NULL) return;
    ctx->current_state = STATE_IDLE;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    ctx->buffer_pos = 0;
    ctx->word_count = 0;
}

int is_valid_char(char c) {
    return isalpha(c) || c == ' ' || c == '\n' || c == '\t' || c == '\0';
}

enum state handle_idle(struct context *ctx, char input) {
    if (ctx == NULL) return STATE_ERROR;
    
    if (is_valid_char(input)) {
        if (isalpha(input)) {
            if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
                ctx->buffer[ctx->buffer_pos++] = input;
                return STATE_READING;
            }
        }
        return STATE_IDLE;
    }
    return STATE_ERROR;
}

enum state handle_reading(struct context *ctx, char input) {
    if (ctx == NULL) return STATE_ERROR;
    
    if (!is_valid_char(input)) {
        return STATE_ERROR;
    }
    
    if (isalpha(input)) {
        if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->buffer_pos++] = input;
            return STATE_READING;
        } else {
            return STATE_ERROR;
        }
    } else if (input == ' ' || input == '\n' || input == '\t') {
        if (ctx->buffer_pos > 0) {
            ctx->buffer[ctx->buffer_pos] = '\0';
            return STATE_PROCESSING;
        }
        return STATE_READING;
    }
    
    return STATE_ERROR;
}

enum state handle_processing(struct context *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    if (ctx->buffer_pos > 0) {
        ctx->word_count++;
    }
    
    ctx->buffer_pos = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    
    return STATE_IDLE;
}

enum state state_machine_step(struct context *ctx, char input) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (ctx->current_state) {
        case STATE_IDLE:
            return handle_idle(ctx, input);
        case STATE_READING:
            return handle_reading(ctx, input);
        case STATE_PROCESSING:
            return handle_processing(ctx);
        case STATE_DONE:
            return STATE_DONE;
        case STATE_ERROR:
            return STATE_ERROR;
        default:
            return STATE_ERROR;
    }
}

int main(void) {
    struct context ctx;
    context_init(&ctx);
    
    printf("Enter text (Ctrl+D to end): ");
    
    int c;
    while ((c = getchar()) != EOF) {
        if (c == '\0') break;
        
        char input_char = (char)c;
        ctx.current_state = state_machine_step(&ctx, input_char);
        
        if (ctx.current_state == STATE_ERROR) {
            printf("Error: Invalid input character detected\n");
            return 1;
        }
        
        if (ctx.current_state == STATE_PROCESSING) {
            ctx.current_state = state_machine_step(&ctx, ' ');
        }
    }
    
    if (ctx.current_state == STATE_READING && ctx.buffer_pos > 0) {
        ctx.current_state = STATE_PROCESSING;
        ctx.current_state = state_machine_step(&ctx, ' ');
    }
    
    if (ctx.current_state == STATE_ERROR) {
        printf("Error: Invalid input sequence\n");
        return 1;
    }
    
    printf("Word count: %d\n", ctx.word_count);
    return 0;
}