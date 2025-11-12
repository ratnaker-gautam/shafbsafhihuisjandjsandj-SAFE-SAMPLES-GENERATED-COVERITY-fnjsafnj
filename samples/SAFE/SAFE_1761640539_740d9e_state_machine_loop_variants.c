//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

enum state {
    STATE_START,
    STATE_READING,
    STATE_PROCESSING,
    STATE_OUTPUT,
    STATE_ERROR,
    STATE_DONE
};

struct context {
    enum state current_state;
    char buffer[256];
    size_t buffer_pos;
    int word_count;
    int char_count;
};

void state_machine_init(struct context *ctx) {
    ctx->current_state = STATE_START;
    ctx->buffer_pos = 0;
    ctx->word_count = 0;
    ctx->char_count = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
}

int is_valid_input(char c) {
    return isprint(c) || c == '\n' || c == '\t' || c == ' ';
}

enum state handle_start(struct context *ctx, char input) {
    if (input == '\n') {
        return STATE_DONE;
    }
    if (is_valid_input(input)) {
        if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->buffer_pos++] = input;
            ctx->char_count++;
        }
        return STATE_READING;
    }
    return STATE_ERROR;
}

enum state handle_reading(struct context *ctx, char input) {
    if (input == '\n') {
        return STATE_PROCESSING;
    }
    if (is_valid_input(input)) {
        if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->buffer_pos++] = input;
            ctx->char_count++;
        } else {
            return STATE_ERROR;
        }
    } else {
        return STATE_ERROR;
    }
    return STATE_READING;
}

enum state handle_processing(struct context *ctx) {
    int in_word = 0;
    for (size_t i = 0; i < ctx->buffer_pos; i++) {
        if (ctx->buffer[i] == ' ' || ctx->buffer[i] == '\t') {
            in_word = 0;
        } else if (!in_word) {
            in_word = 1;
            ctx->word_count++;
        }
    }
    return STATE_OUTPUT;
}

enum state handle_output(struct context *ctx) {
    printf("Characters: %d\n", ctx->char_count);
    printf("Words: %d\n", ctx->word_count);
    return STATE_DONE;
}

enum state handle_error(struct context *ctx) {
    printf("Error: Invalid input detected\n");
    return STATE_DONE;
}

int main(void) {
    struct context ctx;
    state_machine_init(&ctx);
    
    printf("Enter text (max 255 chars, newline to finish):\n");
    
    while (ctx.current_state != STATE_DONE && ctx.current_state != STATE_ERROR) {
        char input;
        int result = scanf("%c", &input);
        if (result != 1) {
            ctx.current_state = STATE_ERROR;
            break;
        }
        
        switch (ctx.current_state) {
            case STATE_START:
                ctx.current_state = handle_start(&ctx, input);
                break;
            case STATE_READING:
                ctx.current_state = handle_reading(&ctx, input);
                break;
            case STATE_PROCESSING:
                ctx.current_state = handle_processing(&ctx);
                break;
            case STATE_OUTPUT:
                ctx.current_state = handle_output(&ctx);
                break;
            case STATE_ERROR:
                ctx.current_state = handle_error(&ctx);
                break;
            case STATE_DONE:
                break;
        }
    }
    
    if (ctx.current_state == STATE_ERROR) {
        handle_error(&ctx);
    }
    
    return 0;
}