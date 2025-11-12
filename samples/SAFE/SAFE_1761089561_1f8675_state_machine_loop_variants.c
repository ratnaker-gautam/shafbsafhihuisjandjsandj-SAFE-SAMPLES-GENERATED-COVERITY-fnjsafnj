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

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || isspace((unsigned char)c);
}

void process_char(struct Context *ctx, char c) {
    if (!is_valid_char(c)) {
        ctx->current_state = ERROR;
        return;
    }

    switch (ctx->current_state) {
        case START:
            if (isdigit((unsigned char)c)) {
                ctx->current_state = READING_NUMBER;
                ctx->buffer[ctx->buffer_pos++] = c;
            } else if (isalpha((unsigned char)c)) {
                ctx->current_state = READING_WORD;
                ctx->buffer[ctx->buffer_pos++] = c;
            } else if (isspace((unsigned char)c)) {
                ctx->current_state = SKIPPING_SPACE;
            } else {
                ctx->current_state = ERROR;
            }
            break;

        case READING_NUMBER:
            if (isdigit((unsigned char)c)) {
                if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
                    ctx->buffer[ctx->buffer_pos++] = c;
                } else {
                    ctx->current_state = ERROR;
                }
            } else if (isspace((unsigned char)c)) {
                ctx->number_count++;
                ctx->buffer_pos = 0;
                ctx->current_state = SKIPPING_SPACE;
            } else {
                ctx->current_state = ERROR;
            }
            break;

        case READING_WORD:
            if (isalpha((unsigned char)c)) {
                if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
                    ctx->buffer[ctx->buffer_pos++] = c;
                } else {
                    ctx->current_state = ERROR;
                }
            } else if (isspace((unsigned char)c)) {
                ctx->word_count++;
                ctx->buffer_pos = 0;
                ctx->current_state = SKIPPING_SPACE;
            } else {
                ctx->current_state = ERROR;
            }
            break;

        case SKIPPING_SPACE:
            if (isdigit((unsigned char)c)) {
                ctx->current_state = READING_NUMBER;
                ctx->buffer[ctx->buffer_pos++] = c;
            } else if (isalpha((unsigned char)c)) {
                ctx->current_state = READING_WORD;
                ctx->buffer[ctx->buffer_pos++] = c;
            } else if (!isspace((unsigned char)c)) {
                ctx->current_state = ERROR;
            }
            break;

        case FINISHED:
        case ERROR:
            break;
    }
}

void process_input(struct Context *ctx, const char *input) {
    if (input == NULL) {
        ctx->current_state = ERROR;
        return;
    }

    size_t len = strlen(input);
    if (len == 0) {
        ctx->current_state = FINISHED;
        return;
    }

    size_t i = 0;
    while (i < len && ctx->current_state != ERROR && ctx->current_state != FINISHED) {
        process_char(ctx, input[i]);
        i++;
    }

    if (ctx->current_state == READING_NUMBER || ctx->current_state == READING_WORD) {
        if (ctx->current_state == READING_NUMBER) {
            ctx->number_count++;
        } else {
            ctx->word_count++;
        }
        ctx->current_state = FINISHED;
    } else if (ctx->current_state == SKIPPING_SPACE) {
        ctx->current_state = FINISHED;
    }
}

int main(void) {
    struct Context ctx;
    char input[512];

    printf("Enter text to analyze (max 511 chars): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }

    init_context(&ctx);
    process_input(&ctx, input);

    if (ctx.current_state == ERROR) {
        printf("Invalid input format\n");
        return 1;
    }

    printf("Numbers found: %d\n", ctx.number_count);
    printf("Words found: %d\n", ctx.word_count);

    return 0;
}