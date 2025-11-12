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

void state_idle(struct context *ctx, char input);
void state_reading(struct context *ctx, char input);
void state_processing(struct context *ctx, char input);
void state_complete(struct context *ctx, char input);
void state_error(struct context *ctx, char input);

void initialize_context(struct context *ctx) {
    if (ctx == NULL) return;
    ctx->current_state = STATE_IDLE;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    ctx->buffer_index = 0;
    ctx->word_count = 0;
    ctx->char_count = 0;
}

int is_valid_input(char c) {
    return isprint(c) || c == '\n' || c == '\t' || c == ' ';
}

void process_input(struct context *ctx, char input) {
    if (ctx == NULL) return;
    
    if (!is_valid_input(input)) {
        ctx->current_state = STATE_ERROR;
        return;
    }

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
        case STATE_COMPLETE:
            state_complete(ctx, input);
            break;
        case STATE_ERROR:
            state_error(ctx, input);
            break;
    }
}

void state_idle(struct context *ctx, char input) {
    if (input == '\n') {
        return;
    }
    
    if (isspace(input)) {
        return;
    }
    
    if (isprint(input)) {
        if (ctx->buffer_index < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->buffer_index++] = input;
            ctx->char_count++;
            ctx->current_state = STATE_READING;
        } else {
            ctx->current_state = STATE_ERROR;
        }
    }
}

void state_reading(struct context *ctx, char input) {
    if (input == '\n') {
        if (ctx->buffer_index > 0) {
            ctx->word_count++;
        }
        ctx->current_state = STATE_PROCESSING;
        return;
    }
    
    if (isspace(input)) {
        if (ctx->buffer_index > 0) {
            ctx->word_count++;
            ctx->buffer_index = 0;
        }
        return;
    }
    
    if (isprint(input)) {
        if (ctx->buffer_index < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->buffer_index++] = input;
            ctx->char_count++;
        } else {
            ctx->current_state = STATE_ERROR;
        }
    }
}

void state_processing(struct context *ctx, char input) {
    if (input == '\n') {
        ctx->current_state = STATE_COMPLETE;
        return;
    }
    
    if (isprint(input) && !isspace(input)) {
        if (ctx->buffer_index < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->buffer_index++] = input;
            ctx->char_count++;
            ctx->current_state = STATE_READING;
        } else {
            ctx->current_state = STATE_ERROR;
        }
    }
}

void state_complete(struct context *ctx, char input) {
    if (input == '\n') {
        return;
    }
    
    if (isprint(input)) {
        ctx->current_state = STATE_ERROR;
    }
}

void state_error(struct context *ctx, char input) {
    return;
}

void print_results(const struct context *ctx) {
    if (ctx == NULL) return;
    
    if (ctx->current_state == STATE_COMPLETE) {
        printf("Processing complete.\n");
        printf("Words counted: %d\n", ctx->word_count);
        printf("Characters counted: %d\n", ctx->char_count);
    } else if (ctx->current_state == STATE_ERROR) {
        printf("Error: Invalid input or buffer overflow detected.\n");
    } else {
        printf("Processing interrupted or incomplete.\n");
    }
}

int main(void) {
    struct context ctx;
    initialize_context(&ctx);
    
    printf("Enter text (empty line to finish):\n");
    
    int c;
    while ((c = getchar()) != EOF) {
        if (c == '\n' && ctx.current_state == STATE_COMPLETE) {
            break;
        }
        
        process_input(&ctx, (char)c);
        
        if (ctx.current_state == STATE_COMPLETE || ctx.current_state == STATE_ERROR) {
            break;
        }
    }
    
    if (ctx.current_state == STATE_READING || ctx.current_state == STATE_PROCESSING) {
        if (ctx.buffer_index > 0) {
            ctx.word_count++;
        }
        ctx.current_state = STATE_COMPLETE;
    }
    
    print_results(&ctx);
    
    return ctx.current_state == STATE_ERROR ? EXIT_FAILURE : EXIT_SUCCESS;
}