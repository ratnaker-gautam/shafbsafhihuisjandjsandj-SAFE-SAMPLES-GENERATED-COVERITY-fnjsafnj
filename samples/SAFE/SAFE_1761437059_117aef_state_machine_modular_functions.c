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
} State;

typedef enum {
    EVENT_DIGIT,
    EVENT_LETTER,
    EVENT_SPACE,
    EVENT_EOF,
    EVENT_INVALID
} Event;

typedef struct {
    int numbers_found;
    int words_found;
    char current_token[MAX_INPUT_LEN];
    int token_len;
} Context;

Event get_next_event(char c) {
    if (c == EOF) return EVENT_EOF;
    if (isspace(c)) return EVENT_SPACE;
    if (isdigit(c)) return EVENT_DIGIT;
    if (isalpha(c)) return EVENT_LETTER;
    return EVENT_INVALID;
}

State handle_start(Event ev, char c, Context *ctx) {
    switch (ev) {
        case EVENT_DIGIT:
            ctx->current_token[0] = c;
            ctx->token_len = 1;
            return STATE_READING_NUMBER;
        case EVENT_LETTER:
            ctx->current_token[0] = c;
            ctx->token_len = 1;
            return STATE_READING_WORD;
        case EVENT_SPACE:
            return STATE_START;
        case EVENT_EOF:
            return STATE_END;
        default:
            return STATE_ERROR;
    }
}

State handle_reading_number(Event ev, char c, Context *ctx) {
    switch (ev) {
        case EVENT_DIGIT:
            if (ctx->token_len < MAX_INPUT_LEN - 1) {
                ctx->current_token[ctx->token_len++] = c;
                return STATE_READING_NUMBER;
            }
            return STATE_ERROR;
        case EVENT_SPACE:
            ctx->current_token[ctx->token_len] = '\0';
            ctx->numbers_found++;
            printf("Found number: %s\n", ctx->current_token);
            return STATE_START;
        case EVENT_EOF:
            ctx->current_token[ctx->token_len] = '\0';
            ctx->numbers_found++;
            printf("Found number: %s\n", ctx->current_token);
            return STATE_END;
        default:
            return STATE_ERROR;
    }
}

State handle_reading_word(Event ev, char c, Context *ctx) {
    switch (ev) {
        case EVENT_LETTER:
            if (ctx->token_len < MAX_INPUT_LEN - 1) {
                ctx->current_token[ctx->token_len++] = c;
                return STATE_READING_WORD;
            }
            return STATE_ERROR;
        case EVENT_SPACE:
            ctx->current_token[ctx->token_len] = '\0';
            ctx->words_found++;
            printf("Found word: %s\n", ctx->current_token);
            return STATE_START;
        case EVENT_EOF:
            ctx->current_token[ctx->token_len] = '\0';
            ctx->words_found++;
            printf("Found word: %s\n", ctx->current_token);
            return STATE_END;
        default:
            return STATE_ERROR;
    }
}

State handle_error(Context *ctx) {
    printf("Error: Invalid input format\n");
    ctx->token_len = 0;
    return STATE_END;
}

void run_state_machine(void) {
    State current_state = STATE_START;
    Context ctx = {0};
    char input_char;
    
    while (current_state != STATE_END && current_state != STATE_ERROR) {
        input_char = getchar();
        Event ev = get_next_event(input_char);
        
        switch (current_state) {
            case STATE_START:
                current_state = handle_start(ev, input_char, &ctx);
                break;
            case STATE_READING_NUMBER:
                current_state = handle_reading_number(ev, input_char, &ctx);
                break;
            case STATE_READING_WORD:
                current_state = handle_reading_word(ev, input_char, &ctx);
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
    printf("Enter text (Ctrl+D to end):\n");
    run_state_machine();
    return 0;
}