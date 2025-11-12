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

void context_init(struct context *ctx) {
    if (ctx == NULL) return;
    ctx->current_state = STATE_IDLE;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    ctx->buffer_pos = 0;
    ctx->word_count = 0;
}

int is_valid_char(char c) {
    return isalpha(c) || c == ' ' || c == '\n' || c == '\t';
}

enum state handle_idle(struct context *ctx, char input) {
    if (ctx == NULL) return STATE_ERROR;
    
    if (is_valid_char(input)) {
        if (isalpha(input)) {
            ctx->buffer[ctx->buffer_pos++] = input;
            if (ctx->buffer_pos >= sizeof(ctx->buffer) - 1) {
                return STATE_ERROR;
            }
            return STATE_READING;
        }
        return STATE_IDLE;
    }
    return STATE_ERROR;
}

enum state handle_reading(struct context *ctx, char input) {
    if (ctx == NULL) return STATE_ERROR;
    
    if (is_valid_char(input)) {
        if (isalpha(input)) {
            ctx->buffer[ctx->buffer_pos++] = input;
            if (ctx->buffer_pos >= sizeof(ctx->buffer) - 1) {
                return STATE_ERROR;
            }
            return STATE_READING;
        } else if (input == ' ' || input == '\n' || input == '\t') {
            if (ctx->buffer_pos > 0) {
                ctx->buffer[ctx->buffer_pos] = '\0';
                return STATE_PROCESSING;
            }
            return STATE_READING;
        }
    }
    return STATE_ERROR;
}

enum state handle_processing(struct context *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    if (ctx->buffer_pos > 0) {
        ctx->word_count++;
    }
    ctx->buffer_pos = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    return STATE_IDLE;
}

void process_input(struct context *ctx, const char *input) {
    if (ctx == NULL || input == NULL) return;
    
    size_t input_len = strlen(input);
    if (input_len == 0) return;
    
    for (size_t i = 0; i < input_len; i++) {
        char c = input[i];
        
        switch (ctx->current_state) {
            case STATE_IDLE:
                ctx->current_state = handle_idle(ctx, c);
                break;
            case STATE_READING:
                ctx->current_state = handle_reading(ctx, c);
                break;
            case STATE_PROCESSING:
                ctx->current_state = handle_processing(ctx);
                if (ctx->current_state == STATE_IDLE) {
                    ctx->current_state = handle_idle(ctx, c);
                }
                break;
            case STATE_DONE:
                return;
            case STATE_ERROR:
                return;
        }
        
        if (ctx->current_state == STATE_ERROR) {
            return;
        }
    }
    
    if (ctx->current_state == STATE_READING && ctx->buffer_pos > 0) {
        ctx->current_state = STATE_PROCESSING;
        ctx->current_state = handle_processing(ctx);
    }
}

int main(void) {
    struct context ctx;
    context_init(&ctx);
    
    char input_line[512];
    
    printf("Enter text (empty line to finish):\n");
    
    while (fgets(input_line, sizeof(input_line), stdin) != NULL) {
        size_t len = strlen(input_line);
        if (len > 0 && input_line[len - 1] == '\n') {
            input_line[len - 1] = '\0';
        }
        
        if (strlen(input_line) == 0) {
            break;
        }
        
        process_input(&ctx, input_line);
        
        if (ctx.current_state == STATE_ERROR) {
            printf("Error: Invalid input detected\n");
            return 1;
        }
        
        memset(input_line, 0, sizeof(input_line));
    }
    
    printf("Word count: %d\n", ctx.word_count);
    
    return 0;
}