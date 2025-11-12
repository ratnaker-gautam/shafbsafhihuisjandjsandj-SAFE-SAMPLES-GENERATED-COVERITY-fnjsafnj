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
    EVENT_TERMINATOR,
    EVENT_INVALID
} Event;

typedef struct {
    int number_count;
    int word_count;
    int current_number;
    char current_word[MAX_INPUT_LEN];
} Context;

Event classify_char(char c) {
    if (c == '\0') return EVENT_TERMINATOR;
    if (isspace((unsigned char)c)) return EVENT_SPACE;
    if (isdigit((unsigned char)c)) return EVENT_DIGIT;
    if (isalpha((unsigned char)c)) return EVENT_LETTER;
    return EVENT_INVALID;
}

State handle_start(Event ev, char c, Context *ctx) {
    switch (ev) {
        case EVENT_DIGIT:
            ctx->current_number = c - '0';
            return STATE_READING_NUMBER;
        case EVENT_LETTER:
            if (ctx->current_word[0] == '\0') {
                ctx->current_word[0] = c;
                ctx->current_word[1] = '\0';
            }
            return STATE_READING_WORD;
        case EVENT_SPACE:
            return STATE_START;
        case EVENT_TERMINATOR:
            return STATE_END;
        default:
            return STATE_ERROR;
    }
}

State handle_reading_number(Event ev, char c, Context *ctx) {
    switch (ev) {
        case EVENT_DIGIT:
            if (ctx->current_number <= (INT_MAX - (c - '0')) / 10) {
                ctx->current_number = ctx->current_number * 10 + (c - '0');
            } else {
                return STATE_ERROR;
            }
            return STATE_READING_NUMBER;
        case EVENT_SPACE:
            ctx->number_count++;
            return STATE_START;
        case EVENT_TERMINATOR:
            ctx->number_count++;
            return STATE_END;
        default:
            return STATE_ERROR;
    }
}

State handle_reading_word(Event ev, char c, Context *ctx) {
    size_t len = strlen(ctx->current_word);
    switch (ev) {
        case EVENT_LETTER:
            if (len < MAX_INPUT_LEN - 1) {
                ctx->current_word[len] = c;
                ctx->current_word[len + 1] = '\0';
            } else {
                return STATE_ERROR;
            }
            return STATE_READING_WORD;
        case EVENT_SPACE:
            ctx->word_count++;
            ctx->current_word[0] = '\0';
            return STATE_START;
        case EVENT_TERMINATOR:
            ctx->word_count++;
            ctx->current_word[0] = '\0';
            return STATE_END;
        default:
            return STATE_ERROR;
    }
}

State handle_error(Context *ctx) {
    ctx->current_word[0] = '\0';
    ctx->current_number = 0;
    return STATE_ERROR;
}

void process_input(const char *input, Context *ctx) {
    State current_state = STATE_START;
    size_t i = 0;
    
    while (current_state != STATE_END && current_state != STATE_ERROR) {
        if (i >= MAX_INPUT_LEN) {
            current_state = STATE_ERROR;
            break;
        }
        
        char c = input[i];
        Event ev = classify_char(c);
        
        switch (current_state) {
            case STATE_START:
                current_state = handle_start(ev, c, ctx);
                break;
            case STATE_READING_NUMBER:
                current_state = handle_reading_number(ev, c, ctx);
                break;
            case STATE_READING_WORD:
                current_state = handle_reading_word(ev, c, ctx);
                break;
            case STATE_ERROR:
                current_state = handle_error(ctx);
                break;
            default:
                current_state = STATE_ERROR;
                break;
        }
        
        i++;
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    Context ctx = {0, 0, 0, {0}};
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    process_input(input, &ctx);
    
    printf("Numbers found: %d\n", ctx.number_count);
    printf("Words found: %d\n", ctx.word_count);
    
    return 0;
}