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

void initialize_context(struct context *ctx) {
    if (ctx == NULL) return;
    ctx->current_state = STATE_IDLE;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    ctx->buffer_pos = 0;
    ctx->word_count = 0;
    ctx->char_count = 0;
}

int is_valid_input(char c) {
    return isprint(c) || c == '\n' || c == '\t' || c == ' ';
}

enum state handle_idle(struct context *ctx, char input) {
    if (ctx == NULL) return STATE_ERROR;
    
    if (input == '\n') {
        return STATE_DONE;
    } else if (is_valid_input(input)) {
        ctx->buffer[ctx->buffer_pos++] = input;
        ctx->char_count++;
        return STATE_READING;
    }
    return STATE_ERROR;
}

enum state handle_reading(struct context *ctx, char input) {
    if (ctx == NULL) return STATE_ERROR;
    
    if (input == '\n') {
        return STATE_PROCESSING;
    } else if (is_valid_input(input)) {
        if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->buffer_pos++] = input;
            ctx->char_count++;
            return STATE_READING;
        } else {
            return STATE_ERROR;
        }
    }
    return STATE_ERROR;
}

enum state handle_processing(struct context *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    if (ctx->buffer_pos > 0) {
        int in_word = 0;
        for (size_t i = 0; i < ctx->buffer_pos; i++) {
            if (isspace(ctx->buffer[i])) {
                in_word = 0;
            } else if (!in_word) {
                ctx->word_count++;
                in_word = 1;
            }
        }
    }
    return STATE_DONE;
}

enum state process_input(struct context *ctx, char input) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (ctx->current_state) {
        case STATE_IDLE:
            return handle_idle(ctx, input);
        case STATE_READING:
            return handle_reading(ctx, input);
        case STATE_PROCESSING:
            return handle_processing(ctx);
        case STATE_DONE:
        case STATE_ERROR:
        default:
            return ctx->current_state;
    }
}

void print_results(const struct context *ctx) {
    if (ctx == NULL) return;
    
    printf("Processing complete.\n");
    printf("Characters processed: %d\n", ctx->char_count);
    printf("Words counted: %d\n", ctx->word_count);
    printf("Final state: ");
    
    switch (ctx->current_state) {
        case STATE_DONE:
            printf("DONE\n");
            break;
        case STATE_ERROR:
            printf("ERROR\n");
            break;
        default:
            printf("UNKNOWN\n");
            break;
    }
}

int main(void) {
    struct context ctx;
    initialize_context(&ctx);
    
    printf("Enter text (press Enter twice to finish):\n");
    
    int input_char;
    while ((input_char = getchar()) != EOF) {
        if (input_char == '\n' && ctx.current_state == STATE_READING) {
            ctx.current_state = process_input(&ctx, '\n');
            if (ctx.current_state == STATE_PROCESSING) {
                ctx.current_state = process_input(&ctx, '\n');
            }
        } else {
            ctx.current_state = process_input(&ctx, (char)input_char);
        }
        
        if (ctx.current_state == STATE_DONE || ctx.current_state == STATE_ERROR) {
            break;
        }
    }
    
    if (ctx.current_state == STATE_READING) {
        ctx.current_state = process_input(&ctx, '\n');
        if (ctx.current_state == STATE_PROCESSING) {
            ctx.current_state = process_input(&ctx, '\n');
        }
    }
    
    print_results(&ctx);
    
    return (ctx.current_state == STATE_DONE) ? EXIT_SUCCESS : EXIT_FAILURE;
}