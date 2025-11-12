//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

enum state {
    STATE_START,
    STATE_READING,
    STATE_PROCESSING,
    STATE_FINISHED,
    STATE_ERROR
};

struct context {
    enum state current_state;
    char buffer[256];
    size_t buffer_pos;
    int word_count;
    int line_count;
};

void context_init(struct context *ctx) {
    if (ctx == NULL) return;
    ctx->current_state = STATE_START;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    ctx->buffer_pos = 0;
    ctx->word_count = 0;
    ctx->line_count = 0;
}

int is_valid_char(char c) {
    return isprint(c) || c == '\n' || c == '\t' || c == ' ';
}

int handle_start(struct context *ctx, char input) {
    if (ctx == NULL) return 0;
    
    if (input == '\n') {
        ctx->line_count++;
        return 1;
    }
    
    if (is_valid_char(input)) {
        ctx->current_state = STATE_READING;
        if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->buffer_pos++] = input;
        }
        return 1;
    }
    
    ctx->current_state = STATE_ERROR;
    return 0;
}

int handle_reading(struct context *ctx, char input) {
    if (ctx == NULL) return 0;
    
    if (input == '\n') {
        ctx->line_count++;
        ctx->current_state = STATE_PROCESSING;
        return 1;
    }
    
    if (!is_valid_char(input)) {
        ctx->current_state = STATE_ERROR;
        return 0;
    }
    
    if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
        ctx->buffer[ctx->buffer_pos++] = input;
        return 1;
    }
    
    ctx->current_state = STATE_ERROR;
    return 0;
}

int handle_processing(struct context *ctx) {
    if (ctx == NULL) return 0;
    
    if (ctx->buffer_pos == 0) {
        ctx->current_state = STATE_FINISHED;
        return 1;
    }
    
    int in_word = 0;
    for (size_t i = 0; i < ctx->buffer_pos; i++) {
        if (isspace(ctx->buffer[i])) {
            in_word = 0;
        } else if (!in_word) {
            ctx->word_count++;
            in_word = 1;
        }
    }
    
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    ctx->buffer_pos = 0;
    
    ctx->current_state = STATE_READING;
    return 1;
}

int process_input(struct context *ctx, char input) {
    if (ctx == NULL) return 0;
    
    switch (ctx->current_state) {
        case STATE_START:
            return handle_start(ctx, input);
        case STATE_READING:
            return handle_reading(ctx, input);
        case STATE_PROCESSING:
            return handle_processing(ctx);
        case STATE_FINISHED:
            return 1;
        case STATE_ERROR:
            return 0;
        default:
            ctx->current_state = STATE_ERROR;
            return 0;
    }
}

void print_results(const struct context *ctx) {
    if (ctx == NULL) return;
    
    if (ctx->current_state == STATE_ERROR) {
        printf("Error: Invalid input encountered\n");
        return;
    }
    
    printf("Processing complete:\n");
    printf("Lines processed: %d\n", ctx->line_count);
    printf("Words counted: %d\n", ctx->word_count);
    printf("Final state: %s\n", 
           ctx->current_state == STATE_FINISHED ? "FINISHED" : "READING");
}

int main(void) {
    struct context ctx;
    context_init(&ctx);
    
    printf("Enter text (Ctrl+D to finish):\n");
    
    int input_char;
    while ((input_char = getchar()) != EOF) {
        if (input_char == 4) break;
        
        if (!process_input(&ctx, (char)input_char)) {
            break;
        }
        
        if (ctx.current_state == STATE_FINISHED) {
            break;
        }
    }
    
    if (ctx.current_state == STATE_READING && ctx.buffer_pos > 0) {
        ctx.current_state = STATE_PROCESSING;
        handle_processing(&ctx);
        ctx.current_state = STATE_FINISHED;
    } else if (ctx.current_state == STATE_READING) {
        ctx.current_state = STATE_FINISHED;
    }
    
    print_results(&ctx);
    
    return ctx.current_state == STATE_ERROR ? 1 : 0;
}