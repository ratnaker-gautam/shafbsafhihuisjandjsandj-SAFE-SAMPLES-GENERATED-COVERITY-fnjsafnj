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
    int number_count;
    int word_count;
    int current_number;
    char current_word[MAX_INPUT_LEN];
} Context;

Event get_event(char c) {
    if (c == EOF) return EVENT_EOF;
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
            if (ctx != NULL) {
                ctx->current_word[0] = c;
                ctx->current_word[1] = '\0';
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

State handle_reading_number(Event ev, char c, Context *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (ev) {
        case EVENT_DIGIT:
            if (ctx->current_number > (INT_MAX - (c - '0')) / 10) {
                return STATE_ERROR;
            }
            ctx->current_number = ctx->current_number * 10 + (c - '0');
            return STATE_READING_NUMBER;
        case EVENT_SPACE:
            ctx->number_count++;
            return STATE_START;
        case EVENT_EOF:
            ctx->number_count++;
            return STATE_END;
        default:
            return STATE_ERROR;
    }
}

State handle_reading_word(Event ev, char c, Context *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    size_t len = strlen(ctx->current_word);
    if (len >= MAX_INPUT_LEN - 1) {
        return STATE_ERROR;
    }
    
    switch (ev) {
        case EVENT_LETTER:
            ctx->current_word[len] = c;
            ctx->current_word[len + 1] = '\0';
            return STATE_READING_WORD;
        case EVENT_SPACE:
            ctx->word_count++;
            return STATE_START;
        case EVENT_EOF:
            ctx->word_count++;
            return STATE_END;
        default:
            return STATE_ERROR;
    }
}

State handle_error(Context *ctx) {
    if (ctx != NULL) {
        ctx->number_count = 0;
        ctx->word_count = 0;
    }
    return STATE_END;
}

void process_input(Context *ctx) {
    if (ctx == NULL) return;
    
    State current_state = STATE_START;
    int input_char;
    
    while (current_state != STATE_END && current_state != STATE_ERROR) {
        input_char = getchar();
        if (input_char == EOF && ferror(stdin)) {
            current_state = STATE_ERROR;
            break;
        }
        Event event = get_event(input_char);
        
        switch (current_state) {
            case STATE_START:
                current_state = handle_start(event, input_char, ctx);
                break;
            case STATE_READING_NUMBER:
                current_state = handle_reading_number(event, input_char, ctx);
                break;
            case STATE_READING_WORD:
                current_state = handle_reading_word(event, input_char, ctx);
                break;
            case STATE_ERROR:
                current_state = handle_error(ctx);
                break;
            default:
                current_state = STATE_ERROR;
                break;
        }
    }
    
    if (current_state == STATE_ERROR) {
        handle_error(ctx);
    }
}

int main(void) {
    Context ctx = {0, 0, 0, {0}};
    
    printf("Enter text (numbers and words): ");
    process_input(&ctx);
    
    printf("Numbers found: %d\n", ctx.number_count);
    printf("Words found: %d\n", ctx.word_count);
    
    return 0;
}