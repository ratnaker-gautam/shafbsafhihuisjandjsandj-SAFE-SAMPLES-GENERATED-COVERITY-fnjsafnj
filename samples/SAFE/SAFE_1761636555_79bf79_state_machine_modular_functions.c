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
            if (ctx->buffer_pos > 0) {
                ctx->word_count++;
            }
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

enum state transition_state(struct context *ctx, char input) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (ctx->current_state) {
        case STATE_IDLE:
            return handle_idle(ctx, input);
        case STATE_READING:
            return handle_reading(ctx, input);
        case STATE_PROCESSING:
            return handle_processing(ctx, input);
        case STATE_DONE:
            return STATE_DONE;
        case STATE_ERROR:
            return STATE_ERROR;
        default:
            return STATE_ERROR;
    }
}

int process_input(struct context *ctx, const char *input, size_t input_len) {
    if (ctx == NULL || input == NULL) return 0;
    
    for (size_t i = 0; i < input_len; i++) {
        ctx->current_state = transition_state(ctx, input[i]);
        if (ctx->current_state == STATE_ERROR) {
            return 0;
        }
    }
    
    if (ctx->current_state == STATE_READING && ctx->buffer_pos > 0) {
        ctx->word_count++;
    }
    
    ctx->current_state = STATE_DONE;
    return 1;
}

int main(void) {
    struct context ctx;
    char input_buffer[512];
    size_t bytes_read;
    
    initialize_context(&ctx);
    
    printf("Enter text (max 511 characters): ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    bytes_read = strlen(input_buffer);
    if (bytes_read > 0 && input_buffer[bytes_read - 1] == '\n') {
        input_buffer[bytes_read - 1] = '\0';
        bytes_read--;
    }
    
    if (bytes_read >= sizeof(input_buffer) - 1) {
        printf("Input too long\n");
        return 1;
    }
    
    if (!process_input(&ctx, input_buffer, bytes_read)) {
        printf("Error processing input\n");
        return 1;
    }
    
    printf("Word count: %d\n", ctx.word_count);
    
    return 0;
}