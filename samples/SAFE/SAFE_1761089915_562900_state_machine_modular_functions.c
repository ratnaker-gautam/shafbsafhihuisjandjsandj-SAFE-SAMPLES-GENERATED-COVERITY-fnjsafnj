//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: modular_functions ; Variation: state_machine
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

Event get_event(char c) {
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
            ctx->current_word[0] = c;
            ctx->current_word[1] = '\0';
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
            if (ctx->current_number > (INT_MAX - (c - '0')) / 10) {
                return STATE_ERROR;
            }
            ctx->current_number = ctx->current_number * 10 + (c - '0');
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
        case EVENT_TERMINATOR:
            ctx->word_count++;
            return STATE_END;
        default:
            return STATE_ERROR;
    }
}

State handle_error(Event ev, char c, Context *ctx) {
    return STATE_ERROR;
}

State handle_end(Event ev, char c, Context *ctx) {
    return STATE_END;
}

void process_input(const char *input) {
    State current_state = STATE_START;
    Context ctx = {0, 0, 0, {0}};
    size_t input_len = strlen(input);
    
    if (input_len >= MAX_INPUT_LEN) {
        printf("Error: Input too long\n");
        return;
    }
    
    for (size_t i = 0; i <= input_len; i++) {
        char c = input[i];
        Event ev = get_event(c);
        
        State (*handlers[MAX_STATES])(Event, char, Context*) = {
            handle_start,
            handle_reading_number,
            handle_reading_word,
            handle_error,
            handle_end
        };
        
        if (current_state >= MAX_STATES) {
            printf("Error: Invalid state\n");
            return;
        }
        
        State next_state = handlers[current_state](ev, c, &ctx);
        
        if (next_state == STATE_ERROR) {
            printf("Error: Invalid input format\n");
            return;
        }
        
        current_state = next_state;
        
        if (current_state == STATE_END) {
            break;
        }
    }
    
    printf("Numbers: %d\n", ctx.number_count);
    printf("Words: %d\n", ctx.word_count);
}

int main(void) {
    char input[MAX_INPUT_LEN + 1] = {0};
    
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