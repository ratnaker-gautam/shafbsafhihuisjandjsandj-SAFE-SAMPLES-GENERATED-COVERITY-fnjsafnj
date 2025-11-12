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

int is_word_char(int c) {
    return isalpha(c) || isdigit(c);
}

enum state handle_idle(struct context *ctx, int c) {
    if (!is_valid_char(c)) return STATE_ERROR;
    
    if (c == EOF) return STATE_DONE;
    if (c == '\n') return STATE_DONE;
    
    if (is_word_char(c)) {
        if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->buffer_pos++] = (char)c;
            ctx->char_count++;
        }
        return STATE_READING;
    }
    
    return STATE_IDLE;
}

enum state handle_reading(struct context *ctx, int c) {
    if (!is_valid_char(c)) return STATE_ERROR;
    
    if (c == EOF || c == '\n') {
        if (ctx->buffer_pos > 0) {
            ctx->word_count++;
        }
        return STATE_PROCESSING;
    }
    
    if (is_word_char(c)) {
        if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->buffer_pos++] = (char)c;
            ctx->char_count++;
        }
        return STATE_READING;
    } else {
        if (ctx->buffer_pos > 0) {
            ctx->word_count++;
        }
        memset(ctx->buffer, 0, sizeof(ctx->buffer));
        ctx->buffer_pos = 0;
        return STATE_IDLE;
    }
}

enum state handle_processing(struct context *ctx) {
    printf("Processed: %s\n", ctx->buffer);
    printf("Word count: %d\n", ctx->word_count);
    printf("Character count: %d\n", ctx->char_count);
    
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    ctx->buffer_pos = 0;
    
    return STATE_DONE;
}

enum state handle_error(struct context *ctx) {
    printf("Error: Invalid input encountered\n");
    return STATE_ERROR;
}

void run_state_machine(struct context *ctx) {
    int c;
    
    while (ctx->current_state != STATE_DONE && ctx->current_state != STATE_ERROR) {
        switch (ctx->current_state) {
            case STATE_IDLE:
                c = getchar();
                ctx->current_state = handle_idle(ctx, c);
                break;
                
            case STATE_READING:
                c = getchar();
                ctx->current_state = handle_reading(ctx, c);
                break;
                
            case STATE_PROCESSING:
                ctx->current_state = handle_processing(ctx);
                break;
                
            case STATE_ERROR:
                ctx->current_state = handle_error(ctx);
                break;
                
            case STATE_DONE:
                break;
        }
    }
}

int main(void) {
    struct context ctx;
    
    context_init(&ctx);
    
    printf("Enter text (Ctrl+D or empty line to finish):\n");
    
    run_state_machine(&ctx);
    
    if (ctx.current_state == STATE_DONE) {
        printf("Final statistics:\n");
        printf("Total words: %d\n", ctx.word_count);
        printf("Total characters: %d\n", ctx.char_count);
    }
    
    return ctx.current_state == STATE_DONE ? EXIT_SUCCESS : EXIT_FAILURE;
}