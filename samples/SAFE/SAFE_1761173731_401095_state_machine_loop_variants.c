//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <limits.h>

enum state {
    INIT,
    READING_NUMBER,
    READING_WORD,
    ERROR,
    DONE
};

struct context {
    enum state current_state;
    int number_value;
    char word_buffer[64];
    int word_len;
    int total_numbers;
    int total_words;
};

void state_machine_init(struct context *ctx) {
    if (ctx == NULL) return;
    ctx->current_state = INIT;
    ctx->number_value = 0;
    ctx->word_len = 0;
    ctx->total_numbers = 0;
    ctx->total_words = 0;
    memset(ctx->word_buffer, 0, sizeof(ctx->word_buffer));
}

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == ' ' || c == '\t' || c == '\n' || c == EOF;
}

void process_char(struct context *ctx, char c) {
    if (ctx == NULL) return;
    
    if (!is_valid_char(c)) {
        ctx->current_state = ERROR;
        return;
    }

    switch (ctx->current_state) {
        case INIT:
            if (isdigit((unsigned char)c)) {
                ctx->current_state = READING_NUMBER;
                ctx->number_value = c - '0';
            } else if (isalpha((unsigned char)c)) {
                ctx->current_state = READING_WORD;
                ctx->word_len = 0;
                if (ctx->word_len < 63) {
                    ctx->word_buffer[ctx->word_len++] = c;
                }
            } else if (c == EOF) {
                ctx->current_state = DONE;
            }
            break;

        case READING_NUMBER:
            if (isdigit((unsigned char)c)) {
                int digit = c - '0';
                if (ctx->number_value <= (INT_MAX - digit) / 10) {
                    ctx->number_value = ctx->number_value * 10 + digit;
                } else {
                    ctx->current_state = ERROR;
                }
            } else if (isalpha((unsigned char)c)) {
                ctx->total_numbers++;
                ctx->current_state = READING_WORD;
                ctx->word_len = 0;
                if (ctx->word_len < 63) {
                    ctx->word_buffer[ctx->word_len++] = c;
                }
            } else if (c == ' ' || c == '\t' || c == '\n') {
                ctx->total_numbers++;
                ctx->current_state = INIT;
            } else if (c == EOF) {
                ctx->total_numbers++;
                ctx->current_state = DONE;
            } else {
                ctx->current_state = ERROR;
            }
            break;

        case READING_WORD:
            if (isalnum((unsigned char)c)) {
                if (ctx->word_len < 63) {
                    ctx->word_buffer[ctx->word_len++] = c;
                }
            } else if (c == ' ' || c == '\t' || c == '\n') {
                ctx->total_words++;
                ctx->current_state = INIT;
                ctx->word_len = 0;
                memset(ctx->word_buffer, 0, sizeof(ctx->word_buffer));
            } else if (c == EOF) {
                ctx->total_words++;
                ctx->current_state = DONE;
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
    state_machine_init(&ctx);

    printf("Enter text (alphanumeric characters and spaces only):\n");

    int c;
    while ((c = getchar()) != EOF) {
        if (c < 0 || c > 255) {
            ctx.current_state = ERROR;
            break;
        }
        process_char(&ctx, (char)c);
        if (ctx.current_state == ERROR || ctx.current_state == DONE) {
            break;
        }
    }

    if (ctx.current_state == READING_NUMBER) {
        ctx.total_numbers++;
    } else if (ctx.current_state == READING_WORD) {
        ctx.total_words++;
    }

    if (ctx.current_state == ERROR) {
        printf("Error: Invalid input detected\n");
        return 1;
    }

    printf("Total numbers: %d\n", ctx.total_numbers);
    printf("Total words: %d\n", ctx.total_words);

    return 0;
}