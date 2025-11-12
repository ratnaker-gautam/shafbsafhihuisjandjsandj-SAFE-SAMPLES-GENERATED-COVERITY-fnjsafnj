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
        return (c == EOF) ? STATE_DONE : STATE_IDLE;
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
        ctx->buffer_pos = 0;
        return STATE_PROCESSING;
    }
}

enum state handle_processing(struct context *ctx, int c) {
    if (!is_valid_char(c)) return STATE_ERROR;
    
    if (c == EOF || c == '\n') {
        return (c == EOF) ? STATE_DONE : STATE_IDLE;
    }
    
    if (is_word_char(c)) {
        if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->buffer_pos++] = (char)c;
            ctx->char_count++;
        }
        return STATE_READING;
    }
    
    return STATE_PROCESSING;
}

void process_state(struct context *ctx, int c) {
    enum state next_state = ctx->current_state;
    
    switch (ctx->current_state) {
        case STATE_IDLE:
            next_state = handle_idle(ctx, c);
            break;
        case STATE_READING:
            next_state = handle_reading(ctx, c);
            break;
        case STATE_PROCESSING:
            next_state = handle_processing(ctx, c);
            break;
        case STATE_DONE:
        case STATE_ERROR:
            break;
    }
    
    ctx->current_state = next_state;
}

void print_results(const struct context *ctx) {
    if (ctx == NULL) return;
    
    printf("Words: %d\n", ctx->word_count);
    printf("Characters: %d\n", ctx->char_count);
}

int main(void) {
    struct context ctx;
    context_init(&ctx);
    
    printf("Enter text (Ctrl+D to end):\n");
    
    int c;
    while (ctx.current_state != STATE_DONE && ctx.current_state != STATE_ERROR) {
        c = getchar();
        if (c == EOF && feof(stdin)) {
            c = EOF;
        } else if (c == EOF) {
            ctx.current_state = STATE_ERROR;
            break;
        }
        
        process_state(&ctx, c);
    }
    
    if (ctx.current_state == STATE_ERROR) {
        fprintf(stderr, "Error: Invalid input encountered\n");
        return EXIT_FAILURE;
    }
    
    if (ctx.buffer_pos > 0) {
        ctx.word_count++;
    }
    
    print_results(&ctx);
    return EXIT_SUCCESS;
}