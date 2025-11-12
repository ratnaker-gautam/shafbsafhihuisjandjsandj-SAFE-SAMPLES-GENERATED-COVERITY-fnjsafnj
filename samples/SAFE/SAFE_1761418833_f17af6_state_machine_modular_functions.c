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

void state_idle(struct context *ctx, char input);
void state_reading(struct context *ctx, char input);
void state_processing(struct context *ctx, char input);
void state_done(struct context *ctx, char input);
void state_error(struct context *ctx, char input);

void initialize_context(struct context *ctx) {
    if (ctx == NULL) return;
    ctx->current_state = STATE_IDLE;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    ctx->buffer_pos = 0;
    ctx->word_count = 0;
    ctx->char_count = 0;
}

int is_valid_input(char c) {
    return c >= 0 && c <= 127;
}

void state_idle(struct context *ctx, char input) {
    if (ctx == NULL) return;
    if (!is_valid_input(input)) {
        ctx->current_state = STATE_ERROR;
        return;
    }
    
    if (input == '\n') {
        ctx->current_state = STATE_DONE;
    } else if (!isspace((unsigned char)input)) {
        ctx->buffer[ctx->buffer_pos++] = input;
        ctx->char_count++;
        ctx->current_state = STATE_READING;
    }
}

void state_reading(struct context *ctx, char input) {
    if (ctx == NULL) return;
    if (!is_valid_input(input)) {
        ctx->current_state = STATE_ERROR;
        return;
    }
    
    if (input == '\n') {
        if (ctx->buffer_pos > 0) {
            ctx->word_count++;
        }
        ctx->current_state = STATE_DONE;
    } else if (isspace((unsigned char)input)) {
        if (ctx->buffer_pos > 0) {
            ctx->word_count++;
            ctx->buffer_pos = 0;
            memset(ctx->buffer, 0, sizeof(ctx->buffer));
        }
        ctx->current_state = STATE_PROCESSING;
    } else {
        if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->buffer_pos++] = input;
            ctx->char_count++;
        } else {
            ctx->current_state = STATE_ERROR;
        }
    }
}

void state_processing(struct context *ctx, char input) {
    if (ctx == NULL) return;
    if (!is_valid_input(input)) {
        ctx->current_state = STATE_ERROR;
        return;
    }
    
    if (input == '\n') {
        ctx->current_state = STATE_DONE;
    } else if (!isspace((unsigned char)input)) {
        ctx->buffer[ctx->buffer_pos++] = input;
        ctx->char_count++;
        ctx->current_state = STATE_READING;
    }
}

void state_done(struct context *ctx, char input) {
    if (ctx == NULL) return;
}

void state_error(struct context *ctx, char input) {
    if (ctx == NULL) return;
}

void process_character(struct context *ctx, char input) {
    if (ctx == NULL) return;
    
    switch (ctx->current_state) {
        case STATE_IDLE:
            state_idle(ctx, input);
            break;
        case STATE_READING:
            state_reading(ctx, input);
            break;
        case STATE_PROCESSING:
            state_processing(ctx, input);
            break;
        case STATE_DONE:
            state_done(ctx, input);
            break;
        case STATE_ERROR:
            state_error(ctx, input);
            break;
    }
}

void print_results(const struct context *ctx) {
    if (ctx == NULL) return;
    
    if (ctx->current_state == STATE_ERROR) {
        printf("Error: Invalid input encountered\n");
    } else {
        printf("Words: %d\n", ctx->word_count);
        printf("Characters: %d\n", ctx->char_count);
    }
}

int main(void) {
    struct context ctx;
    initialize_context(&ctx);
    
    printf("Enter text (press Enter to finish): ");
    
    int c;
    while ((c = getchar()) != EOF && ctx.current_state != STATE_DONE && ctx.current_state != STATE_ERROR) {
        if (c == '\n') {
            process_character(&ctx, '\n');
            break;
        }
        process_character(&ctx, (char)c);
    }
    
    if (ctx.current_state == STATE_READING || ctx.current_state == STATE_PROCESSING) {
        if (ctx.buffer_pos > 0) {
            ctx.word_count++;
        }
    }
    
    print_results(&ctx);
    
    return ctx.current_state == STATE_ERROR ? EXIT_FAILURE : EXIT_SUCCESS;
}