//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state {
    STATE_START,
    STATE_READING_WORD,
    STATE_READING_NUMBER,
    STATE_ERROR,
    STATE_DONE
};

struct context {
    enum state current_state;
    int word_count;
    int number_count;
    char buffer[256];
    size_t buffer_pos;
};

void init_context(struct context *ctx) {
    if (ctx == NULL) return;
    ctx->current_state = STATE_START;
    ctx->word_count = 0;
    ctx->number_count = 0;
    ctx->buffer_pos = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
}

int is_valid_char(char c) {
    return isprint(c) || c == '\n' || c == '\t' || c == ' ';
}

void process_char(struct context *ctx, char c) {
    if (ctx == NULL || !is_valid_char(c)) {
        ctx->current_state = STATE_ERROR;
        return;
    }

    switch (ctx->current_state) {
        case STATE_START:
            if (isalpha(c)) {
                ctx->current_state = STATE_READING_WORD;
                ctx->buffer[ctx->buffer_pos++] = c;
            } else if (isdigit(c)) {
                ctx->current_state = STATE_READING_NUMBER;
                ctx->buffer[ctx->buffer_pos++] = c;
            } else if (c == '\n') {
                ctx->current_state = STATE_DONE;
            }
            break;

        case STATE_READING_WORD:
            if (isalpha(c)) {
                if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
                    ctx->buffer[ctx->buffer_pos++] = c;
                } else {
                    ctx->current_state = STATE_ERROR;
                }
            } else if (isspace(c)) {
                ctx->word_count++;
                ctx->buffer_pos = 0;
                memset(ctx->buffer, 0, sizeof(ctx->buffer));
                if (c == '\n') {
                    ctx->current_state = STATE_DONE;
                } else {
                    ctx->current_state = STATE_START;
                }
            } else {
                ctx->current_state = STATE_ERROR;
            }
            break;

        case STATE_READING_NUMBER:
            if (isdigit(c)) {
                if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
                    ctx->buffer[ctx->buffer_pos++] = c;
                } else {
                    ctx->current_state = STATE_ERROR;
                }
            } else if (isspace(c)) {
                ctx->number_count++;
                ctx->buffer_pos = 0;
                memset(ctx->buffer, 0, sizeof(ctx->buffer));
                if (c == '\n') {
                    ctx->current_state = STATE_DONE;
                } else {
                    ctx->current_state = STATE_START;
                }
            } else {
                ctx->current_state = STATE_ERROR;
            }
            break;

        case STATE_ERROR:
        case STATE_DONE:
            break;
    }
}

int main(void) {
    struct context ctx;
    init_context(&ctx);
    
    printf("Enter text (words and numbers separated by spaces, end with newline):\n");
    
    int c;
    while ((c = getchar()) != EOF && ctx.current_state != STATE_DONE && ctx.current_state != STATE_ERROR) {
        process_char(&ctx, (char)c);
    }

    if (ctx.current_state == STATE_READING_WORD) {
        ctx.word_count++;
    } else if (ctx.current_state == STATE_READING_NUMBER) {
        ctx.number_count++;
    }

    if (ctx.current_state == STATE_ERROR) {
        printf("Error: Invalid input format detected.\n");
        return 1;
    }

    printf("Words: %d\n", ctx.word_count);
    printf("Numbers: %d\n", ctx.number_count);
    
    return 0;
}