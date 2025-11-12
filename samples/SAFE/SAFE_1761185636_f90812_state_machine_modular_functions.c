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
    STATE_COMPLETE,
    STATE_ERROR
};

struct context {
    enum state current_state;
    char buffer[256];
    size_t buffer_index;
    int word_count;
    int char_count;
};

void initialize_context(struct context *ctx) {
    if (ctx == NULL) return;
    ctx->current_state = STATE_IDLE;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    ctx->buffer_index = 0;
    ctx->word_count = 0;
    ctx->char_count = 0;
}

int validate_input(const char *input, size_t len) {
    if (input == NULL || len == 0) return 0;
    if (len > 255) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint(input[i]) && input[i] != '\n' && input[i] != '\t') {
            return 0;
        }
    }
    return 1;
}

void handle_idle(struct context *ctx, char input_char) {
    if (ctx == NULL) return;
    if (isspace(input_char)) {
        ctx->current_state = STATE_IDLE;
    } else {
        ctx->buffer[ctx->buffer_index++] = input_char;
        ctx->char_count++;
        ctx->current_state = STATE_READING;
    }
}

void handle_reading(struct context *ctx, char input_char) {
    if (ctx == NULL) return;
    if (isspace(input_char)) {
        ctx->word_count++;
        ctx->buffer_index = 0;
        ctx->current_state = STATE_PROCESSING;
    } else {
        if (ctx->buffer_index < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->buffer_index++] = input_char;
            ctx->char_count++;
        } else {
            ctx->current_state = STATE_ERROR;
        }
    }
}

void handle_processing(struct context *ctx, char input_char) {
    if (ctx == NULL) return;
    if (isspace(input_char)) {
        ctx->current_state = STATE_PROCESSING;
    } else {
        ctx->buffer[ctx->buffer_index++] = input_char;
        ctx->char_count++;
        ctx->current_state = STATE_READING;
    }
}

void process_state_machine(struct context *ctx, const char *input, size_t len) {
    if (ctx == NULL || input == NULL) return;
    if (!validate_input(input, len)) {
        ctx->current_state = STATE_ERROR;
        return;
    }
    
    for (size_t i = 0; i < len && ctx->current_state != STATE_ERROR; i++) {
        switch (ctx->current_state) {
            case STATE_IDLE:
                handle_idle(ctx, input[i]);
                break;
            case STATE_READING:
                handle_reading(ctx, input[i]);
                break;
            case STATE_PROCESSING:
                handle_processing(ctx, input[i]);
                break;
            case STATE_COMPLETE:
            case STATE_ERROR:
                return;
        }
    }
    
    if (ctx->current_state == STATE_READING && ctx->buffer_index > 0) {
        ctx->word_count++;
    }
    
    if (ctx->current_state != STATE_ERROR) {
        ctx->current_state = STATE_COMPLETE;
    }
}

void print_results(const struct context *ctx) {
    if (ctx == NULL) return;
    switch (ctx->current_state) {
        case STATE_COMPLETE:
            printf("Processing complete.\n");
            printf("Words counted: %d\n", ctx->word_count);
            printf("Characters counted: %d\n", ctx->char_count);
            break;
        case STATE_ERROR:
            printf("Error: Invalid input detected.\n");
            break;
        default:
            printf("Processing incomplete.\n");
            break;
    }
}

int main(void) {
    struct context ctx;
    char input_buffer[512];
    size_t input_length;
    
    initialize_context(&ctx);
    
    printf("Enter text to analyze (max 511 characters): ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    input_length = strlen(input_buffer);
    if (input_length > 0 && input_buffer[input_length - 1] == '\n') {
        input_buffer[input_length - 1] = '\0';
        input_length--;
    }
    
    process_state_machine(&ctx, input_buffer, input_length);
    print_results(&ctx);
    
    return ctx.current_state == STATE_ERROR ? 1 : 0;
}