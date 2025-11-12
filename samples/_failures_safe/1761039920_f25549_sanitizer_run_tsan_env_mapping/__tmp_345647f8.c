//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state {
    STATE_START,
    STATE_READING_NUMBER,
    STATE_READING_WORD,
    STATE_ERROR,
    STATE_DONE
};

struct context {
    enum state current_state;
    int number_count;
    int word_count;
    char buffer[256];
    size_t buffer_pos;
};

void context_init(struct context *ctx) {
    if (ctx == NULL) return;
    ctx->current_state = STATE_START;
    ctx->number_count = 0;
    ctx->word_count = 0;
    ctx->buffer_pos = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
}

int is_valid_char(int c) {
    return c == EOF || c == ' ' || c == '\t' || c == '\n' || isalnum(c);
}

void process_char(struct context *ctx, int c) {
    if (ctx == NULL) return;
    
    if (!is_valid_char(c)) {
        ctx->current_state = STATE_ERROR;
        return;
    }

    switch (ctx->current_state) {
        case STATE_START:
            if (c == EOF) {
                ctx->current_state = STATE_DONE;
            } else if (isdigit(c)) {
                ctx->buffer[ctx->buffer_pos++] = (char)c;
                ctx->current_state = STATE_READING_NUMBER;
            } else if (isalpha(c)) {
                ctx->buffer[ctx->buffer_pos++] = (char)c;
                ctx->current_state = STATE_READING_WORD;
            } else if (c == ' ' || c == '\t' || c == '\n') {
            } else {
                ctx->current_state = STATE_ERROR;
            }
            break;

        case STATE_READING_NUMBER:
            if (c == EOF || c == ' ' || c == '\t' || c == '\n') {
                if (ctx->buffer_pos > 0) {
                    ctx->number_count++;
                }
                ctx->buffer_pos = 0;
                memset(ctx->buffer, 0, sizeof(ctx->buffer));
                ctx->current_state = (c == EOF) ? STATE_DONE : STATE_START;
            } else if (isdigit(c)) {
                if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
                    ctx->buffer[ctx->buffer_pos++] = (char)c;
                } else {
                    ctx->current_state = STATE_ERROR;
                }
            } else {
                ctx->current_state = STATE_ERROR;
            }
            break;

        case STATE_READING_WORD:
            if (c == EOF || c == ' ' || c == '\t' || c == '\n') {
                if (ctx->buffer_pos > 0) {
                    ctx->word_count++;
                }
                ctx->buffer_pos = 0;
                memset(ctx->buffer, 0, sizeof(ctx->buffer));
                ctx->current_state = (c == EOF) ? STATE_DONE : STATE_START;
            } else if (isalpha(c)) {
                if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
                    ctx->buffer[ctx->buffer_pos++] = (char)c;
                } else {
                    ctx->current_state = STATE_ERROR;
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
    context_init(&ctx);
    
    printf("Enter text (Ctrl+D to end):\n");
    
    int c;
    while ((c = getchar()) != EOF && ctx.current_state != STATE_ERROR && ctx.current_state != STATE_DONE) {
        process_char(&ctx, c);
    }
    
    if (ctx.current_state == STATE_READING_NUMBER || ctx.current_state == STATE_READING_WORD) {
        process_char(&ctx, EOF);
    }
    
    if (ctx.current_state == STATE_ERROR) {
        printf("Error: Invalid input format\n");
        return EXIT_FAILURE;
    }
    
    printf("Numbers: %d\n", ctx.number_count);
    printf("Words: %d\n", ctx.word_count);
    
    return EXIT_SUCCESS;
}