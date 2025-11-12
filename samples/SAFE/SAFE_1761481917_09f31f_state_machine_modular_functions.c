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
    int current_word_len;
} Context;

Event get_event(char c) {
    if (c == '\0' || c == EOF) return EVENT_EOF;
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
        ctx->current_word_len = 1;
        return STATE_READING_WORD;
    } else if (ev == EVENT_SPACE || ev == EVENT_EOF) {
        return STATE_START;
    }
    return STATE_ERROR;
}

State handle_reading_number(Event ev, char c, Context* ctx) {
    if (ev == EVENT_DIGIT) {
        int digit = c - '0';
        if (ctx->current_number <= (INT_MAX - digit) / 10) {
            ctx->current_number = ctx->current_number * 10 + digit;
            return STATE_READING_NUMBER;
        }
        return STATE_ERROR;
    } else if (ev == EVENT_SPACE || ev == EVENT_EOF) {
        ctx->number_count++;
        return (ev == EVENT_EOF) ? STATE_END : STATE_START;
    }
    return STATE_ERROR;
}

State handle_reading_word(Event ev, char c, Context* ctx) {
    if (ev == EVENT_LETTER) {
        if (ctx->current_word_len < MAX_INPUT_LEN - 1) {
            ctx->current_word_len++;
            return STATE_READING_WORD;
        }
        return STATE_ERROR;
    } else if (ev == EVENT_SPACE || ev == EVENT_EOF) {
        ctx->word_count++;
        return (ev == EVENT_EOF) ? STATE_END : STATE_START;
    }
    return STATE_ERROR;
}

State handle_error(Context* ctx) {
    ctx->number_count = 0;
    ctx->word_count = 0;
    return STATE_END;
}

void process_input(const char* input) {
    State current_state = STATE_START;
    Context ctx = {0, 0, 0, 0};
    size_t input_len = strlen(input);
    
    if (input_len >= MAX_INPUT_LEN) {
        printf("Input too long\n");
        return;
    }
    
    for (size_t i = 0; i <= input_len; i++) {
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
                goto done;
        }
        
        if (current_state == STATE_ERROR) {
            current_state = handle_error(&ctx);
        }
    }
    
done:
    if (current_state == STATE_END || current_state == STATE_ERROR) {
        printf("Numbers: %d, Words: %d\n", ctx.number_count, ctx.word_count);
    }
}

int main() {
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