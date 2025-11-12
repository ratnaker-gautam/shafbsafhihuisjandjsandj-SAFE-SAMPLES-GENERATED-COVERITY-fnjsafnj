//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT 256
#define MAX_STATES 8

typedef enum {
    STATE_START,
    STATE_READING_WORD,
    STATE_READING_NUMBER,
    STATE_PUNCTUATION,
    STATE_END,
    STATE_ERROR
} parser_state_t;

typedef enum {
    EVENT_CHAR,
    EVENT_SPACE,
    EVENT_PUNCT,
    EVENT_EOF,
    EVENT_INVALID
} parser_event_t;

static parser_event_t get_event(char c) {
    if (c == '\0') return EVENT_EOF;
    if (isspace((unsigned char)c)) return EVENT_SPACE;
    if (isalpha((unsigned char)c)) return EVENT_CHAR;
    if (isdigit((unsigned char)c)) return EVENT_CHAR;
    if (ispunct((unsigned char)c)) return EVENT_PUNCT;
    return EVENT_INVALID;
}

static void process_state(parser_state_t *state, parser_event_t event, char *input, char **current) {
    static int word_count = 0;
    static int number_count = 0;
    static int punct_count = 0;
    
    switch (*state) {
        case STATE_START:
            if (event == EVENT_CHAR) {
                if (isdigit((unsigned char)**current)) {
                    *state = STATE_READING_NUMBER;
                    number_count++;
                } else {
                    *state = STATE_READING_WORD;
                    word_count++;
                }
            } else if (event == EVENT_PUNCT) {
                *state = STATE_PUNCTUATION;
                punct_count++;
            } else if (event == EVENT_SPACE) {
                *state = STATE_START;
            } else if (event == EVENT_EOF) {
                *state = STATE_END;
            } else {
                *state = STATE_ERROR;
            }
            break;
            
        case STATE_READING_WORD:
            if (event == EVENT_SPACE) {
                *state = STATE_START;
            } else if (event == EVENT_PUNCT) {
                *state = STATE_PUNCTUATION;
                punct_count++;
            } else if (event == EVENT_EOF) {
                *state = STATE_END;
            } else if (event != EVENT_CHAR) {
                *state = STATE_ERROR;
            }
            break;
            
        case STATE_READING_NUMBER:
            if (event == EVENT_SPACE) {
                *state = STATE_START;
            } else if (event == EVENT_PUNCT) {
                *state = STATE_PUNCTUATION;
                punct_count++;
            } else if (event == EVENT_EOF) {
                *state = STATE_END;
            } else if (!isdigit((unsigned char)**current)) {
                *state = STATE_ERROR;
            }
            break;
            
        case STATE_PUNCTUATION:
            if (event == EVENT_SPACE) {
                *state = STATE_START;
            } else if (event == EVENT_EOF) {
                *state = STATE_END;
            } else if (event == EVENT_CHAR) {
                if (isdigit((unsigned char)**current)) {
                    *state = STATE_READING_NUMBER;
                    number_count++;
                } else {
                    *state = STATE_READING_WORD;
                    word_count++;
                }
            } else {
                *state = STATE_ERROR;
            }
            break;
            
        case STATE_END:
            printf("Words: %d, Numbers: %d, Punctuation: %d\n", word_count, number_count, punct_count);
            break;
            
        case STATE_ERROR:
            fprintf(stderr, "Invalid input sequence\n");
            exit(1);
            break;
    }
}

int main(void) {
    char input[MAX_INPUT + 1] = {0};
    char *current = input;
    parser_state_t state = STATE_START;
    
    printf("Enter text to analyze: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Failed to read input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len >= sizeof(input)) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    while (state != STATE_END && state != STATE_ERROR) {
        if (current >= input + sizeof(input)) {
            state = STATE_ERROR;
            break;
        }
        
        parser_event_t event = get_event(*current);
        process_state(&state, event, input, &current);
        
        if (state != STATE_END && state != STATE_ERROR) {
            current++;
        }
    }
    
    if (state == STATE_ERROR) {
        fprintf(stderr, "Parsing failed\n");
        return 1;
    }
    
    return 0;
}