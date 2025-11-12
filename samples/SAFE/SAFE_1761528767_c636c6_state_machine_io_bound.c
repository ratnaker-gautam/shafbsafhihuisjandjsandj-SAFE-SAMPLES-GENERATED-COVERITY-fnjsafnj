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
    int line_count;
};

void state_idle(struct context *ctx);
void state_reading(struct context *ctx);
void state_processing(struct context *ctx);
void state_writing(struct context *ctx);
void state_error(struct context *ctx);
void state_done(struct context *ctx);

int main(void) {
    struct context ctx;
    ctx.current_state = STATE_IDLE;
    ctx.buffer_pos = 0;
    ctx.word_count = 0;
    ctx.line_count = 0;
    memset(ctx.buffer, 0, sizeof(ctx.buffer));

    printf("Word counter state machine\n");
    printf("Enter text (empty line to finish):\n");

    while (ctx.current_state != STATE_DONE && ctx.current_state != STATE_ERROR) {
        switch (ctx.current_state) {
            case STATE_IDLE:
                state_idle(&ctx);
                break;
            case STATE_READING:
                state_reading(&ctx);
                break;
            case STATE_PROCESSING:
                state_processing(&ctx);
                break;
            case STATE_WRITING:
                state_writing(&ctx);
                break;
            case STATE_ERROR:
                state_error(&ctx);
                break;
            case STATE_DONE:
                state_done(&ctx);
                break;
        }
    }

    return 0;
}

void state_idle(struct context *ctx) {
    ctx->current_state = STATE_READING;
}

void state_reading(struct context *ctx) {
    if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) == NULL) {
        ctx->current_state = STATE_ERROR;
        return;
    }

    size_t len = strlen(ctx->buffer);
    if (len > 0 && ctx->buffer[len - 1] == '\n') {
        ctx->buffer[len - 1] = '\0';
    }

    if (strlen(ctx->buffer) == 0) {
        ctx->current_state = STATE_DONE;
    } else {
        ctx->current_state = STATE_PROCESSING;
    }
}

void state_processing(struct context *ctx) {
    char *ptr = ctx->buffer;
    int in_word = 0;
    int words_in_line = 0;

    while (*ptr != '\0') {
        if (isspace((unsigned char)*ptr)) {
            if (in_word) {
                words_in_line++;
                in_word = 0;
            }
        } else {
            in_word = 1;
        }
        ptr++;
    }

    if (in_word) {
        words_in_line++;
    }

    ctx->word_count += words_in_line;
    ctx->line_count++;
    ctx->current_state = STATE_WRITING;
}

void state_writing(struct context *ctx) {
    printf("Line %d: %s\n", ctx->line_count, ctx->buffer);
    ctx->current_state = STATE_READING;
}

void state_error(struct context *ctx) {
    printf("Error reading input\n");
    exit(1);
}

void state_done(struct context *ctx) {
    printf("\nSummary:\n");
    printf("Lines: %d\n", ctx->line_count);
    printf("Words: %d\n", ctx->word_count);
    exit(0);
}