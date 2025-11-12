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

int validate_input(const char *input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len >= 255) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint((unsigned char)input[i])) return 0;
    }
    return 1;
}

void handle_idle(struct context *ctx, const char *input) {
    if (ctx == NULL || input == NULL) return;
    if (validate_input(input)) {
        ctx->current_state = STATE_READING;
        strncpy(ctx->buffer, input, sizeof(ctx->buffer) - 1);
        ctx->buffer[sizeof(ctx->buffer) - 1] = '\0';
        ctx->buffer_index = strlen(ctx->buffer);
    } else {
        ctx->current_state = STATE_ERROR;
    }
}

void handle_reading(struct context *ctx) {
    if (ctx == NULL) return;
    ctx->current_state = STATE_PROCESSING;
}

void handle_processing(struct context *ctx) {
    if (ctx == NULL) return;
    int in_word = 0;
    ctx->word_count = 0;
    ctx->char_count = 0;
    
    for (size_t i = 0; i < ctx->buffer_index; i++) {
        if (isalnum((unsigned char)ctx->buffer[i])) {
            ctx->char_count++;
            if (!in_word) {
                ctx->word_count++;
                in_word = 1;
            }
        } else {
            in_word = 0;
        }
    }
    
    ctx->current_state = STATE_COMPLETE;
}

void handle_complete(struct context *ctx) {
    if (ctx == NULL) return;
    printf("Processing complete:\n");
    printf("Characters: %d\n", ctx->char_count);
    printf("Words: %d\n", ctx->word_count);
    printf("Buffer: %s\n", ctx->buffer);
    ctx->current_state = STATE_IDLE;
    initialize_context(ctx);
}

void handle_error(struct context *ctx) {
    if (ctx == NULL) return;
    printf("Error: Invalid input detected\n");
    ctx->current_state = STATE_IDLE;
    initialize_context(ctx);
}

void run_state_machine(struct context *ctx, const char *input) {
    if (ctx == NULL) return;
    
    switch (ctx->current_state) {
        case STATE_IDLE:
            handle_idle(ctx, input);
            break;
        case STATE_READING:
            handle_reading(ctx);
            break;
        case STATE_PROCESSING:
            handle_processing(ctx);
            break;
        case STATE_COMPLETE:
            handle_complete(ctx);
            break;
        case STATE_ERROR:
            handle_error(ctx);
            break;
        default:
            ctx->current_state = STATE_ERROR;
            break;
    }
}

int main(void) {
    struct context ctx;
    initialize_context(&ctx);
    char input[256];
    
    printf("Text Processing State Machine\n");
    printf("Enter text (or 'quit' to exit):\n");
    
    while (1) {
        printf("> ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }
        
        if (strcmp(input, "quit") == 0) {
            break;
        }
        
        run_state_machine(&ctx, input);
    }
    
    printf("Goodbye!\n");
    return 0;
}