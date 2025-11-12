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
    FINISHED,
    ERROR
};

struct context {
    enum state current;
    int number;
    char word[32];
    size_t word_len;
};

void state_machine_init(struct context *ctx) {
    if (ctx == NULL) return;
    ctx->current = START;
    ctx->number = 0;
    memset(ctx->word, 0, sizeof(ctx->word));
    ctx->word_len = 0;
}

enum state handle_start(struct context *ctx, char c) {
    if (c == 'q') return FINISHED;
    if (isdigit((unsigned char)c)) {
        ctx->number = c - '0';
        return READING_NUMBER;
    }
    if (isalpha((unsigned char)c)) {
        if (ctx->word_len < sizeof(ctx->word) - 1) {
            ctx->word[ctx->word_len++] = c;
        }
        return READING_WORD;
    }
    return ERROR;
}

enum state handle_reading_number(struct context *ctx, char c) {
    if (c == 'q') return FINISHED;
    if (isdigit((unsigned char)c)) {
        int digit = c - '0';
        if (ctx->number >= 0 && ctx->number <= (INT_MAX - digit) / 10) {
            ctx->number = ctx->number * 10 + digit;
            return READING_NUMBER;
        }
        return ERROR;
    }
    if (isalpha((unsigned char)c)) {
        if (ctx->word_len < sizeof(ctx->word) - 1) {
            ctx->word[ctx->word_len++] = c;
        }
        return READING_WORD;
    }
    return ERROR;
}

enum state handle_reading_word(struct context *ctx, char c) {
    if (c == 'q') return FINISHED;
    if (isalpha((unsigned char)c)) {
        if (ctx->word_len < sizeof(ctx->word) - 1) {
            ctx->word[ctx->word_len++] = c;
        }
        return READING_WORD;
    }
    if (isdigit((unsigned char)c)) {
        ctx->number = c - '0';
        return READING_NUMBER;
    }
    return ERROR;
}

void process_input(const char *input) {
    if (input == NULL) return;
    
    struct context ctx;
    state_machine_init(&ctx);
    
    size_t len = strlen(input);
    for (size_t i = 0; i < len; i++) {
        char c = input[i];
        
        switch (ctx.current) {
            case START:
                ctx.current = handle_start(&ctx, c);
                break;
            case READING_NUMBER:
                ctx.current = handle_reading_number(&ctx, c);
                break;
            case READING_WORD:
                ctx.current = handle_reading_word(&ctx, c);
                break;
            case FINISHED:
                return;
            case ERROR:
                printf("Error at position %zu\n", i);
                return;
        }
        
        if (ctx.current == FINISHED) {
            break;
        }
    }
    
    if (ctx.current == FINISHED) {
        printf("Finished by user request\n");
    } else if (ctx.current == READING_NUMBER) {
        printf("Final number: %d\n", ctx.number);
    } else if (ctx.current == READING_WORD) {
        if (ctx.word_len < sizeof(ctx.word)) {
            ctx.word[ctx.word_len] = '\0';
        } else {
            ctx.word[sizeof(ctx.word) - 1] = '\0';
        }
        printf("Final word: %s\n", ctx.word);
    } else if (ctx.current == START) {
        printf("No input processed\n");
    }
}

int main(void) {
    char buffer[256];
    
    printf("Enter input (q to finish): ");
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
    
    if (strlen(buffer) == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    process_input(buffer);
    
    return 0;
}