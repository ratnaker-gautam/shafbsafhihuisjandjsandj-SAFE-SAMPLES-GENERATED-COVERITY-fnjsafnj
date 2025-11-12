//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: modular_functions ; Variation: state_machine
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

enum state handle_idle(struct context *ctx, int input_char) {
    if (ctx == NULL) return STATE_ERROR;
    
    if (!is_valid_char(input_char)) return STATE_ERROR;
    
    if (input_char == EOF) return STATE_DONE;
    
    if (isspace(input_char)) return STATE_IDLE;
    
    if (is_word_char(input_char)) {
        if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->buffer_pos++] = (char)input_char;
            ctx->char_count++;
            return STATE_READING;
        }
        return STATE_ERROR;
    }
    
    return STATE_ERROR;
}

enum state handle_reading(struct context *ctx, int input_char) {
    if (ctx == NULL) return STATE_ERROR;
    
    if (!is_valid_char(input_char)) return STATE_ERROR;
    
    if (input_char == EOF) {
        ctx->word_count++;
        return STATE_DONE;
    }
    
    if (is_word_char(input_char)) {
        if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->buffer_pos++] = (char)input_char;
            ctx->char_count++;
            return STATE_READING;
        }
        return STATE_ERROR;
    }
    
    if (isspace(input_char)) {
        ctx->word_count++;
        ctx->buffer_pos = 0;
        return STATE_IDLE;
    }
    
    return STATE_ERROR;
}

enum state handle_processing(struct context *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    if (ctx->word_count > 0 && ctx->char_count > 0) {
        return STATE_DONE;
    }
    
    return STATE_ERROR;
}

enum state handle_error(struct context *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    ctx->word_count = -1;
    ctx->char_count = -1;
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
        case STATE_DONE:
            return STATE_DONE;
        case STATE_ERROR:
            return handle_error(ctx);
        default:
            return STATE_ERROR;
    }
}

void print_results(const struct context *ctx) {
    if (ctx == NULL) return;
    
    if (ctx->current_state == STATE_DONE) {
        printf("Words processed: %d\n", ctx->word_count);
        printf("Characters processed: %d\n", ctx->char_count);
    } else if (ctx->current_state == STATE_ERROR) {
        printf("Error occurred during processing\n");
    } else {
        printf("Processing incomplete\n");
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
            break;
        }
        
        if (ctx.current_state == STATE_DONE) {
            break;
        }
    }
    
    if (input_char == EOF && ctx.current_state != STATE_ERROR) {
        ctx.current_state = process_input(&ctx, EOF);
    }
    
    if (ctx.current_state == STATE_READING || ctx.current_state == STATE_IDLE) {
        ctx.current_state = STATE_PROCESSING;
        ctx.current_state = process_input(&ctx, 0);
    }
    
    print_results(&ctx);
    
    return (ctx.current_state == STATE_DONE) ? EXIT_SUCCESS : EXIT_FAILURE;
}