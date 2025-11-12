//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    int number;
    char word[32];
    int word_len;
    int total_numbers;
    int total_words;
};

void init_context(struct context *ctx) {
    ctx->current_state = START;
    ctx->number = 0;
    memset(ctx->word, 0, sizeof(ctx->word));
    ctx->word_len = 0;
    ctx->total_numbers = 0;
    ctx->total_words = 0;
}

int is_digit_char(char c) {
    return c >= '0' && c <= '9';
}

int is_alpha_char(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

void process_char(struct context *ctx, char c) {
    switch (ctx->current_state) {
        case START:
            if (is_digit_char(c)) {
                ctx->number = c - '0';
                ctx->current_state = READING_NUMBER;
            } else if (is_alpha_char(c)) {
                if (ctx->word_len < 31) {
                    ctx->word[ctx->word_len++] = c;
                    ctx->current_state = READING_WORD;
                } else {
                    ctx->current_state = ERROR;
                }
            } else if (c == '\n' || c == '\0') {
                ctx->current_state = DONE;
            }
            break;
            
        case READING_NUMBER:
            if (is_digit_char(c)) {
                int digit = c - '0';
                if (ctx->number <= (INT_MAX - digit) / 10) {
                    ctx->number = ctx->number * 10 + digit;
                } else {
                    ctx->current_state = ERROR;
                }
            } else if (c == ' ' || c == '\n' || c == '\0') {
                ctx->total_numbers++;
                printf("Found number: %d\n", ctx->number);
                ctx->number = 0;
                ctx->current_state = (c == '\n' || c == '\0') ? DONE : START;
            } else {
                ctx->current_state = ERROR;
            }
            break;
            
        case READING_WORD:
            if (is_alpha_char(c)) {
                if (ctx->word_len < 31) {
                    ctx->word[ctx->word_len++] = c;
                } else {
                    ctx->current_state = ERROR;
                }
            } else if (c == ' ' || c == '\n' || c == '\0') {
                ctx->word[ctx->word_len] = '\0';
                ctx->total_words++;
                printf("Found word: %s\n", ctx->word);
                ctx->word_len = 0;
                memset(ctx->word, 0, sizeof(ctx->word));
                ctx->current_state = (c == '\n' || c == '\0') ? DONE : START;
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
    struct context ctx;
    char input[1024];
    
    init_context(&ctx);
    
    printf("Enter text (numbers and words separated by spaces): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    for (size_t i = 0; i < len && ctx.current_state != ERROR && ctx.current_state != DONE; i++) {
        process_char(&ctx, input[i]);
    }
    
    if (ctx.current_state == READING_NUMBER) {
        ctx.total_numbers++;
        printf("Found number: %d\n", ctx.number);
    } else if (ctx.current_state == READING_WORD) {
        ctx.word[ctx.word_len] = '\0';
        ctx.total_words++;
        printf("Found word: %s\n", ctx.word);
    }
    
    if (ctx.current_state == ERROR) {
        fprintf(stderr, "Error processing input\n");
        return 1;
    }
    
    printf("Summary: %d numbers, %d words\n", ctx.total_numbers, ctx.total_words);
    
    return 0;
}