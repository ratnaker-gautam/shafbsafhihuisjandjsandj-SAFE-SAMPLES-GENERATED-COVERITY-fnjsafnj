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

int is_valid_input_char(char c) {
    return isalnum(c) || isspace(c) || c == '.' || c == ',' || c == '!' || c == '?';
}

enum state handle_idle(struct context *ctx, char input) {
    if (ctx == NULL) return STATE_ERROR;
    
    if (is_valid_input_char(input)) {
        if (!isspace(input)) {
            if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
                ctx->buffer[ctx->buffer_pos++] = input;
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
    
    if (!is_valid_input_char(input)) {
        return STATE_ERROR;
    }
    
    if (isspace(input)) {
        if (ctx->buffer_pos > 0) {
            ctx->buffer[ctx->buffer_pos] = '\0';
            return STATE_PROCESSING;
        }
    } else {
        if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->buffer_pos++] = input;
            return STATE_READING;
        } else {
            return STATE_ERROR;
        }
    }
    return STATE_READING;
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

enum state handle_done(struct context *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    return STATE_DONE;
}

enum state handle_error(struct context *ctx) {
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
            return handle_processing(ctx);
        case STATE_DONE:
            return handle_done(ctx);
        case STATE_ERROR:
            return handle_error(ctx);
        default:
            return STATE_ERROR;
    }
}

void print_results(const struct context *ctx) {
    if (ctx == NULL) return;
    
    printf("Word count: %d\n", ctx->word_count);
    printf("Final state: ");
    
    switch (ctx->current_state) {
        case STATE_IDLE:
            printf("IDLE\n");
            break;
        case STATE_READING:
            printf("READING\n");
            break;
        case STATE_PROCESSING:
            printf("PROCESSING\n");
            break;
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
    
    printf("Enter text (press Enter then Ctrl+D to finish):\n");
    
    int input_char;
    while ((input_char = getchar()) != EOF) {
        if (input_char == '\n') {
            input_char = ' ';
        }
        
        ctx.current_state = process_input(&ctx, (char)input_char);
        
        if (ctx.current_state == STATE_ERROR) {
            printf("Error: Invalid input or buffer overflow\n");
            break;
        }
        
        if (ctx.current_state == STATE_DONE) {
            break;
        }
    }
    
    if (ctx.current_state != STATE_ERROR) {
        if (ctx.current_state == STATE_READING && ctx.buffer_pos > 0) {
            ctx.current_state = process_input(&ctx, ' ');
        }
        
        if (ctx.current_state == STATE_PROCESSING) {
            ctx.current_state = process_input(&ctx, ' ');
        }
        
        if (ctx.current_state != STATE_ERROR) {
            ctx.current_state = STATE_DONE;
        }
    }
    
    print_results(&ctx);
    
    return (ctx.current_state == STATE_ERROR) ? EXIT_FAILURE : EXIT_SUCCESS;
}