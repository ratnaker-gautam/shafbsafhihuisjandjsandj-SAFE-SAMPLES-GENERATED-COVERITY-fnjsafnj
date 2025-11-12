//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

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
    char current_word[MAX_INPUT_LEN];
    int word_pos;
} context_t;

static event_t classify_char(int c) {
    if (c == EOF) return EVENT_EOF;
    if (isspace(c)) return EVENT_SPACE;
    if (isdigit(c)) return EVENT_DIGIT;
    if (isalpha(c)) return EVENT_LETTER;
    return EVENT_INVALID;
}

static state_t handle_start(event_t event, int c, context_t *ctx) {
    switch (event) {
        case EVENT_DIGIT:
            ctx->numbers_found++;
            return STATE_READING_NUMBER;
        case EVENT_LETTER:
            ctx->words_found++;
            ctx->word_pos = 0;
            if (ctx->word_pos < MAX_INPUT_LEN - 1) {
                ctx->current_word[ctx->word_pos++] = (char)c;
                ctx->current_word[ctx->word_pos] = '\0';
            }
            return STATE_READING_WORD;
        case EVENT_SPACE:
            return STATE_START;
        case EVENT_EOF:
            return STATE_END;
        default:
            return STATE_ERROR;
    }
}

static state_t handle_reading_number(event_t event, int c, context_t *ctx) {
    switch (event) {
        case EVENT_DIGIT:
            return STATE_READING_NUMBER;
        case EVENT_SPACE:
            return STATE_START;
        case EVENT_EOF:
            return STATE_END;
        default:
            return STATE_ERROR;
    }
}

static state_t handle_reading_word(event_t event, int c, context_t *ctx) {
    switch (event) {
        case EVENT_LETTER:
            if (ctx->word_pos < MAX_INPUT_LEN - 1) {
                ctx->current_word[ctx->word_pos++] = (char)c;
                ctx->current_word[ctx->word_pos] = '\0';
            }
            return STATE_READING_WORD;
        case EVENT_SPACE:
            printf("Word found: %s\n", ctx->current_word);
            return STATE_START;
        case EVENT_EOF:
            printf("Word found: %s\n", ctx->current_word);
            return STATE_END;
        default:
            return STATE_ERROR;
    }
}

static state_t handle_error(context_t *ctx) {
    printf("Invalid input character encountered\n");
    ctx->numbers_found = 0;
    ctx->words_found = 0;
    return STATE_END;
}

static void process_input(void) {
    state_t current_state = STATE_START;
    context_t ctx = {0, 0, {0}, 0};
    int c;
    
    while (current_state != STATE_END && current_state != STATE_ERROR) {
        c = getchar();
        if (c == EOF && feof(stdin)) break;
        
        event_t event = classify_char(c);
        
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
            default:
                current_state = STATE_ERROR;
                break;
        }
    }
    
    if (current_state == STATE_ERROR) {
        handle_error(&ctx);
    }
    
    printf("Summary: %d numbers, %d words\n", ctx.numbers_found, ctx.words_found);
}

int main(void) {
    printf("Enter text (Ctrl+D to end): ");
    process_input();
    return 0;
}