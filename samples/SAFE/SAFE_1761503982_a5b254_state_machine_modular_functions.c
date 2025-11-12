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

void process_input(struct context *ctx, char input) {
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
    
    if (isalpha(input)) {
        if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->buffer_pos++] = input;
            ctx->char_count++;
            transition_state(ctx, STATE_READING);
        } else {
            transition_state(ctx, STATE_ERROR);
        }
    } else if (input == '\n') {
        transition_state(ctx, STATE_DONE);
    }
}

void state_reading(struct context *ctx, char input) {
    if (ctx == NULL) return;
    
    if (isalpha(input)) {
        if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->buffer_pos++] = input;
            ctx->char_count++;
        } else {
            transition_state(ctx, STATE_ERROR);
        }
    } else if (isspace(input)) {
        if (ctx->buffer_pos > 0) {
            ctx->word_count++;
            ctx->buffer_pos = 0;
            transition_state(ctx, STATE_PROCESSING);
        }
    } else if (input == '\n') {
        if (ctx->buffer_pos > 0) {
            ctx->word_count++;
        }
        transition_state(ctx, STATE_DONE);
    } else {
        transition_state(ctx, STATE_ERROR);
    }
}

void state_processing(struct context *ctx, char input) {
    if (ctx == NULL) return;
    
    if (isalpha(input)) {
        if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->buffer_pos++] = input;
            ctx->char_count++;
            transition_state(ctx, STATE_READING);
        } else {
            transition_state(ctx, STATE_ERROR);
        }
    } else if (isspace(input)) {
    } else if (input == '\n') {
        transition_state(ctx, STATE_DONE);
    } else {
        transition_state(ctx, STATE_ERROR);
    }
}

void state_done(struct context *ctx, char input) {
}

void state_error(struct context *ctx, char input) {
}

void print_results(const struct context *ctx) {
    if (ctx == NULL) return;
    
    printf("Processing complete.\n");
    printf("Words counted: %d\n", ctx->word_count);
    printf("Characters counted: %d\n", ctx->char_count);
    
    if (ctx->current_state == STATE_ERROR) {
        printf("State: ERROR - Invalid input encountered\n");
    } else {
        printf("State: DONE\n");
    }
}

int main(void) {
    struct context ctx;
    initialize_context(&ctx);
    
    printf("Enter text (press Enter twice to finish):\n");
    
    int c;
    while (ctx.current_state != STATE_DONE && ctx.current_state != STATE_ERROR) {
        c = getchar();
        if (c == EOF) break;
        
        process_input(&ctx, (char)c);
        
        if (c == '\n') {
            int next_char = getchar();
            if (next_char == '\n') {
                process_input(&ctx, '\n');
                break;
            } else if (next_char != EOF) {
                ungetc(next_char, stdin);
            }
        }
    }
    
    print_results(&ctx);
    
    return ctx.current_state == STATE_ERROR ? EXIT_FAILURE : EXIT_SUCCESS;
}