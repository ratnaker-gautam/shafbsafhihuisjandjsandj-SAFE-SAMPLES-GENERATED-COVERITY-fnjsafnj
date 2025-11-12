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
    int current_number;
    char current_word[MAX_INPUT_LEN];
    int word_pos;
} Context;

Event classify_char(char c) {
    if (c == '\0' || c == '\n') return EVENT_EOF;
    if (isspace(c)) return EVENT_SPACE;
    if (isdigit(c)) return EVENT_DIGIT;
    if (isalpha(c)) return EVENT_LETTER;
    return EVENT_INVALID;
}

State handle_start(Event ev, char c, Context *ctx) {
    switch (ev) {
        case EVENT_DIGIT:
            ctx->current_number = c - '0';
            return STATE_READING_NUMBER;
        case EVENT_LETTER:
            ctx->current_word[0] = c;
            ctx->word_pos = 1;
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
            if (ctx->current_number >= 0 && ctx->current_number <= (INT_MAX - (c - '0')) / 10) {
                ctx->current_number = ctx->current_number * 10 + (c - '0');
                return STATE_READING_NUMBER;
            }
            return STATE_ERROR;
        case EVENT_SPACE:
        case EVENT_EOF:
            ctx->numbers_found++;
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

State handle_reading_word(Event ev, char c, Context *ctx) {
    switch (ev) {
        case EVENT_LETTER:
            if (ctx->word_pos >= 0 && ctx->word_pos < MAX_INPUT_LEN - 1) {
                ctx->current_word[ctx->word_pos++] = c;
                return STATE_READING_WORD;
            }
            return STATE_ERROR;
        case EVENT_SPACE:
        case EVENT_EOF:
            if (ctx->word_pos > 0 && ctx->word_pos < MAX_INPUT_LEN) {
                ctx->current_word[ctx->word_pos] = '\0';
                ctx->words_found++;
            }
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

State handle_error(Context *ctx) {
    ctx->numbers_found = 0;
    ctx->words_found = 0;
    return STATE_END;
}

void process_input(const char *input) {
    State current_state = STATE_START;
    Context ctx = {0, 0, 0, {0}, 0};
    size_t input_len = strnlen(input, MAX_INPUT_LEN);
    
    for (size_t i = 0; i <= input_len; i++) {
        char c = input[i];
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
                goto done;
        }
        
        if (current_state == STATE_ERROR) {
            current_state = handle_error(&ctx);
        }
    }
    
done:
    if (current_state == STATE_END) {
        printf("Invalid input format\n");
    } else {
        printf("Found %d numbers and %d words\n", ctx.numbers_found, ctx.words_found);
    }
}

int main() {
    char input[MAX_INPUT_LEN + 1];
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    process_input(input);
    return 0;
}