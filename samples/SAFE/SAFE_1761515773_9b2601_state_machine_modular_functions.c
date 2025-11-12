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
    
    if (c == EOF) {
        ctx->word_count++;
        return STATE_DONE;
    }
    
    if (c == '\n') {
        ctx->word_count++;
        return STATE_DONE;
    }
    
    if (is_word_char(c)) {
        if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->buffer_pos++] = (char)c;
            ctx->char_count++;
        }
        return STATE_READING;
    } else {
        ctx->word_count++;
        ctx->buffer_pos = 0;
        memset(ctx->buffer, 0, sizeof(ctx->buffer));
        return STATE_IDLE;
    }
}

enum state handle_processing(struct context *ctx) {
    if (ctx->buffer_pos > 0) {
        ctx->word_count++;
    }
    ctx->buffer_pos = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    return STATE_IDLE;
}

enum state handle_error(struct context *ctx) {
    ctx->buffer_pos = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    return STATE_ERROR;
}

void process_state_machine(struct context *ctx) {
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
        
        if (ctx->current_state == STATE_READING && ctx->buffer_pos >= sizeof(ctx->buffer) - 1) {
            ctx->current_state = STATE_PROCESSING;
        }
    }
}

void print_results(const struct context *ctx) {
    if (ctx == NULL) return;
    
    if (ctx->current_state == STATE_ERROR) {
        printf("Error: Invalid input encountered\n");
        return;
    }
    
    printf("Words: %d\n", ctx->word_count);
    printf("Characters: %d\n", ctx->char_count);
}

int main(void) {
    struct context ctx;
    
    context_init(&ctx);
    process_state_machine(&ctx);
    print_results(&ctx);
    
    if (ctx.current_state == STATE_ERROR) {
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}