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

int is_valid_char(int c) {
    return c >= 0 && c <= 127;
}

int is_word_separator(int c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

int handle_idle(struct context *ctx, int input_char) {
    if (ctx == NULL) return -1;
    if (!is_valid_char(input_char)) return -1;
    
    if (input_char == EOF) {
        ctx->current_state = STATE_DONE;
        return 0;
    }
    
    if (!is_word_separator(input_char)) {
        if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->buffer_pos++] = (char)input_char;
            ctx->char_count++;
            ctx->current_state = STATE_READING;
        } else {
            ctx->current_state = STATE_ERROR;
        }
    }
    return 0;
}

int handle_reading(struct context *ctx, int input_char) {
    if (ctx == NULL) return -1;
    if (!is_valid_char(input_char)) return -1;
    
    if (input_char == EOF) {
        if (ctx->buffer_pos > 0) {
            ctx->word_count++;
        }
        ctx->current_state = STATE_DONE;
        return 0;
    }
    
    if (is_word_separator(input_char)) {
        if (ctx->buffer_pos > 0) {
            ctx->word_count++;
            ctx->current_state = STATE_PROCESSING;
        }
    } else {
        if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->buffer_pos++] = (char)input_char;
            ctx->char_count++;
        } else {
            ctx->current_state = STATE_ERROR;
        }
    }
    return 0;
}

int handle_processing(struct context *ctx, int input_char) {
    if (ctx == NULL) return -1;
    if (!is_valid_char(input_char)) return -1;
    
    ctx->buffer_pos = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    
    if (input_char == EOF) {
        ctx->current_state = STATE_DONE;
        return 0;
    }
    
    if (!is_word_separator(input_char)) {
        if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->buffer_pos++] = (char)input_char;
            ctx->char_count++;
            ctx->current_state = STATE_READING;
        } else {
            ctx->current_state = STATE_ERROR;
        }
    } else {
        ctx->current_state = STATE_PROCESSING;
    }
    return 0;
}

int state_machine_step(struct context *ctx, int input_char) {
    if (ctx == NULL) return -1;
    
    switch (ctx->current_state) {
        case STATE_IDLE:
            return handle_idle(ctx, input_char);
        case STATE_READING:
            return handle_reading(ctx, input_char);
        case STATE_PROCESSING:
            return handle_processing(ctx, input_char);
        case STATE_DONE:
        case STATE_ERROR:
            return 0;
        default:
            ctx->current_state = STATE_ERROR;
            return -1;
    }
}

int main(void) {
    struct context ctx;
    context_init(&ctx);
    
    printf("Enter text (Ctrl+D to end):\n");
    
    int input_char;
    while (ctx.current_state != STATE_DONE && ctx.current_state != STATE_ERROR) {
        input_char = getchar();
        if (state_machine_step(&ctx, input_char) != 0) {
            ctx.current_state = STATE_ERROR;
            break;
        }
    }
    
    if (ctx.current_state == STATE_ERROR) {
        printf("Error: Processing failed\n");
        return 1;
    }
    
    printf("Results:\n");
    printf("Characters: %d\n", ctx.char_count);
    printf("Words: %d\n", ctx.word_count);
    
    return 0;
}