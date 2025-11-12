//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define STATE_COUNT 5

typedef enum {
    STATE_START,
    STATE_READING_DIGITS,
    STATE_READING_LETTERS,
    STATE_READING_SYMBOLS,
    STATE_ERROR
} parser_state_t;

typedef enum {
    EVENT_DIGIT,
    EVENT_LETTER,
    EVENT_SYMBOL,
    EVENT_SPACE,
    EVENT_END,
    EVENT_INVALID
} parser_event_t;

static parser_event_t classify_char(char c) {
    if (c == '\0') return EVENT_END;
    if (isspace((unsigned char)c)) return EVENT_SPACE;
    if (isdigit((unsigned char)c)) return EVENT_DIGIT;
    if (isalpha((unsigned char)c)) return EVENT_LETTER;
    if (c >= 33 && c <= 126) return EVENT_SYMBOL;
    return EVENT_INVALID;
}

static parser_state_t transition(parser_state_t current, parser_event_t event) {
    static const parser_state_t transitions[STATE_COUNT][6] = {
        {STATE_READING_DIGITS, STATE_READING_LETTERS, STATE_READING_SYMBOLS, STATE_START, STATE_START, STATE_ERROR},
        {STATE_READING_DIGITS, STATE_ERROR, STATE_ERROR, STATE_START, STATE_START, STATE_ERROR},
        {STATE_ERROR, STATE_READING_LETTERS, STATE_ERROR, STATE_START, STATE_START, STATE_ERROR},
        {STATE_ERROR, STATE_ERROR, STATE_READING_SYMBOLS, STATE_START, STATE_START, STATE_ERROR},
        {STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR}
    };
    
    if (current >= STATE_COUNT || event > EVENT_INVALID) return STATE_ERROR;
    return transitions[current][event];
}

static void process_token(parser_state_t state, const char* token, size_t len) {
    if (len == 0) return;
    
    switch (state) {
        case STATE_READING_DIGITS:
            printf("NUMBER: ");
            break;
        case STATE_READING_LETTERS:
            printf("WORD: ");
            break;
        case STATE_READING_SYMBOLS:
            printf("SYMBOL: ");
            break;
        default:
            return;
    }
    
    for (size_t i = 0; i < len; i++) {
        putchar(token[i]);
    }
    putchar('\n');
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    char token[MAX_INPUT_LEN + 1];
    size_t token_len = 0;
    
    printf("Enter text to parse: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len >= MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    parser_state_t current_state = STATE_START;
    parser_state_t last_valid_state = STATE_START;
    
    for (size_t i = 0; i <= input_len; i++) {
        char c = input[i];
        parser_event_t event = classify_char(c);
        parser_state_t next_state = transition(current_state, event);
        
        if (next_state == STATE_ERROR) {
            fprintf(stderr, "Parse error at position %zu\n", i);
            return 1;
        }
        
        if (current_state != STATE_START && next_state == STATE_START) {
            process_token(last_valid_state, token, token_len);
            token_len = 0;
        }
        
        if (next_state != STATE_START && event != EVENT_SPACE && event != EVENT_END) {
            if (token_len < MAX_INPUT_LEN) {
                token[token_len++] = c;
            } else {
                fprintf(stderr, "Token too long\n");
                return 1;
            }
        }
        
        if (next_state != STATE_START) {
            last_valid_state = next_state;
        }
        
        current_state = next_state;
    }
    
    if (current_state != STATE_START) {
        process_token(last_valid_state, token, token_len);
    }
    
    return 0;
}