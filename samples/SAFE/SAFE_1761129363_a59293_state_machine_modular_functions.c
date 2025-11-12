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
        if (input[i] == '\0') return 0;
        if (!isprint(input[i]) && input[i] != '\n' && input[i] != '\t') return 0;
    }
    return 1;
}

enum state handle_idle(struct context *ctx, const char input) {
    if (ctx == NULL) return STATE_ERROR;
    if (isalnum(input)) {
        ctx->buffer[ctx->buffer_index++] = input;
        ctx->char_count++;
        return STATE_READING;
    }
    return STATE_IDLE;
}

enum state handle_reading(struct context *ctx, const char input) {
    if (ctx == NULL) return STATE_ERROR;
    if (isalnum(input)) {
        if (ctx->buffer_index < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->buffer_index++] = input;
            ctx->char_count++;
            return STATE_READING;
        } else {
            return STATE_ERROR;
        }
    } else {
        ctx->buffer[ctx->buffer_index] = '\0';
        ctx->word_count++;
        ctx->buffer_index = 0;
        return STATE_PROCESSING;
    }
}

enum state handle_processing(struct context *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    printf("Processed word: %s\n", ctx->buffer);
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    return STATE_IDLE;
}

void process_input(struct context *ctx, const char *input, size_t len) {
    if (ctx == NULL || input == NULL || !validate_input(input, len)) {
        if (ctx != NULL) ctx->current_state = STATE_ERROR;
        return;
    }

    for (size_t i = 0; i < len; i++) {
        switch (ctx->current_state) {
            case STATE_IDLE:
                ctx->current_state = handle_idle(ctx, input[i]);
                break;
            case STATE_READING:
                ctx->current_state = handle_reading(ctx, input[i]);
                break;
            case STATE_PROCESSING:
                ctx->current_state = handle_processing(ctx);
                if (ctx->current_state == STATE_IDLE && isalnum(input[i])) {
                    ctx->current_state = handle_idle(ctx, input[i]);
                }
                break;
            case STATE_COMPLETE:
                return;
            case STATE_ERROR:
                return;
            default:
                ctx->current_state = STATE_ERROR;
                return;
        }
        if (ctx->current_state == STATE_ERROR) break;
    }
}

void finalize_processing(struct context *ctx) {
    if (ctx == NULL) return;
    if (ctx->current_state == STATE_READING && ctx->buffer_index > 0) {
        ctx->buffer[ctx->buffer_index] = '\0';
        ctx->word_count++;
        printf("Processed word: %s\n", ctx->buffer);
    }
    ctx->current_state = STATE_COMPLETE;
}

void print_statistics(const struct context *ctx) {
    if (ctx == NULL) return;
    printf("Processing complete.\n");
    printf("Words counted: %d\n", ctx->word_count);
    printf("Characters counted: %d\n", ctx->char_count);
}

int main(void) {
    struct context ctx;
    initialize_context(&ctx);
    
    char input_line[512];
    printf("Enter text to process (empty line to finish):\n");
    
    while (fgets(input_line, sizeof(input_line), stdin) != NULL) {
        size_t len = strlen(input_line);
        if (len > 0 && input_line[len - 1] == '\n') {
            input_line[len - 1] = '\0';
            len--;
        }
        if (len == 0) break;
        
        process_input(&ctx, input_line, len);
        if (ctx.current_state == STATE_ERROR) {
            printf("Error: Invalid input or buffer overflow\n");
            return 1;
        }
    }
    
    finalize_processing(&ctx);
    print_statistics(&ctx);
    
    return 0;
}