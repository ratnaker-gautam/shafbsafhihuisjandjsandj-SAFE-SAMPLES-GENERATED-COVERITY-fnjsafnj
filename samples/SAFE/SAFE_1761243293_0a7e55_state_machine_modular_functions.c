//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State {
    STATE_START,
    STATE_READING_NUMBER,
    STATE_READING_WORD,
    STATE_ERROR,
    STATE_END
};

struct Context {
    enum State current_state;
    int number_count;
    int word_count;
    char buffer[256];
    int buffer_index;
};

void context_init(struct Context *ctx) {
    if (ctx == NULL) return;
    ctx->current_state = STATE_START;
    ctx->number_count = 0;
    ctx->word_count = 0;
    ctx->buffer_index = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
}

int is_valid_input_char(char c) {
    return isalnum(c) || isspace(c) || c == '\0';
}

void process_char(struct Context *ctx, char c) {
    if (ctx == NULL || !is_valid_input_char(c)) {
        ctx->current_state = STATE_ERROR;
        return;
    }

    switch (ctx->current_state) {
        case STATE_START:
            if (isdigit(c)) {
                ctx->current_state = STATE_READING_NUMBER;
                ctx->buffer[ctx->buffer_index++] = c;
            } else if (isalpha(c)) {
                ctx->current_state = STATE_READING_WORD;
                ctx->buffer[ctx->buffer_index++] = c;
            } else if (isspace(c)) {
                ctx->current_state = STATE_START;
            } else if (c == '\0') {
                ctx->current_state = STATE_END;
            } else {
                ctx->current_state = STATE_ERROR;
            }
            break;

        case STATE_READING_NUMBER:
            if (isdigit(c)) {
                if (ctx->buffer_index < 255) {
                    ctx->buffer[ctx->buffer_index++] = c;
                } else {
                    ctx->current_state = STATE_ERROR;
                }
            } else if (isspace(c) || c == '\0') {
                ctx->number_count++;
                ctx->buffer_index = 0;
                memset(ctx->buffer, 0, sizeof(ctx->buffer));
                if (c == '\0') {
                    ctx->current_state = STATE_END;
                } else {
                    ctx->current_state = STATE_START;
                }
            } else {
                ctx->current_state = STATE_ERROR;
            }
            break;

        case STATE_READING_WORD:
            if (isalpha(c)) {
                if (ctx->buffer_index < 255) {
                    ctx->buffer[ctx->buffer_index++] = c;
                } else {
                    ctx->current_state = STATE_ERROR;
                }
            } else if (isspace(c) || c == '\0') {
                ctx->word_count++;
                ctx->buffer_index = 0;
                memset(ctx->buffer, 0, sizeof(ctx->buffer));
                if (c == '\0') {
                    ctx->current_state = STATE_END;
                } else {
                    ctx->current_state = STATE_START;
                }
            } else {
                ctx->current_state = STATE_ERROR;
            }
            break;

        case STATE_ERROR:
        case STATE_END:
            break;
    }
}

int process_input(struct Context *ctx, const char *input) {
    if (ctx == NULL || input == NULL) return 0;

    size_t len = strlen(input);
    if (len > 1024) return 0;

    for (size_t i = 0; i <= len; i++) {
        char c = input[i];
        process_char(ctx, c);
        if (ctx->current_state == STATE_ERROR) {
            return 0;
        }
    }

    return ctx->current_state == STATE_END;
}

int main(void) {
    struct Context ctx;
    context_init(&ctx);

    char input[1025];
    printf("Enter text (max 1024 chars): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }

    if (!process_input(&ctx, input)) {
        fprintf(stderr, "Error processing input\n");
        return 1;
    }

    printf("Numbers found: %d\n", ctx.number_count);
    printf("Words found: %d\n", ctx.word_count);

    return 0;
}