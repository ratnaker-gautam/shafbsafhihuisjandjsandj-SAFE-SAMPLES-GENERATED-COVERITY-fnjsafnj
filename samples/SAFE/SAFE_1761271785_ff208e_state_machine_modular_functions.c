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

Event classify_char(char c) {
    if (c == '\0' || c == EOF) return EVENT_EOF;
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
        case EVENT_EOF:
            return STATE_START;
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
        case EVENT_EOF:
            ctx->current_token[ctx->token_len] = '\0';
            ctx->numbers_found++;
            printf("Found number: %s\n", ctx->current_token);
            ctx->token_len = 0;
            return (ev == EVENT_EOF) ? STATE_END : STATE_START;
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
        case EVENT_EOF:
            ctx->current_token[ctx->token_len] = '\0';
            ctx->words_found++;
            printf("Found word: %s\n", ctx->current_token);
            ctx->token_len = 0;
            return (ev == EVENT_EOF) ? STATE_END : STATE_START;
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
    char input[MAX_INPUT_LEN];
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    for (size_t i = 0; i <= input_len && current_state != STATE_END && current_state != STATE_ERROR; i++) {
        char c = (i < input_len) ? input[i] : '\0';
        Event ev = classify_char(c);
        
        switch (current_state) {
            case STATE_START:
                current_state = handle_start(ev, c, &ctx);
                break;
            case STATE_READING_NUMBER:
                current_state = handle_reading_number(ev, c, &ctx);
                break;
            case STATE_READING_WORD:
                current_state = handle_reading_word(ev, c, &ctx);
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
    run_state_machine();
    return 0;
}