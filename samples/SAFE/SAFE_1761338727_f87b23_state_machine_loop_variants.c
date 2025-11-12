//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <limits.h>

enum state {
    START,
    READING_NUMBER,
    READING_WORD,
    ERROR,
    DONE
};

struct context {
    enum state current_state;
    int number_value;
    char word_buffer[64];
    size_t word_pos;
    int has_number;
    int has_word;
};

void context_init(struct context *ctx) {
    ctx->current_state = START;
    ctx->number_value = 0;
    memset(ctx->word_buffer, 0, sizeof(ctx->word_buffer));
    ctx->word_pos = 0;
    ctx->has_number = 0;
    ctx->has_word = 0;
}

int is_valid_input(char c) {
    return isalnum(c) || isspace(c) || c == '\0';
}

void process_char(struct context *ctx, char c) {
    if (!is_valid_input(c)) {
        ctx->current_state = ERROR;
        return;
    }

    switch (ctx->current_state) {
        case START:
            if (isdigit(c)) {
                ctx->current_state = READING_NUMBER;
                ctx->number_value = c - '0';
            } else if (isalpha(c)) {
                ctx->current_state = READING_WORD;
                ctx->word_buffer[0] = c;
                ctx->word_pos = 1;
            } else if (c == '\0') {
                ctx->current_state = DONE;
            }
            break;

        case READING_NUMBER:
            if (isdigit(c)) {
                int digit = c - '0';
                if (ctx->number_value <= (INT_MAX - digit) / 10) {
                    ctx->number_value = ctx->number_value * 10 + digit;
                } else {
                    ctx->current_state = ERROR;
                }
            } else if (isalpha(c)) {
                ctx->has_number = 1;
                ctx->current_state = READING_WORD;
                ctx->word_buffer[0] = c;
                ctx->word_pos = 1;
            } else if (isspace(c) || c == '\0') {
                ctx->has_number = 1;
                if (c == '\0') {
                    ctx->current_state = DONE;
                } else {
                    ctx->current_state = START;
                }
            } else {
                ctx->current_state = ERROR;
            }
            break;

        case READING_WORD:
            if (isalpha(c)) {
                if (ctx->word_pos < sizeof(ctx->word_buffer) - 1) {
                    ctx->word_buffer[ctx->word_pos++] = c;
                } else {
                    ctx->current_state = ERROR;
                }
            } else if (isdigit(c)) {
                ctx->has_word = 1;
                ctx->current_state = READING_NUMBER;
                ctx->number_value = c - '0';
            } else if (isspace(c) || c == '\0') {
                ctx->has_word = 1;
                if (c == '\0') {
                    ctx->current_state = DONE;
                } else {
                    ctx->current_state = START;
                }
            } else {
                ctx->current_state = ERROR;
            }
            break;

        case ERROR:
        case DONE:
            break;
    }
}

int main(void) {
    struct context ctx;
    char input_buffer[256];
    size_t input_len;
    size_t i;

    printf("Enter text (alphanumeric only): ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
        input_len--;
    }

    context_init(&ctx);

    for (i = 0; i <= input_len; i++) {
        process_char(&ctx, input_buffer[i]);
        if (ctx.current_state == ERROR) {
            printf("Invalid input format\n");
            return 1;
        }
    }

    if (ctx.current_state != DONE) {
        printf("Incomplete input processing\n");
        return 1;
    }

    printf("Processing results:\n");
    if (ctx.has_number) {
        printf("Found number: %d\n", ctx.number_value);
    }
    if (ctx.has_word) {
        printf("Found word: %s\n", ctx.word_buffer);
    }
    if (!ctx.has_number && !ctx.has_word) {
        printf("No alphanumeric content found\n");
    }

    return 0;
}