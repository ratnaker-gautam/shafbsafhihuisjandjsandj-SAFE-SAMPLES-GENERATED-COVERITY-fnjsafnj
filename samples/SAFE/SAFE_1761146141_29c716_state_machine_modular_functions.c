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
    if (c == '\0' || c == '\n') return EVENT_EOF;
    if (isspace(c)) return EVENT_SPACE;
    if (isdigit(c)) return EVENT_DIGIT;
    if (isalpha(c)) return EVENT_LETTER;
    return EVENT_INVALID;
}

State handle_start(Event ev, char c, Context* ctx) {
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

State handle_reading_number(Event ev, char c, Context* ctx) {
    if (ev == EVENT_DIGIT) {
        if (ctx->current_number <= (INT_MAX - (c - '0')) / 10) {
            ctx->current_number = ctx->current_number * 10 + (c - '0');
            return STATE_READING_NUMBER;
        }
        return STATE_ERROR;
    } else if (ev == EVENT_SPACE || ev == EVENT_EOF) {
        ctx->number_count++;
        printf("Found number: %d\n", ctx->current_number);
        if (ev == EVENT_EOF) return STATE_END;
        return STATE_START;
    }
    return STATE_ERROR;
}

State handle_reading_word(Event ev, char c, Context* ctx) {
    size_t len = strlen(ctx->current_word);
    if (ev == EVENT_LETTER) {
        if (len < MAX_INPUT_LEN - 1) {
            ctx->current_word[len] = c;
            ctx->current_word[len + 1] = '\0';
            return STATE_READING_WORD;
        }
        return STATE_ERROR;
    } else if (ev == EVENT_SPACE || ev == EVENT_EOF) {
        ctx->word_count++;
        printf("Found word: %s\n", ctx->current_word);
        if (ev == EVENT_EOF) return STATE_END;
        return STATE_START;
    }
    return STATE_ERROR;
}

State handle_error(Context* ctx) {
    printf("Error: Invalid input format\n");
    return STATE_END;
}

void process_input(const char* input) {
    State current_state = STATE_START;
    Context ctx = {0, 0, 0, {0}};
    size_t input_len = strlen(input);
    
    if (input_len >= MAX_INPUT_LEN) {
        printf("Error: Input too long\n");
        return;
    }
    
    for (size_t i = 0; i <= input_len && current_state != STATE_END && current_state != STATE_ERROR; i++) {
        char c = input[i];
        Event ev = get_event(c);
        
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
    
    printf("Summary: %d numbers, %d words\n", ctx.number_count, ctx.word_count);
}

int main() {
    char input[MAX_INPUT_LEN + 2];
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len >= MAX_INPUT_LEN) {
        printf("Error: Input too long\n");
        return 1;
    }
    
    process_input(input);
    return 0;
}