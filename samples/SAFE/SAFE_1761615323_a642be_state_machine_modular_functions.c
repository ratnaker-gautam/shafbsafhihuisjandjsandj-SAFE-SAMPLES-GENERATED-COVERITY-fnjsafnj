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
    int char_count;
};

void context_init(struct context *ctx) {
    if (ctx == NULL) return;
    ctx->current_state = STATE_IDLE;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    ctx->buffer_pos = 0;
    ctx->word_count = 0;
    ctx->char_count = 0;
}

int is_valid_char(char c) {
    return isprint(c) || c == '\n' || c == '\t' || c == ' ';
}

int handle_idle(struct context *ctx, char input) {
    if (ctx == NULL) return 0;
    
    if (is_valid_char(input)) {
        if (input != '\n' && input != ' ') {
            ctx->current_state = STATE_READING;
            if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
                ctx->buffer[ctx->buffer_pos++] = input;
                ctx->char_count++;
            }
        }
        return 1;
    }
    return 0;
}

int handle_reading(struct context *ctx, char input) {
    if (ctx == NULL) return 0;
    
    if (!is_valid_char(input)) {
        ctx->current_state = STATE_ERROR;
        return 0;
    }
    
    if (input == '\n' || input == ' ') {
        ctx->current_state = STATE_PROCESSING;
        return 1;
    }
    
    if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
        ctx->buffer[ctx->buffer_pos++] = input;
        ctx->char_count++;
        return 1;
    }
    
    ctx->current_state = STATE_ERROR;
    return 0;
}

int handle_processing(struct context *ctx) {
    if (ctx == NULL) return 0;
    
    if (ctx->buffer_pos > 0) {
        ctx->word_count++;
        ctx->buffer_pos = 0;
        memset(ctx->buffer, 0, sizeof(ctx->buffer));
    }
    
    ctx->current_state = STATE_IDLE;
    return 1;
}

int process_input(struct context *ctx, char input) {
    if (ctx == NULL) return 0;
    
    switch (ctx->current_state) {
        case STATE_IDLE:
            return handle_idle(ctx, input);
        case STATE_READING:
            return handle_reading(ctx, input);
        case STATE_PROCESSING:
            return handle_processing(ctx);
        case STATE_DONE:
            return 1;
        case STATE_ERROR:
            return 0;
        default:
            ctx->current_state = STATE_ERROR;
            return 0;
    }
}

int main(void) {
    struct context ctx;
    context_init(&ctx);
    
    printf("Enter text (Ctrl+D to end):\n");
    
    int c;
    while ((c = getchar()) != EOF) {
        if (c == 4) break;
        
        if (!process_input(&ctx, (char)c)) {
            printf("Error: Invalid input or state machine error\n");
            return 1;
        }
        
        if (ctx.current_state == STATE_ERROR) {
            printf("Error: Invalid input sequence\n");
            return 1;
        }
    }
    
    if (ctx.current_state == STATE_READING && ctx.buffer_pos > 0) {
        ctx.word_count++;
    }
    
    printf("Words: %d\n", ctx.word_count);
    printf("Characters: %d\n", ctx.char_count);
    
    return 0;
}