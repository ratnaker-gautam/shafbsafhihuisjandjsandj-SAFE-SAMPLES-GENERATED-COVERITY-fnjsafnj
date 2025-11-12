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

int handle_idle(struct context *ctx, int c) {
    if (!is_valid_char(c)) {
        ctx->current_state = STATE_ERROR;
        return 0;
    }
    
    if (c == EOF) {
        ctx->current_state = STATE_DONE;
        return 0;
    }
    
    if (!is_word_separator(c)) {
        if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->buffer_pos++] = (char)c;
            ctx->char_count++;
            ctx->current_state = STATE_READING;
        } else {
            ctx->current_state = STATE_ERROR;
        }
    }
    return 1;
}

int handle_reading(struct context *ctx, int c) {
    if (!is_valid_char(c)) {
        ctx->current_state = STATE_ERROR;
        return 0;
    }
    
    if (c == EOF) {
        ctx->word_count++;
        ctx->current_state = STATE_DONE;
        return 0;
    }
    
    if (is_word_separator(c)) {
        ctx->word_count++;
        ctx->buffer_pos = 0;
        ctx->current_state = STATE_IDLE;
    } else {
        if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->buffer_pos++] = (char)c;
            ctx->char_count++;
        } else {
            ctx->current_state = STATE_ERROR;
        }
    }
    return 1;
}

int handle_processing(struct context *ctx, int c) {
    if (!is_valid_char(c)) {
        ctx->current_state = STATE_ERROR;
        return 0;
    }
    
    if (c == EOF) {
        ctx->current_state = STATE_DONE;
        return 0;
    }
    
    ctx->current_state = STATE_IDLE;
    return 1;
}

void process_state(struct context *ctx, int c) {
    switch (ctx->current_state) {
        case STATE_IDLE:
            handle_idle(ctx, c);
            break;
        case STATE_READING:
            handle_reading(ctx, c);
            break;
        case STATE_PROCESSING:
            handle_processing(ctx, c);
            break;
        case STATE_DONE:
        case STATE_ERROR:
            break;
    }
}

void print_results(const struct context *ctx) {
    if (ctx == NULL) return;
    
    printf("Processing complete.\n");
    printf("Words counted: %d\n", ctx->word_count);
    printf("Characters counted: %d\n", ctx->char_count);
    
    if (ctx->current_state == STATE_ERROR) {
        printf("Terminated due to error.\n");
    }
}

int main(void) {
    struct context ctx;
    context_init(&ctx);
    
    printf("Enter text (Ctrl+D to end):\n");
    
    int c;
    while (ctx.current_state != STATE_DONE && ctx.current_state != STATE_ERROR) {
        c = getchar();
        if (c == EOF && feof(stdin)) {
            process_state(&ctx, EOF);
        } else if (c == EOF) {
            ctx.current_state = STATE_ERROR;
        } else {
            process_state(&ctx, c);
        }
    }
    
    print_results(&ctx);
    
    if (ctx.current_state == STATE_ERROR) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}