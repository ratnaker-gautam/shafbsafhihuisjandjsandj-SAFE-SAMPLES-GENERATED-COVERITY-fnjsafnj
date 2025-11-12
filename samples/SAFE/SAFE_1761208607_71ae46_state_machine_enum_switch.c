//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State {
    START,
    READING_NUMBER,
    READING_WORD,
    ERROR,
    DONE
};

struct Context {
    enum State current_state;
    int number_count;
    int word_count;
    char buffer[256];
    size_t buffer_pos;
};

void init_context(struct Context *ctx) {
    ctx->current_state = START;
    ctx->number_count = 0;
    ctx->word_count = 0;
    ctx->buffer_pos = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
}

int is_valid_char(char c) {
    return isalnum(c) || isspace(c) || c == '\0';
}

void process_char(struct Context *ctx, char c) {
    if (!is_valid_char(c)) {
        ctx->current_state = ERROR;
        return;
    }

    switch (ctx->current_state) {
        case START:
            if (isdigit(c)) {
                ctx->current_state = READING_NUMBER;
                ctx->buffer[ctx->buffer_pos++] = c;
            } else if (isalpha(c)) {
                ctx->current_state = READING_WORD;
                ctx->buffer[ctx->buffer_pos++] = c;
            } else if (isspace(c)) {
                break;
            } else {
                ctx->current_state = ERROR;
            }
            break;

        case READING_NUMBER:
            if (isdigit(c)) {
                if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
                    ctx->buffer[ctx->buffer_pos++] = c;
                } else {
                    ctx->current_state = ERROR;
                }
            } else if (isspace(c) || c == '\0') {
                ctx->number_count++;
                ctx->buffer_pos = 0;
                ctx->current_state = START;
            } else {
                ctx->current_state = ERROR;
            }
            break;

        case READING_WORD:
            if (isalpha(c)) {
                if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
                    ctx->buffer[ctx->buffer_pos++] = c;
                } else {
                    ctx->current_state = ERROR;
                }
            } else if (isspace(c) || c == '\0') {
                ctx->word_count++;
                ctx->buffer_pos = 0;
                ctx->current_state = START;
            } else {
                ctx->current_state = ERROR;
            }
            break;

        case ERROR:
            break;

        case DONE:
            break;
    }
}

int main(void) {
    struct Context ctx;
    init_context(&ctx);

    char input[1024];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }

    if (input_len >= sizeof(input)) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }

    for (size_t i = 0; i <= input_len; i++) {
        if (ctx.current_state == ERROR) {
            break;
        }
        process_char(&ctx, input[i]);
    }

    if (ctx.current_state == ERROR) {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }

    printf("Numbers: %d\n", ctx.number_count);
    printf("Words: %d\n", ctx.word_count);

    return 0;
}