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
            if (ctx->current_number <= (INT_MAX - (c - '0')) / 10) {
                ctx->current_number = ctx->current_number * 10 + (c - '0');
                return STATE_READING_NUMBER;
            }
            return STATE_ERROR;
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
                return STATE_READING_WORD;
            }
            return STATE_ERROR;
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

State process_state(State current, Event ev, char c, Context *ctx) {
    switch (current) {
        case STATE_START:
            return handle_start(ev, c, ctx);
        case STATE_READING_NUMBER:
            return handle_reading_number(ev, c, ctx);
        case STATE_READING_WORD:
            return handle_reading_word(ev, c, ctx);
        case STATE_ERROR:
            return handle_error(ev, c, ctx);
        case STATE_END:
            return STATE_END;
        default:
            return STATE_ERROR;
    }
}

int validate_input(const char *input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isalnum((unsigned char)input[i]) && !isspace((unsigned char)input[i])) {
            return 0;
        }
    }
    return 1;
}

int main(void) {
    char input[MAX_INPUT_LEN + 2];
    Context ctx = {0, 0, 0, {0}};
    State current_state = STATE_START;
    
    printf("Enter text (alphanumeric and spaces only): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!validate_input(input)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    for (size_t i = 0; i < strlen(input) + 1; i++) {
        char c = input[i];
        Event ev = get_event(c);
        current_state = process_state(current_state, ev, c, &ctx);
        
        if (current_state == STATE_ERROR) {
            fprintf(stderr, "Error processing input at position %zu\n", i);
            return 1;
        }
        
        if (current_state == STATE_END) {
            break;
        }
    }
    
    printf("Numbers found: %d\n", ctx.number_count);
    printf("Words found: %d\n", ctx.word_count);
    
    return 0;
}