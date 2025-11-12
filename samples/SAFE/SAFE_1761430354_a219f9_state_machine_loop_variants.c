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
    if (c == '\n' || c == EOF) return FINISHED;
    if (isspace((unsigned char)c)) return START;
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
    if (c == '\n' || c == EOF) return FINISHED;
    if (isspace((unsigned char)c)) return START;
    if (isdigit((unsigned char)c)) {
        int digit = c - '0';
        if (ctx->number <= (INT_MAX - digit) / 10) {
            ctx->number = ctx->number * 10 + digit;
        } else {
            return ERROR;
        }
        return READING_NUMBER;
    }
    return ERROR;
}

enum state handle_reading_word(struct context *ctx, char c) {
    if (c == '\n' || c == EOF) return FINISHED;
    if (isspace((unsigned char)c)) return START;
    if (isalpha((unsigned char)c)) {
        if (ctx->word_len < sizeof(ctx->word) - 1) {
            ctx->word[ctx->word_len++] = c;
        }
        return READING_WORD;
    }
    return ERROR;
}

void process_input(const char *input) {
    struct context ctx;
    state_machine_init(&ctx);
    
    size_t i = 0;
    char c;
    while ((c = input[i]) != '\0') {
        enum state next_state;
        
        switch (ctx.current) {
            case START:
                next_state = handle_start(&ctx, c);
                break;
            case READING_NUMBER:
                next_state = handle_reading_number(&ctx, c);
                break;
            case READING_WORD:
                next_state = handle_reading_word(&ctx, c);
                break;
            case FINISHED:
            case ERROR:
                goto done;
            default:
                next_state = ERROR;
        }
        
        if (next_state == FINISHED || next_state == ERROR) {
            ctx.current = next_state;
            goto done;
        }
        
        ctx.current = next_state;
        i++;
        
        if (ctx.current == START) {
            if (ctx.word_len > 0) {
                printf("Word: %s\n", ctx.word);
                ctx.word_len = 0;
                memset(ctx.word, 0, sizeof(ctx.word));
            } else if (ctx.number != 0) {
                printf("Number: %d\n", ctx.number);
                ctx.number = 0;
            }
        }
    }
    
    if (ctx.current == READING_WORD && ctx.word_len > 0) {
        printf("Word: %s\n", ctx.word);
    } else if (ctx.current == READING_NUMBER && ctx.number != 0) {
        printf("Number: %d\n", ctx.number);
    }
    
done:
    if (ctx.current == ERROR) {
        printf("Error: Invalid input format\n");
    }
}

int main(void) {
    char input[1024];
    
    printf("Enter text (numbers and words separated by spaces): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }
    
    if (len == 1 && input[0] == '\n') {
        printf("No input provided\n");
        return 0;
    }
    
    process_input(input);
    return 0;
}