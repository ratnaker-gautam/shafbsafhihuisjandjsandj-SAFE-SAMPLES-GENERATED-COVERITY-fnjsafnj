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
    int line_count;
};

void context_init(struct context *ctx) {
    if (ctx == NULL) return;
    ctx->current_state = STATE_IDLE;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    ctx->buffer_pos = 0;
    ctx->word_count = 0;
    ctx->line_count = 0;
}

int is_whitespace(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

int is_valid_char(char c) {
    return (c >= 32 && c <= 126) || c == '\n' || c == '\t' || c == '\r';
}

enum state handle_idle(struct context *ctx, char input) {
    if (!is_valid_char(input)) {
        return STATE_ERROR;
    }
    if (input == '\n') {
        ctx->line_count++;
        return STATE_IDLE;
    }
    if (!is_whitespace(input)) {
        if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->buffer_pos++] = input;
        }
        return STATE_READING;
    }
    return STATE_IDLE;
}

enum state handle_reading(struct context *ctx, char input) {
    if (!is_valid_char(input)) {
        return STATE_ERROR;
    }
    if (is_whitespace(input)) {
        if (input == '\n') {
            ctx->line_count++;
        }
        ctx->word_count++;
        ctx->buffer_pos = 0;
        return STATE_PROCESSING;
    }
    if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
        ctx->buffer[ctx->buffer_pos++] = input;
        return STATE_READING;
    }
    return STATE_ERROR;
}

enum state handle_processing(struct context *ctx, char input) {
    if (!is_valid_char(input)) {
        return STATE_ERROR;
    }
    if (input == '\n') {
        ctx->line_count++;
    }
    if (is_whitespace(input)) {
        return STATE_PROCESSING;
    }
    if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
        ctx->buffer[ctx->buffer_pos++] = input;
        return STATE_READING;
    }
    return STATE_ERROR;
}

enum state handle_done(struct context *ctx, char input) {
    return STATE_DONE;
}

enum state handle_error(struct context *ctx, char input) {
    return STATE_ERROR;
}

enum state process_input(struct context *ctx, char input) {
    if (ctx == NULL) {
        return STATE_ERROR;
    }
    switch (ctx->current_state) {
        case STATE_IDLE:
            return handle_idle(ctx, input);
        case STATE_READING:
            return handle_reading(ctx, input);
        case STATE_PROCESSING:
            return handle_processing(ctx, input);
        case STATE_DONE:
            return handle_done(ctx, input);
        case STATE_ERROR:
            return handle_error(ctx, input);
        default:
            return STATE_ERROR;
    }
}

void print_results(const struct context *ctx) {
    if (ctx == NULL) return;
    printf("Words: %d\n", ctx->word_count);
    printf("Lines: %d\n", ctx->line_count);
}

int main(void) {
    struct context ctx;
    context_init(&ctx);
    
    printf("Enter text (Ctrl+D to end):\n");
    
    int c;
    while ((c = getchar()) != EOF) {
        if (c == 0 || c == EOF) {
            break;
        }
        char input = (char)c;
        ctx.current_state = process_input(&ctx, input);
        if (ctx.current_state == STATE_ERROR) {
            fprintf(stderr, "Error: Invalid input encountered\n");
            return 1;
        }
    }
    
    if (ctx.current_state == STATE_READING) {
        ctx.word_count++;
    }
    if (ctx.buffer_pos > 0) {
        ctx.word_count++;
    }
    
    ctx.current_state = STATE_DONE;
    print_results(&ctx);
    
    return 0;
}