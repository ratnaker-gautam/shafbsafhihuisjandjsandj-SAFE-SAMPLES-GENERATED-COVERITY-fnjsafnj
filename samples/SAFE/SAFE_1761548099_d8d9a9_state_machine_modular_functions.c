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

void state_idle(struct context *ctx, char input) {
    if (ctx == NULL) return;
    
    if (isalpha(input)) {
        if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->buffer_pos++] = input;
            ctx->current_state = STATE_READING;
        } else {
            ctx->current_state = STATE_ERROR;
        }
    } else if (input == '\n') {
        ctx->current_state = STATE_DONE;
    } else if (!isspace(input)) {
        ctx->current_state = STATE_ERROR;
    }
}

void state_reading(struct context *ctx, char input) {
    if (ctx == NULL) return;
    
    if (isalpha(input)) {
        if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->buffer_pos++] = input;
        } else {
            ctx->current_state = STATE_ERROR;
        }
    } else if (isspace(input)) {
        ctx->word_count++;
        ctx->buffer_pos = 0;
        memset(ctx->buffer, 0, sizeof(ctx->buffer));
        ctx->current_state = STATE_PROCESSING;
    } else if (input == '\n') {
        ctx->word_count++;
        ctx->current_state = STATE_DONE;
    } else {
        ctx->current_state = STATE_ERROR;
    }
}

void state_processing(struct context *ctx, char input) {
    if (ctx == NULL) return;
    
    if (isalpha(input)) {
        if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->buffer_pos++] = input;
            ctx->current_state = STATE_READING;
        } else {
            ctx->current_state = STATE_ERROR;
        }
    } else if (input == '\n') {
        ctx->current_state = STATE_DONE;
    } else if (!isspace(input)) {
        ctx->current_state = STATE_ERROR;
    }
}

void state_done(struct context *ctx, char input) {
    if (ctx == NULL) return;
    ctx->current_state = STATE_DONE;
}

void state_error(struct context *ctx, char input) {
    if (ctx == NULL) return;
    ctx->current_state = STATE_ERROR;
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

int main(void) {
    struct context ctx;
    char input_line[512];
    
    initialize_context(&ctx);
    
    printf("Enter text to count words (max 511 characters): ");
    
    if (fgets(input_line, sizeof(input_line), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_line);
    if (len > 0 && input_line[len - 1] == '\n') {
        input_line[len - 1] = '\0';
    }
    
    if (len == sizeof(input_line) - 1 && input_line[len - 1] != '\n') {
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        printf("Input too long, truncating\n");
    }
    
    process_input(&ctx, input_line);
    
    if (ctx.current_state == STATE_ERROR) {
        printf("Error: Invalid input format\n");
        return 1;
    }
    
    printf("Word count: %d\n", ctx.word_count);
    
    return 0;
}