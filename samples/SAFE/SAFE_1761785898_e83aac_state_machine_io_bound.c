//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_WRITING,
    STATE_ERROR,
    STATE_DONE
};

struct context {
    enum state current_state;
    char buffer[256];
    size_t buffer_pos;
    int word_count;
    int char_count;
    int line_count;
};

void state_idle(struct context *ctx, char input);
void state_reading(struct context *ctx, char input);
void state_processing(struct context *ctx, char input);
void state_writing(struct context *ctx, char input);
void state_error(struct context *ctx, char input);
void state_done(struct context *ctx, char input);

int main(void) {
    struct context ctx;
    ctx.current_state = STATE_IDLE;
    ctx.buffer_pos = 0;
    ctx.word_count = 0;
    ctx.char_count = 0;
    ctx.line_count = 0;
    memset(ctx.buffer, 0, sizeof(ctx.buffer));

    printf("Enter text (Ctrl+D to end):\n");

    int input_char;
    while ((input_char = getchar()) != EOF && ctx.current_state != STATE_DONE) {
        if (input_char < 0 || input_char > 127) {
            ctx.current_state = STATE_ERROR;
            continue;
        }

        char c = (char)input_char;

        switch (ctx.current_state) {
            case STATE_IDLE:
                state_idle(&ctx, c);
                break;
            case STATE_READING:
                state_reading(&ctx, c);
                break;
            case STATE_PROCESSING:
                state_processing(&ctx, c);
                break;
            case STATE_WRITING:
                state_writing(&ctx, c);
                break;
            case STATE_ERROR:
                state_error(&ctx, c);
                break;
            case STATE_DONE:
                state_done(&ctx, c);
                break;
        }
    }

    if (ctx.current_state == STATE_READING || ctx.current_state == STATE_PROCESSING) {
        if (ctx.buffer_pos > 0) {
            ctx.current_state = STATE_PROCESSING;
            state_processing(&ctx, ' ');
        }
        ctx.current_state = STATE_WRITING;
        state_writing(&ctx, '\0');
    }

    if (ctx.current_state == STATE_ERROR) {
        printf("Error: Invalid input encountered\n");
        return 1;
    }

    printf("\nStatistics:\n");
    printf("Characters: %d\n", ctx.char_count);
    printf("Words: %d\n", ctx.word_count);
    printf("Lines: %d\n", ctx.line_count);

    return 0;
}

void state_idle(struct context *ctx, char input) {
    if (input == '\n') {
        ctx->line_count++;
        ctx->char_count++;
        return;
    }
    
    if (!isspace((unsigned char)input)) {
        ctx->current_state = STATE_READING;
        if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->buffer_pos++] = input;
            ctx->char_count++;
        } else {
            ctx->current_state = STATE_ERROR;
        }
    } else {
        ctx->char_count++;
    }
}

void state_reading(struct context *ctx, char input) {
    if (isspace((unsigned char)input)) {
        ctx->current_state = STATE_PROCESSING;
        state_processing(ctx, input);
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
    if (ctx->buffer_pos > 0) {
        ctx->word_count++;
        ctx->buffer[ctx->buffer_pos] = '\0';
        printf("Word %d: %s\n", ctx->word_count, ctx->buffer);
        ctx->buffer_pos = 0;
        memset(ctx->buffer, 0, sizeof(ctx->buffer));
    }
    
    if (input == '\n') {
        ctx->line_count++;
    }
    
    ctx->char_count++;
    
    if (input == '\0') {
        ctx->current_state = STATE_DONE;
    } else if (isspace((unsigned char)input)) {
        ctx->current_state = STATE_IDLE;
    } else {
        ctx->current_state = STATE_READING;
        if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->buffer_pos++] = input;
        } else {
            ctx->current_state = STATE_ERROR;
        }
    }
}

void state_writing(struct context *ctx, char input) {
    if (input == '\0') {
        ctx->current_state = STATE_DONE;
    } else {
        ctx->current_state = STATE_ERROR;
    }
}

void state_error(struct context *ctx, char input) {
    ctx->current_state = STATE_ERROR;
}

void state_done(struct context *ctx, char input) {
    ctx->current_state = STATE_DONE;
}