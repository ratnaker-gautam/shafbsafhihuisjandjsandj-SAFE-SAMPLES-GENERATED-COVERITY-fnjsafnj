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

int is_whitespace(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

int is_digit(char c) {
    return c >= '0' && c <= '9';
}

int is_alpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

void process_char(struct context *ctx, char c) {
    switch (ctx->current_state) {
        case START:
            if (is_digit(c)) {
                ctx->number = c - '0';
                ctx->current_state = READING_NUMBER;
            } else if (is_alpha(c)) {
                if (ctx->word_len < 31) {
                    ctx->word[ctx->word_len++] = tolower(c);
                    ctx->current_state = READING_WORD;
                } else {
                    ctx->current_state = ERROR;
                }
            } else if (!is_whitespace(c)) {
                ctx->current_state = ERROR;
            }
            break;
            
        case READING_NUMBER:
            if (is_digit(c)) {
                int digit = c - '0';
                if (ctx->number <= (INT_MAX - digit) / 10) {
                    ctx->number = ctx->number * 10 + digit;
                } else {
                    ctx->current_state = ERROR;
                }
            } else if (is_whitespace(c)) {
                ctx->total_numbers++;
                ctx->current_state = START;
            } else {
                ctx->current_state = ERROR;
            }
            break;
            
        case READING_WORD:
            if (is_alpha(c)) {
                if (ctx->word_len < 31) {
                    ctx->word[ctx->word_len++] = tolower(c);
                } else {
                    ctx->current_state = ERROR;
                }
            } else if (is_whitespace(c)) {
                ctx->word[ctx->word_len] = '\0';
                ctx->total_words++;
                ctx->word_len = 0;
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

void finalize_processing(struct context *ctx) {
    switch (ctx->current_state) {
        case READING_NUMBER:
            ctx->total_numbers++;
            break;
            
        case READING_WORD:
            if (ctx->word_len > 0) {
                ctx->word[ctx->word_len] = '\0';
                ctx->total_words++;
            }
            break;
            
        case ERROR:
            break;
            
        default:
            break;
    }
    ctx->current_state = DONE;
}

int main(void) {
    struct context ctx;
    init_context(&ctx);
    
    printf("Enter text (mix of numbers and words): ");
    
    int c;
    while ((c = getchar()) != EOF && c != '\n') {
        if (ctx.current_state == ERROR) {
            break;
        }
        process_char(&ctx, (char)c);
    }
    
    if (ctx.current_state != ERROR) {
        finalize_processing(&ctx);
    }
    
    if (ctx.current_state == ERROR) {
        printf("Error: Invalid input format\n");
        return 1;
    }
    
    printf("Total numbers found: %d\n", ctx.total_numbers);
    printf("Total words found: %d\n", ctx.total_words);
    
    return 0;
}