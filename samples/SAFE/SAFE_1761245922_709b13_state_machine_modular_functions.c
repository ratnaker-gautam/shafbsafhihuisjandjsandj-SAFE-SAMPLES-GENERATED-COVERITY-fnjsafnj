//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

Event get_next_event(char c) {
    if (c == '\0' || c == '\n') return EVENT_EOF;
    if (isspace(c)) return EVENT_SPACE;
    if (isdigit(c)) return EVENT_DIGIT;
    if (isalpha(c)) return EVENT_LETTER;
    return EVENT_INVALID;
}

State handle_start(Event ev, char c, Context *ctx) {
    if (ev == EVENT_DIGIT) {
        ctx->current_number = c - '0';
        return STATE_READING_NUMBER;
    } else if (ev == EVENT_LETTER) {
        ctx->current_word[0] = c;
        ctx->current_word[1] = '\0';
        return STATE_READING_WORD;
    } else if (ev == EVENT_SPACE) {
        return STATE_START;
    } else if (ev == EVENT_EOF) {
        return STATE_END;
    }
    return STATE_ERROR;
}

State handle_reading_number(Event ev, char c, Context *ctx) {
    if (ev == EVENT_DIGIT) {
        int digit = c - '0';
        if (ctx->current_number <= (INT_MAX - digit) / 10) {
            ctx->current_number = ctx->current_number * 10 + digit;
            return STATE_READING_NUMBER;
        }
        return STATE_ERROR;
    } else if (ev == EVENT_SPACE || ev == EVENT_EOF) {
        ctx->number_count++;
        if (ev == EVENT_SPACE) return STATE_START;
        return STATE_END;
    }
    return STATE_ERROR;
}

State handle_reading_word(Event ev, char c, Context *ctx) {
    if (ev == EVENT_LETTER) {
        size_t len = strlen(ctx->current_word);
        if (len < MAX_INPUT_LEN - 1) {
            ctx->current_word[len] = c;
            ctx->current_word[len + 1] = '\0';
            return STATE_READING_WORD;
        }
        return STATE_ERROR;
    } else if (ev == EVENT_SPACE || ev == EVENT_EOF) {
        ctx->word_count++;
        if (ev == EVENT_SPACE) return STATE_START;
        return STATE_END;
    }
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
        case STATE_END:
            return current;
        default:
            return STATE_ERROR;
    }
}

int process_input(const char *input, Context *ctx) {
    State current_state = STATE_START;
    size_t i = 0;
    
    while (current_state != STATE_END && current_state != STATE_ERROR) {
        if (i >= MAX_INPUT_LEN) {
            return 0;
        }
        
        char c = input[i];
        Event ev = get_next_event(c);
        current_state = process_state(current_state, ev, c, ctx);
        
        if (current_state == STATE_ERROR) {
            return 0;
        }
        
        if (ev != EVENT_EOF) {
            i++;
        }
    }
    
    return 1;
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    Context ctx = {0, 0, 0, {0}};
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (process_input(input, &ctx)) {
        printf("Numbers found: %d\n", ctx.number_count);
        printf("Words found: %d\n", ctx.word_count);
    } else {
        printf("Error processing input\n");
        return 1;
    }
    
    return 0;
}