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
    STATE_DONE,
    STATE_ERROR
};

struct context {
    enum state current_state;
    char buffer[256];
    size_t buffer_pos;
    int word_count;
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
}

void transition_state(struct context *ctx, enum state new_state) {
    if (ctx == NULL) return;
    ctx->current_state = new_state;
}

void state_idle(struct context *ctx, char input) {
    if (ctx == NULL) return;
    
    if (input == 'S' || input == 's') {
        printf("Starting word count analysis...\n");
        transition_state(ctx, STATE_READING);
    } else if (input == 'Q' || input == 'q') {
        transition_state(ctx, STATE_DONE);
    } else if (input != '\n') {
        printf("Press 'S' to start or 'Q' to quit\n");
    }
}

void state_reading(struct context *ctx, char input) {
    if (ctx == NULL) return;
    
    if (input == '\n') {
        if (ctx->buffer_pos > 0) {
            transition_state(ctx, STATE_PROCESSING);
        } else {
            printf("Empty line detected. Press 'S' for new analysis or 'Q' to quit\n");
            transition_state(ctx, STATE_IDLE);
        }
    } else if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
        ctx->buffer[ctx->buffer_pos++] = input;
    } else {
        printf("Input too long. Maximum %zu characters allowed.\n", sizeof(ctx->buffer) - 1);
        transition_state(ctx, STATE_ERROR);
    }
}

void state_processing(struct context *ctx, char input) {
    if (ctx == NULL) return;
    
    if (input != '\n') return;
    
    int in_word = 0;
    ctx->word_count = 0;
    
    for (size_t i = 0; i < ctx->buffer_pos; i++) {
        if (isalpha((unsigned char)ctx->buffer[i])) {
            if (!in_word) {
                ctx->word_count++;
                in_word = 1;
            }
        } else {
            in_word = 0;
        }
    }
    
    printf("Analysis complete: %d words found in '%s'\n", ctx->word_count, ctx->buffer);
    printf("Press 'S' for new analysis or 'Q' to quit\n");
    
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    ctx->buffer_pos = 0;
    transition_state(ctx, STATE_IDLE);
}

void state_done(struct context *ctx, char input) {
    if (ctx == NULL) return;
    printf("Exiting word counter.\n");
}

void state_error(struct context *ctx, char input) {
    if (ctx == NULL) return;
    
    if (input == '\n') {
        printf("Error state cleared. Press 'S' to restart or 'Q' to quit\n");
        memset(ctx->buffer, 0, sizeof(ctx->buffer));
        ctx->buffer_pos = 0;
        transition_state(ctx, STATE_IDLE);
    }
}

void process_input(struct context *ctx, char input) {
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

int main(void) {
    struct context ctx;
    initialize_context(&ctx);
    
    printf("Word Counter State Machine\n");
    printf("Commands: S=Start, Q=Quit\n");
    printf("Enter text to count words (press Enter to analyze)\n");
    
    int c;
    while (ctx.current_state != STATE_DONE) {
        c = getchar();
        if (c == EOF) {
            break;
        }
        process_input(&ctx, (char)c);
    }
    
    return 0;
}