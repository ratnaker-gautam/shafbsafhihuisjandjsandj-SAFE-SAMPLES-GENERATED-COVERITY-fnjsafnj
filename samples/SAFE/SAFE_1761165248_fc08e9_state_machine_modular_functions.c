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
};

void initialize_context(struct context *ctx) {
    if (ctx == NULL) return;
    ctx->current_state = STATE_IDLE;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    ctx->buffer_pos = 0;
    ctx->word_count = 0;
}

int is_valid_char(char c) {
    return isprint((unsigned char)c) || c == ' ' || c == '\t' || c == '\n';
}

enum state handle_idle(struct context *ctx, char input) {
    if (ctx == NULL) return STATE_ERROR;
    
    if (is_valid_char(input)) {
        if (input == ' ' || input == '\t' || input == '\n') {
            return STATE_IDLE;
        } else {
            if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
                ctx->buffer[ctx->buffer_pos++] = input;
            }
            return STATE_READING;
        }
    }
    return STATE_ERROR;
}

enum state handle_reading(struct context *ctx, char input) {
    if (ctx == NULL) return STATE_ERROR;
    
    if (is_valid_char(input)) {
        if (input == ' ' || input == '\t' || input == '\n') {
            ctx->word_count++;
            return STATE_PROCESSING;
        } else {
            if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
                ctx->buffer[ctx->buffer_pos++] = input;
            }
            return STATE_READING;
        }
    }
    return STATE_ERROR;
}

enum state handle_processing(struct context *ctx, char input) {
    if (ctx == NULL) return STATE_ERROR;
    
    if (is_valid_char(input)) {
        if (input == ' ' || input == '\t' || input == '\n') {
            return STATE_PROCESSING;
        } else {
            ctx->buffer_pos = 0;
            memset(ctx->buffer, 0, sizeof(ctx->buffer));
            if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
                ctx->buffer[ctx->buffer_pos++] = input;
            }
            return STATE_READING;
        }
    }
    return STATE_ERROR;
}

enum state handle_done(struct context *ctx, char input) {
    if (ctx == NULL) return STATE_ERROR;
    return STATE_DONE;
}

enum state handle_error(struct context *ctx, char input) {
    if (ctx == NULL) return STATE_ERROR;
    return STATE_ERROR;
}

enum state process_input(struct context *ctx, char input) {
    if (ctx == NULL) return STATE_ERROR;
    
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
    
    printf("Word count: %d\n", ctx->word_count);
    if (ctx->buffer_pos > 0) {
        printf("Remaining buffer: %s\n", ctx->buffer);
    }
}

int main(void) {
    struct context ctx;
    initialize_context(&ctx);
    
    printf("Enter text (Ctrl+D to end):\n");
    
    int c;
    while ((c = getchar()) != EOF) {
        if (c == '\0') continue;
        
        char input_char = (char)c;
        enum state next_state = process_input(&ctx, input_char);
        
        if (next_state == STATE_ERROR) {
            printf("Error: Invalid input detected\n");
            return 1;
        }
        
        ctx.current_state = next_state;
        
        if (ctx.current_state == STATE_DONE) {
            break;
        }
    }
    
    if (ctx.current_state == STATE_READING && ctx.buffer_pos > 0) {
        ctx.word_count++;
    }
    
    print_results(&ctx);
    
    return 0;
}