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
    STATE_READING_NUM,
    STATE_READING_ALPHA,
    STATE_READING_OTHER,
    STATE_END
} parser_state_t;

typedef enum {
    EVENT_DIGIT,
    EVENT_ALPHA,
    EVENT_SPACE,
    EVENT_OTHER,
    EVENT_EOF
} parser_event_t;

static parser_event_t get_event(char c) {
    if (c == '\0') return EVENT_EOF;
    if (isspace((unsigned char)c)) return EVENT_SPACE;
    if (isdigit((unsigned char)c)) return EVENT_DIGIT;
    if (isalpha((unsigned char)c)) return EVENT_ALPHA;
    return EVENT_OTHER;
}

static void process_token(parser_state_t state, const char* token, int len) {
    if (len <= 0) return;
    
    switch (state) {
        case STATE_READING_NUM:
            printf("Found number: %.*s\n", len, token);
            break;
        case STATE_READING_ALPHA:
            printf("Found word: %.*s\n", len, token);
            break;
        case STATE_READING_OTHER:
            printf("Found symbol: %.*s\n", len, token);
            break;
        default:
            break;
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    char token[MAX_INPUT_LEN + 1];
    int token_len = 0;
    
    printf("Enter text to parse: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 0;
    }
    
    parser_state_t current_state = STATE_START;
    int processed = 0;
    
    while (processed <= (int)input_len) {
        char current_char = input[processed];
        parser_event_t event = get_event(current_char);
        
        parser_state_t next_state = current_state;
        
        switch (current_state) {
            case STATE_START:
                switch (event) {
                    case EVENT_DIGIT:
                        next_state = STATE_READING_NUM;
                        token[token_len++] = current_char;
                        break;
                    case EVENT_ALPHA:
                        next_state = STATE_READING_ALPHA;
                        token[token_len++] = current_char;
                        break;
                    case EVENT_OTHER:
                        next_state = STATE_READING_OTHER;
                        token[token_len++] = current_char;
                        break;
                    case EVENT_SPACE:
                    case EVENT_EOF:
                        next_state = STATE_START;
                        break;
                }
                break;
                
            case STATE_READING_NUM:
                switch (event) {
                    case EVENT_DIGIT:
                        if (token_len < MAX_INPUT_LEN) {
                            token[token_len++] = current_char;
                        }
                        next_state = STATE_READING_NUM;
                        break;
                    default:
                        process_token(STATE_READING_NUM, token, token_len);
                        token_len = 0;
                        if (event == EVENT_ALPHA) {
                            token[token_len++] = current_char;
                            next_state = STATE_READING_ALPHA;
                        } else if (event == EVENT_OTHER) {
                            token[token_len++] = current_char;
                            next_state = STATE_READING_OTHER;
                        } else {
                            next_state = STATE_START;
                        }
                        break;
                }
                break;
                
            case STATE_READING_ALPHA:
                switch (event) {
                    case EVENT_ALPHA:
                        if (token_len < MAX_INPUT_LEN) {
                            token[token_len++] = current_char;
                        }
                        next_state = STATE_READING_ALPHA;
                        break;
                    default:
                        process_token(STATE_READING_ALPHA, token, token_len);
                        token_len = 0;
                        if (event == EVENT_DIGIT) {
                            token[token_len++] = current_char;
                            next_state = STATE_READING_NUM;
                        } else if (event == EVENT_OTHER) {
                            token[token_len++] = current_char;
                            next_state = STATE_READING_OTHER;
                        } else {
                            next_state = STATE_START;
                        }
                        break;
                }
                break;
                
            case STATE_READING_OTHER:
                switch (event) {
                    case EVENT_OTHER:
                        if (token_len < MAX_INPUT_LEN) {
                            token[token_len++] = current_char;
                        }
                        next_state = STATE_READING_OTHER;
                        break;
                    default:
                        process_token(STATE_READING_OTHER, token, token_len);
                        token_len = 0;
                        if (event == EVENT_DIGIT) {
                            token[token_len++] = current_char;
                            next_state = STATE_READING_NUM;
                        } else if (event == EVENT_ALPHA) {
                            token[token_len++] = current_char;
                            next_state = STATE