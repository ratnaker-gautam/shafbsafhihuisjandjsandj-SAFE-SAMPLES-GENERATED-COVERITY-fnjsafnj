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

void transition_state(struct context *ctx, enum state new_state) {
    if (ctx == NULL) return;
    ctx->current_state = new_state;
}

int is_valid_input(char c) {
    return c >= 0 && c <= 127;
}

void process_character(struct context *ctx, char input) {
    if (ctx == NULL || !is_valid_input(input)) {
        if (ctx != NULL) transition_state(ctx, STATE_ERROR);
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
        case STATE_DONE:
            state_done(ctx, input);
            break;
        case STATE_ERROR:
            state_error(ctx, input);
            break;
    }
}

void state_idle(struct context *ctx, char input) {
    if (ctx == NULL) return;
    
    if (input == '\n') {
        transition_state(ctx, STATE_DONE);
        return;
    }
    
    if (isalnum((unsigned char)input)) {
        if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->buffer_pos++] = input;
            ctx->char_count++;
        }
        transition_state(ctx, STATE_READING);
    } else if (!isspace((unsigned char)input)) {
        transition_state(ctx, STATE_ERROR);
    }
}

void state_reading(struct context *ctx, char input) {
    if (ctx == NULL) return;
    
    if (input == '\n') {
        if (ctx->buffer_pos > 0) {
            ctx->word_count++;
        }
        transition_state(ctx, STATE_DONE);
        return;
    }
    
    if (isalnum((unsigned char)input)) {
        if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->buffer_pos++] = input;
            ctx->char_count++;
        }
    } else if (isspace((unsigned char)input)) {
        if (ctx->buffer_pos > 0) {
            ctx->word_count++;
            ctx->buffer_pos = 0;
            memset(ctx->buffer, 0, sizeof(ctx->buffer));
        }
        transition_state(ctx, STATE_PROCESSING);
    } else {
        transition_state(ctx, STATE_ERROR);
    }
}

void state_processing(struct context *ctx, char input) {
    if (ctx == NULL) return;
    
    if (input == '\n') {
        transition_state(ctx, STATE_DONE);
        return;
    }
    
    if (isalnum((unsigned char)input)) {
        if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->buffer_pos++] = input;
            ctx->char_count++;
        }
        transition_state(ctx, STATE_READING);
    } else if (!isspace((unsigned char)input)) {
        transition_state(ctx, STATE_ERROR);
    }
}

void state_done(struct context *ctx, char input) {
    if (ctx == NULL) return;
    if (input != '\n') {
        transition_state(ctx, STATE_ERROR);
    }
}

void state_error(struct context *ctx, char input) {
    if (ctx == NULL) return;
}

void print_results(const struct context *ctx) {
    if (ctx == NULL) return;
    
    if (ctx->current_state == STATE_DONE) {
        printf("Processing complete.\n");
        printf("Words counted: %d\n", ctx->word_count);
        printf("Characters counted: %d\n", ctx->char_count);
    } else if (ctx->current_state == STATE_ERROR) {
        printf("Error: Invalid input encountered.\n");
    } else {
        printf("Processing interrupted.\n");
    }
}

int main(void) {
    struct context ctx;
    initialize_context(&ctx);
    
    printf("Enter text (press Enter to finish):\n");
    
    int c;
    while ((c = getchar()) != EOF) {
        if (c == '\n') {
            process_character(&ctx, (char)c);
            break;
        }
        process_character(&ctx, (char)c);
        
        if (ctx.current_state == STATE_ERROR || ctx.current_state == STATE_DONE) {
            break;
        }
    }
    
    if (ctx.current_state != STATE_DONE && ctx.current_state != STATE_ERROR) {
        process_character(&ctx, '\