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

enum state handle_idle(struct context *ctx, int input_char) {
    if (ctx == NULL) return STATE_ERROR;
    
    if (!is_valid_char(input_char)) return STATE_ERROR;
    
    if (input_char == EOF) return STATE_DONE;
    
    if (!is_word_separator(input_char)) {
        ctx->buffer[ctx->buffer_pos++] = (char)input_char;
        ctx->char_count++;
        return STATE_READING;
    }
    
    return STATE_IDLE;
}

enum state handle_reading(struct context *ctx, int input_char) {
    if (ctx == NULL) return STATE_ERROR;
    
    if (!is_valid_char(input_char)) return STATE_ERROR;
    
    if (input_char == EOF) {
        if (ctx->buffer_pos > 0) {
            ctx->word_count++;
        }
        return STATE_DONE;
    }
    
    if (is_word_separator(input_char)) {
        if (ctx->buffer_pos > 0) {
            ctx->word_count++;
        }
        ctx->buffer_pos = 0;
        memset(ctx->buffer, 0, sizeof(ctx->buffer));
        return STATE_IDLE;
    }
    
    if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
        ctx->buffer[ctx->buffer_pos++] = (char)input_char;
        ctx->char_count++;
        return STATE_READING;
    }
    
    return STATE_ERROR;
}

enum state handle_processing(struct context *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    if (ctx->buffer_pos > 0) {
        ctx->word_count++;
        ctx->buffer_pos = 0;
        memset(ctx->buffer, 0, sizeof(ctx->buffer));
    }
    
    return STATE_DONE;
}

enum state handle_error(struct context *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    ctx->buffer_pos = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    return STATE_ERROR;
}

enum state process_input(struct context *ctx, int input_char) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (ctx->current_state) {
        case STATE_IDLE:
            return handle_idle(ctx, input_char);
        case STATE_READING:
            return handle_reading(ctx, input_char);
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
    
    printf("Words counted: %d\n", ctx->word_count);
    printf("Characters counted: %d\n", ctx->char_count);
    
    switch (ctx->current_state) {
        case STATE_DONE:
            printf("State: Completed successfully\n");
            break;
        case STATE_ERROR:
            printf("State: Error occurred\n");
            break;
        default:
            printf("State: Unexpected state\n");
            break;
    }
}

int main(void) {
    struct context ctx;
    context_init(&ctx);
    
    printf("Enter text (Ctrl+D to end input):\n");
    
    int input_char;
    while ((input_char = getchar()) != EOF) {
        ctx.current_state = process_input(&ctx, input_char);
        
        if (ctx.current_state == STATE_ERROR) {
            printf("Error: Invalid input or buffer overflow\n");
            break;
        }
        
        if (ctx.current_state == STATE_DONE) {
            break;
        }
    }
    
    if (ctx.current_state != STATE_ERROR && ctx.current_state != STATE_DONE) {
        ctx.current_state = process_input(&ctx, EOF);
    }
    
    print_results(&ctx);
    
    return (ctx.current_state == STATE_ERROR) ? EXIT_FAILURE : EXIT_SUCCESS;
}