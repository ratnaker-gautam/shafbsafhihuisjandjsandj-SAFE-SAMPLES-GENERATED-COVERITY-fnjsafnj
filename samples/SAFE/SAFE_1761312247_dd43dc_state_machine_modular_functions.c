//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <limits.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10

typedef enum {
    STATE_START,
    STATE_READING_NUMBER,
    STATE_READING_WORD,
    STATE_ERROR,
    STATE_END
} state_t;

typedef enum {
    EVENT_DIGIT,
    EVENT_LETTER,
    EVENT_SPACE,
    EVENT_EOF,
    EVENT_INVALID
} event_t;

typedef struct {
    int numbers_found;
    int words_found;
    int current_number;
    char current_word[MAX_INPUT_LEN];
    int word_pos;
} context_t;

static event_t get_event(int c) {
    if (c == EOF) return EVENT_EOF;
    if (isspace(c)) return EVENT_SPACE;
    if (isdigit(c)) return EVENT_DIGIT;
    if (isalpha(c)) return EVENT_LETTER;
    return EVENT_INVALID;
}

static state_t handle_start(event_t event, int c, context_t *ctx) {
    if (event == EVENT_DIGIT) {
        ctx->current_number = c - '0';
        return STATE_READING_NUMBER;
    } else if (event == EVENT_LETTER) {
        if (ctx->word_pos < MAX_INPUT_LEN - 1) {
            ctx->current_word[ctx->word_pos++] = (char)c;
            ctx->current_word[ctx->word_pos] = '\0';
        }
        return STATE_READING_WORD;
    } else if (event == EVENT_SPACE || event == EVENT_EOF) {
        return STATE_START;
    }
    return STATE_ERROR;
}

static state_t handle_reading_number(event_t event, int c, context_t *ctx) {
    if (event == EVENT_DIGIT) {
        int digit = c - '0';
        if (ctx->current_number <= (INT_MAX - digit) / 10) {
            ctx->current_number = ctx->current_number * 10 + digit;
        } else {
            return STATE_ERROR;
        }
        return STATE_READING_NUMBER;
    } else if (event == EVENT_SPACE || event == EVENT_EOF) {
        ctx->numbers_found++;
        printf("Found number: %d\n", ctx->current_number);
        ctx->current_number = 0;
        return (event == EVENT_EOF) ? STATE_END : STATE_START;
    }
    return STATE_ERROR;
}

static state_t handle_reading_word(event_t event, int c, context_t *ctx) {
    if (event == EVENT_LETTER) {
        if (ctx->word_pos < MAX_INPUT_LEN - 1) {
            ctx->current_word[ctx->word_pos++] = (char)c;
            ctx->current_word[ctx->word_pos] = '\0';
        }
        return STATE_READING_WORD;
    } else if (event == EVENT_SPACE || event == EVENT_EOF) {
        ctx->words_found++;
        printf("Found word: %s\n", ctx->current_word);
        ctx->word_pos = 0;
        ctx->current_word[0] = '\0';
        return (event == EVENT_EOF) ? STATE_END : STATE_START;
    }
    return STATE_ERROR;
}

static state_t handle_error(context_t *ctx) {
    printf("Error: Invalid input format\n");
    ctx->current_number = 0;
    ctx->word_pos = 0;
    ctx->current_word[0] = '\0';
    return STATE_END;
}

static void run_state_machine(void) {
    state_t current_state = STATE_START;
    context_t ctx = {0};
    int c;
    
    while (current_state != STATE_END && current_state != STATE_ERROR) {
        c = getchar();
        if (c == EOF) {
            if (feof(stdin)) {
                c = EOF;
            } else {
                current_state = STATE_ERROR;
                break;
            }
        }
        
        event_t event = get_event(c);
        
        switch (current_state) {
            case STATE_START:
                current_state = handle_start(event, c, &ctx);
                break;
            case STATE_READING_NUMBER:
                current_state = handle_reading_number(event, c, &ctx);
                break;
            case STATE_READING_WORD:
                current_state = handle_reading_word(event, c, &ctx);
                break;
            case STATE_ERROR:
                current_state = handle_error(&ctx);
                break;
            case STATE_END:
                break;
        }
    }
    
    if (current_state == STATE_ERROR) {
        handle_error(&ctx);
    }
    
    printf("Summary: %d numbers, %d words\n", ctx.numbers_found, ctx.words_found);
}

int main(void) {
    printf("Enter text (mix of numbers and words): ");
    run_state_machine();
    return 0;
}