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

int is_valid_input(int c) {
    return c == EOF || is_valid_char(c);
}

enum state handle_idle(struct context *ctx, int input) {
    if (!is_valid_input(input)) return STATE_ERROR;
    if (input == EOF) return STATE_DONE;
    if (!is_word_separator(input)) {
        ctx->buffer[ctx->buffer_pos++] = (char)input;
        ctx->char_count++;
        return STATE_READING;
    }
    return STATE_IDLE;
}

enum state handle_reading(struct context *ctx, int input) {
    if (!is_valid_input(input)) return STATE_ERROR;
    if (input == EOF) {
        if (ctx->buffer_pos > 0) {
            ctx->word_count++;
        }
        return STATE_DONE;
    }
    if (is_word_separator(input)) {
        if (ctx->buffer_pos > 0) {
            ctx->word_count++;
            ctx->buffer_pos = 0;
        }
        return STATE_IDLE;
    }
    if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
        ctx->buffer[ctx->buffer_pos++] = (char)input;
        ctx->char_count++;
        return STATE_READING;
    } else {
        return STATE_ERROR;
    }
}

enum state handle_processing(struct context *ctx) {
    if (ctx->buffer_pos > 0) {
        ctx->word_count++;
        ctx->buffer_pos = 0;
    }
    return STATE_DONE;
}

enum state handle_error(struct context *ctx) {
    ctx->buffer_pos = 0;
    return STATE_ERROR;
}

enum state transition(struct context *ctx, int input) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (ctx->current_state) {
        case STATE_IDLE:
            return handle_idle(ctx, input);
        case STATE_READING:
            return handle_reading(ctx, input);
        case STATE_PROCESSING:
            return handle_processing(ctx);
        case STATE_ERROR:
            return handle_error(ctx);
        case STATE_DONE:
            return STATE_DONE;
        default:
            return STATE_ERROR;
    }
}

void print_results(const struct context *ctx) {
    if (ctx == NULL) return;
    printf("Characters: %d\n", ctx->char_count);
    printf("Words: %d\n", ctx->word_count);
}

int main(void) {
    struct context ctx;
    context_init(&ctx);
    
    printf("Enter text (Ctrl+D to end):\n");
    
    int input;
    while (ctx.current_state != STATE_DONE && ctx.current_state != STATE_ERROR) {
        input = getchar();
        if (input == EOF && feof(stdin)) {
            input = EOF;
        } else if (input == EOF) {
            ctx.current_state = STATE_ERROR;
            break;
        }
        
        ctx.current_state = transition(&ctx, input);
    }
    
    if (ctx.current_state == STATE_ERROR) {
        printf("Error: Invalid input or buffer overflow\n");
        return EXIT_FAILURE;
    }
    
    if (ctx.current_state == STATE_READING || ctx.current_state == STATE_PROCESSING) {
        ctx.current_state = transition(&ctx, EOF);
    }
    
    print_results(&ctx);
    return EXIT_SUCCESS;
}