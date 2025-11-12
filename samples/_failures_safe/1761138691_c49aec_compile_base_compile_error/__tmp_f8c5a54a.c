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
            if (ctx->word_pos < MAX_INPUT_LEN - 1) {
                ctx->current_word[ctx->word_pos++] = c;
                ctx->current_word[ctx->word_pos] = '\0';
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
    switch (ev) {
        case EVENT_DIGIT:
            if (ctx->current_number <= (INT_MAX - (c - '0')) / 10) {
                ctx->current_number = ctx->current_number * 10 + (c - '0');
            }
            return STATE_READING_NUMBER;
        case EVENT_SPACE:
            ctx->numbers_found++;
            return STATE_START;
        case EVENT_EOF:
            ctx->numbers_found++;
            return STATE_END;
        default:
            return STATE_ERROR;
    }
}

State handle_reading_word(Event ev, char c, Context *ctx) {
    switch (ev) {
        case EVENT_LETTER:
            if (ctx->word_pos < MAX_INPUT_LEN - 1) {
                ctx->current_word[ctx->word_pos++] = c;
                ctx->current_word[ctx->word_pos] = '\0';
            }
            return STATE_READING_WORD;
        case EVENT_SPACE:
            if (ctx->word_pos > 0) {
                ctx->words_found++;
            }
            ctx->word_pos = 0;
            ctx->current_word[0] = '\0';
            return STATE_START;
        case EVENT_EOF:
            if (ctx->word_pos > 0) {
                ctx->words_found++;
            }
            return STATE_END;
        default:
            return STATE_ERROR;
    }
}

State handle_error(Context *ctx) {
    ctx->word_pos = 0;
    ctx->current_word[0] = '\0';
    return STATE_START;
}

void initialize_context(Context *ctx) {
    ctx->numbers_found = 0;
    ctx->words_found = 0;
    ctx->current_number = 0;
    ctx->word_pos = 0;
    ctx->current_word[0] = '\0';
}

void print_results(const Context *ctx) {
    printf("Numbers found: %d\n", ctx->numbers_found);
    printf("Words found: %d\n", ctx->words_found);
}

int main(void) {
    Context ctx;
    initialize_context(&ctx);
    
    State current_state = STATE_START;
    char input_buffer[MAX_INPUT_LEN + 2];
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len > MAX_INPUT_LEN) {
        printf("Input too long\n");
        return 1;
    }
    
    for (size_t i = 0; i <= input_len; i++) {
        char current_char = (i < input_len) ? input_buffer[i] : EOF;
        Event event = get_event(current_char);
        
        State next_state = current_state;
        
        switch (current_state) {
            case STATE_START:
                next_state = handle_start(event, current_char, &ctx);
                break;
            case STATE_READING_NUMBER:
                next_state = handle_reading_number(event, current_char, &ctx);
                break;
            case STATE_READING_WORD:
                next_state = handle_reading_word(event, current_char, &ctx);
                break;
            case STATE_ERROR:
                next_state = handle_error(&ctx);
                break;
            case STATE_END:
                break;
        }
        
        if (next_state == STATE_ERROR) {
            next_state = handle_error(&ctx);
        }
        
        current_state = next_state;
        
        if (current_state ==