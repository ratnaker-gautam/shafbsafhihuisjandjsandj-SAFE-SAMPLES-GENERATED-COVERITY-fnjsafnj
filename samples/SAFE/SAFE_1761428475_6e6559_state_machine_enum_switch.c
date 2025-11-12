//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

enum State {
    INIT,
    READING_NUMBER,
    READING_WORD,
    SKIPPING_SPACE,
    FINISHED,
    ERROR
};

struct Context {
    enum State current_state;
    int number_count;
    int word_count;
    int current_number;
    char current_word[32];
    int word_pos;
};

void init_context(struct Context *ctx) {
    ctx->current_state = INIT;
    ctx->number_count = 0;
    ctx->word_count = 0;
    ctx->current_number = 0;
    memset(ctx->current_word, 0, sizeof(ctx->current_word));
    ctx->word_pos = 0;
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
        case INIT:
            if (isdigit(c)) {
                ctx->current_state = READING_NUMBER;
                ctx->current_number = c - '0';
            } else if (isalpha(c)) {
                ctx->current_state = READING_WORD;
                ctx->current_word[0] = c;
                ctx->word_pos = 1;
            } else if (isspace(c)) {
                ctx->current_state = SKIPPING_SPACE;
            } else if (c == '\0') {
                ctx->current_state = FINISHED;
            }
            break;

        case READING_NUMBER:
            if (isdigit(c)) {
                int digit = c - '0';
                if (ctx->current_number <= (INT_MAX - digit) / 10) {
                    ctx->current_number = ctx->current_number * 10 + digit;
                } else {
                    ctx->current_state = ERROR;
                }
            } else if (isspace(c) || c == '\0') {
                ctx->number_count++;
                ctx->current_state = (c == '\0') ? FINISHED : SKIPPING_SPACE;
            } else {
                ctx->current_state = ERROR;
            }
            break;

        case READING_WORD:
            if (isalpha(c)) {
                if (ctx->word_pos < 31) {
                    ctx->current_word[ctx->word_pos++] = c;
                } else {
                    ctx->current_state = ERROR;
                }
            } else if (isspace(c) || c == '\0') {
                ctx->current_word[ctx->word_pos] = '\0';
                ctx->word_count++;
                ctx->current_state = (c == '\0') ? FINISHED : SKIPPING_SPACE;
            } else {
                ctx->current_state = ERROR;
            }
            break;

        case SKIPPING_SPACE:
            if (isdigit(c)) {
                ctx->current_state = READING_NUMBER;
                ctx->current_number = c - '0';
            } else if (isalpha(c)) {
                ctx->current_state = READING_WORD;
                ctx->current_word[0] = c;
                ctx->word_pos = 1;
            } else if (isspace(c)) {
                break;
            } else if (c == '\0') {
                ctx->current_state = FINISHED;
            } else {
                ctx->current_state = ERROR;
            }
            break;

        case FINISHED:
        case ERROR:
            break;
    }
}

int main(void) {
    struct Context ctx;
    char input[1024];
    
    printf("Enter text (max 1023 chars): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len >= sizeof(input)) {
        printf("Input too long\n");
        return 1;
    }
    
    init_context(&ctx);
    
    for (size_t i = 0; i <= len; i++) {
        process_char(&ctx, input[i]);
        if (ctx.current_state == ERROR) {
            printf("Error: Invalid input format\n");
            return 1;
        }
        if (ctx.current_state == FINISHED) {
            break;
        }
    }
    
    if (ctx.current_state != FINISHED) {
        printf("Error: Incomplete processing\n");
        return 1;
    }
    
    printf("Numbers found: %d\n", ctx.number_count);
    printf("Words found: %d\n", ctx.word_count);
    
    return 0;
}