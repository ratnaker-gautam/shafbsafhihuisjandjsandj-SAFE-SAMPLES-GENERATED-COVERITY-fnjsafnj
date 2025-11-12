//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10

typedef enum {
    STATE_START,
    STATE_READING_NUMBER,
    STATE_READING_WORD,
    STATE_ERROR,
    STATE_END
} parser_state_t;

typedef enum {
    EVENT_DIGIT,
    EVENT_LETTER,
    EVENT_SPACE,
    EVENT_EOF,
    EVENT_INVALID
} parser_event_t;

static parser_event_t get_event(char c) {
    if (c == '\0' || c == '\n') return EVENT_EOF;
    if (isspace(c)) return EVENT_SPACE;
    if (isdigit(c)) return EVENT_DIGIT;
    if (isalpha(c)) return EVENT_LETTER;
    return EVENT_INVALID;
}

static void process_token(const char* token, int len) {
    if (len > 0) {
        printf("Token: ");
        for (int i = 0; i < len; i++) {
            putchar(token[i]);
        }
        printf(" (");
        if (isdigit(token[0])) {
            printf("number");
        } else {
            printf("word");
        }
        printf(")\n");
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
    
    input[strcspn(input, "\n")] = '\0';
    
    parser_state_t state = STATE_START;
    int pos = 0;
    
    while (state != STATE_END && state != STATE_ERROR) {
        char current_char = input[pos];
        parser_event_t event = get_event(current_char);
        
        switch (state) {
            case STATE_START:
                switch (event) {
                    case EVENT_DIGIT:
                        token[0] = current_char;
                        token_len = 1;
                        state = STATE_READING_NUMBER;
                        break;
                    case EVENT_LETTER:
                        token[0] = current_char;
                        token_len = 1;
                        state = STATE_READING_WORD;
                        break;
                    case EVENT_SPACE:
                    case EVENT_EOF:
                        break;
                    case EVENT_INVALID:
                        state = STATE_ERROR;
                        break;
                }
                break;
                
            case STATE_READING_NUMBER:
                switch (event) {
                    case EVENT_DIGIT:
                        if (token_len < MAX_INPUT_LEN) {
                            token[token_len++] = current_char;
                        }
                        break;
                    case EVENT_SPACE:
                    case EVENT_EOF:
                        process_token(token, token_len);
                        token_len = 0;
                        state = (event == EVENT_EOF) ? STATE_END : STATE_START;
                        break;
                    case EVENT_LETTER:
                    case EVENT_INVALID:
                        state = STATE_ERROR;
                        break;
                }
                break;
                
            case STATE_READING_WORD:
                switch (event) {
                    case EVENT_LETTER:
                        if (token_len < MAX_INPUT_LEN) {
                            token[token_len++] = current_char;
                        }
                        break;
                    case EVENT_SPACE:
                    case EVENT_EOF:
                        process_token(token, token_len);
                        token_len = 0;
                        state = (event == EVENT_EOF) ? STATE_END : STATE_START;
                        break;
                    case EVENT_DIGIT:
                    case EVENT_INVALID:
                        state = STATE_ERROR;
                        break;
                }
                break;
                
            case STATE_ERROR:
                fprintf(stderr, "Parse error at position %d\n", pos);
                break;
                
            case STATE_END:
                break;
        }
        
        if (state != STATE_END && state != STATE_ERROR) {
            pos++;
            if (pos >= MAX_INPUT_LEN || input[pos] == '\0') {
                event = EVENT_EOF;
                current_char = '\0';
            }
        }
    }
    
    if (state == STATE_ERROR) {
        return 1;
    }
    
    return 0;
}