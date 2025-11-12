//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

enum State {
    START,
    READING_NUMBER,
    READING_WORD,
    SKIPPING_SPACE,
    FINISHED,
    ERROR
};

struct Context {
    enum State current_state;
    char buffer[256];
    size_t buffer_pos;
    int number_count;
    int word_count;
};

void init_context(struct Context *ctx) {
    ctx->current_state = START;
    ctx->buffer_pos = 0;
    ctx->number_count = 0;
    ctx->word_count = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
}

int is_valid_input_char(char c) {
    return isalnum(c) || isspace(c) || c == '\0';
}

void process_char(struct Context *ctx, char c) {
    if (!is_valid_input_char(c)) {
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
                ctx->current_state = SKIPPING_SPACE;
            } else if (c == '\0') {
                ctx->current_state = FINISHED;
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
                memset(ctx->buffer, 0, sizeof(ctx->buffer));
                if (c == '\0') {
                    ctx->current_state = FINISHED;
                } else {
                    ctx->current_state = SKIPPING_SPACE;
                }
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
                memset(ctx->buffer, 0, sizeof(ctx->buffer));
                if (c == '\0') {
                    ctx->current_state = FINISHED;
                } else {
                    ctx->current_state = SKIPPING_SPACE;
                }
            } else {
                ctx->current_state = ERROR;
            }
            break;

        case SKIPPING_SPACE:
            if (!isspace(c)) {
                if (c == '\0') {
                    ctx->current_state = FINISHED;
                } else if (isdigit(c)) {
                    ctx->current_state = READING_NUMBER;
                    ctx->buffer[ctx->buffer_pos++] = c;
                } else if (isalpha(c)) {
                    ctx->current_state = READING_WORD;
                    ctx->buffer[ctx->buffer_pos++] = c;
                } else {
                    ctx->current_state = ERROR;
                }
            }
            break;

        case FINISHED:
        case ERROR:
            break;
    }
}

int main(void) {
    struct Context ctx;
    init_context(&ctx);

    char input_line[512];
    if (fgets(input_line, sizeof(input_line), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    size_t input_len = strlen(input_line);
    if (input_len > 0 && input_line[input_len - 1] == '\n') {
        input_line[input_len - 1] = '\0';
        input_len--;
    }

    if (input_len >= sizeof(input_line)) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }

    size_t i = 0;
    while (i <= input_len && ctx.current_state != ERROR && ctx.current_state != FINISHED) {
        char current_char = input_line[i];
        process_char(&ctx, current_char);
        i++;
    }

    if (ctx.current_state == ERROR) {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }

    printf("Numbers: %d\n", ctx.number_count);
    printf("Words: %d\n", ctx.word_count);

    return 0;
}