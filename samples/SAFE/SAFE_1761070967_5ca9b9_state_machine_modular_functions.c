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
    STATE_COMPLETE,
    STATE_ERROR
};

struct context {
    enum state current_state;
    char buffer[256];
    size_t buffer_index;
    int word_count;
};

void initialize_context(struct context *ctx) {
    if (ctx == NULL) return;
    ctx->current_state = STATE_IDLE;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    ctx->buffer_index = 0;
    ctx->word_count = 0;
}

int is_valid_input(char c) {
    return isprint(c) || c == '\n' || c == '\t' || c == ' ';
}

enum state handle_idle(struct context *ctx, char input) {
    if (ctx == NULL) return STATE_ERROR;
    
    if (is_valid_input(input)) {
        if (input == '\n') {
            return STATE_IDLE;
        } else if (!isspace(input)) {
            if (ctx->buffer_index < sizeof(ctx->buffer) - 1) {
                ctx->buffer[ctx->buffer_index++] = input;
                return STATE_READING;
            } else {
                return STATE_ERROR;
            }
        }
    }
    return STATE_IDLE;
}

enum state handle_reading(struct context *ctx, char input) {
    if (ctx == NULL) return STATE_ERROR;
    
    if (is_valid_input(input)) {
        if (input == '\n') {
            if (ctx->buffer_index > 0) {
                ctx->word_count++;
                ctx->buffer_index = 0;
                memset(ctx->buffer, 0, sizeof(ctx->buffer));
                return STATE_COMPLETE;
            }
            return STATE_IDLE;
        } else if (isspace(input)) {
            if (ctx->buffer_index > 0) {
                ctx->word_count++;
                ctx->buffer_index = 0;
                memset(ctx->buffer, 0, sizeof(ctx->buffer));
                return STATE_PROCESSING;
            }
        } else {
            if (ctx->buffer_index < sizeof(ctx->buffer) - 1) {
                ctx->buffer[ctx->buffer_index++] = input;
                return STATE_READING;
            } else {
                return STATE_ERROR;
            }
        }
    }
    return STATE_READING;
}

enum state handle_processing(struct context *ctx, char input) {
    if (ctx == NULL) return STATE_ERROR;
    
    if (is_valid_input(input)) {
        if (input == '\n') {
            return STATE_COMPLETE;
        } else if (!isspace(input)) {
            if (ctx->buffer_index < sizeof(ctx->buffer) - 1) {
                ctx->buffer[ctx->buffer_index++] = input;
                return STATE_READING;
            } else {
                return STATE_ERROR;
            }
        }
    }
    return STATE_PROCESSING;
}

enum state handle_complete(struct context *ctx, char input) {
    if (ctx == NULL) return STATE_ERROR;
    
    if (is_valid_input(input)) {
        if (input == '\n') {
            return STATE_IDLE;
        } else if (!isspace(input)) {
            if (ctx->buffer_index < sizeof(ctx->buffer) - 1) {
                ctx->buffer[ctx->buffer_index++] = input;
                return STATE_READING;
            } else {
                return STATE_ERROR;
            }
        }
    }
    return STATE_COMPLETE;
}

enum state handle_error(struct context *ctx, char input) {
    if (ctx == NULL) return STATE_ERROR;
    
    if (input == '\n') {
        initialize_context(ctx);
        return STATE_IDLE;
    }
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
        case STATE_COMPLETE:
            return handle_complete(ctx, input);
        case STATE_ERROR:
            return handle_error(ctx, input);
        default:
            return STATE_ERROR;
    }
}

int main(void) {
    struct context ctx;
    initialize_context(&ctx);
    
    printf("Enter text (press Ctrl+D to finish):\n");
    
    int c;
    while ((c = getchar()) != EOF) {
        if (c == 4) break;
        
        enum state new_state = process_input(&ctx, (char)c);
        ctx.current_state = new_state;
        
        if (new_state == STATE_ERROR) {
            printf("Error: Buffer overflow or invalid input\n");
            initialize_context(&ctx);
        } else if (new_state == STATE_COMPLETE) {
            printf("Word count: %d\n", ctx.word_count);
            ctx.word_count = 0;
        }
    }
    
    if (ctx.buffer_index > 0 && ctx.current_state != STATE_ERROR) {
        ctx.word_count++;
        printf("Final word count: %d\n", ctx.word_count);
    }
    
    return 0;
}