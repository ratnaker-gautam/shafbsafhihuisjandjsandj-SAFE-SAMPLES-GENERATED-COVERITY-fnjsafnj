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

void state_idle(struct context *ctx, char input) {
    if (ctx == NULL) return;
    
    if (input == '\n') {
        transition_state(ctx, STATE_DONE);
    } else if (isalpha(input)) {
        if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->buffer_pos++] = input;
            ctx->char_count++;
            transition_state(ctx, STATE_READING);
        } else {
            transition_state(ctx, STATE_ERROR);
        }
    } else if (isspace(input)) {
    } else {
        transition_state(ctx, STATE_ERROR);
    }
}

void state_reading(struct context *ctx, char input) {
    if (ctx == NULL) return;
    
    if (input == '\n') {
        ctx->word_count++;
        transition_state(ctx, STATE_DONE);
    } else if (isalpha(input)) {
        if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->buffer_pos++] = input;
            ctx->char_count++;
        } else {
            transition_state(ctx, STATE_ERROR);
        }
    } else if (isspace(input)) {
        ctx->word_count++;
        transition_state(ctx, STATE_PROCESSING);
    } else {
        transition_state(ctx, STATE_ERROR);
    }
}

void state_processing(struct context *ctx, char input) {
    if (ctx == NULL) return;
    
    if (input == '\n') {
        transition_state(ctx, STATE_DONE);
    } else if (isalpha(input)) {
        ctx->buffer_pos = 0;
        memset(ctx->buffer, 0, sizeof(ctx->buffer));
        if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->buffer_pos++] = input;
            ctx->char_count++;
            transition_state(ctx, STATE_READING);
        } else {
            transition_state(ctx, STATE_ERROR);
        }
    } else if (isspace(input)) {
    } else {
        transition_state(ctx, STATE_ERROR);
    }
}

void state_done(struct context *ctx, char input) {
}

void state_error(struct context *ctx, char input) {
}

void process_input(struct context *ctx, const char *input) {
    if (ctx == NULL || input == NULL) return;
    
    size_t len = strlen(input);
    for (size_t i = 0; i < len; i++) {
        switch (ctx->current_state) {
            case STATE_IDLE:
                state_idle(ctx, input[i]);
                break;
            case STATE_READING:
                state_reading(ctx, input[i]);
                break;
            case STATE_PROCESSING:
                state_processing(ctx, input[i]);
                break;
            case STATE_DONE:
                state_done(ctx, input[i]);
                break;
            case STATE_ERROR:
                state_error(ctx, input[i]);
                break;
        }
        
        if (ctx->current_state == STATE_ERROR || ctx->current_state == STATE_DONE) {
            break;
        }
    }
}

void print_results(const struct context *ctx) {
    if (ctx == NULL) return;
    
    if (ctx->current_state == STATE_ERROR) {
        printf("Error: Invalid input detected\n");
    } else {
        printf("Words: %d\n", ctx->word_count);
        printf("Characters: %d\n", ctx->char_count);
    }
}

int main(void) {
    struct context ctx;
    char input_line[512];
    
    initialize_context(&ctx);
    
    printf("Enter text (max 511 characters): ");
    
    if (fgets(input_line, sizeof(input_line), stdin) != NULL) {
        size_t len = strlen(input_line);
        if (len > 0 && input_line[len - 1] == '\n') {
            input_line[len - 1] = '\0';
        }
        
        process_input(&ctx, input_line);
        print_results(&ctx);
    } else {
        printf("Error reading input\n");
        return 1;
    }
    
    return 0;
}