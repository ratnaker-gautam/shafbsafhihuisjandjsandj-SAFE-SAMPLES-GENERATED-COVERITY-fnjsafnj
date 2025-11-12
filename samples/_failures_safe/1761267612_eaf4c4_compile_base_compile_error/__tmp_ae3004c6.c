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
    EVENT_SPACE,
    EVENT_ALPHA,
    EVENT_DIGIT,
    EVENT_PUNCT,
    EVENT_END,
    EVENT_INVALID
} parser_event_t;

static parser_event_t get_event(char c) {
    if (c == '\0') return EVENT_END;
    if (isspace(c)) return EVENT_SPACE;
    if (isalpha(c)) return EVENT_ALPHA;
    if (isdigit(c)) return EVENT_DIGIT;
    if (ispunct(c)) return EVENT_PUNCT;
    return EVENT_INVALID;
}

static void process_token(char *start, char *end, int *word_count, int *number_count) {
    if (start >= end) return;
    
    size_t len = end - start;
    if (len >= MAX_INPUT) return;
    
    if (isdigit(*start)) {
        (*number_count)++;
    } else if (isalpha(*start)) {
        (*word_count)++;
    }
}

int main(void) {
    char input[MAX_INPUT + 1] = {0};
    char buffer[MAX_INPUT + 1] = {0};
    
    printf("Enter text to analyze: ");
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
    
    if (input_len >= MAX_INPUT) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    memcpy(buffer, input, input_len + 1);
    
    parser_state_t state = STATE_START;
    parser_state_t next_state = STATE_START;
    
    char *current = buffer;
    char *token_start = NULL;
    int word_count = 0;
    int number_count = 0;
    
    while (state != STATE_END && state != STATE_ERROR) {
        parser_event_t event = get_event(*current);
        
        switch (state) {
            case STATE_START:
                switch (event) {
                    case EVENT_ALPHA:
                        next_state = STATE_READING_WORD;
                        token_start = current;
                        break;
                    case EVENT_DIGIT:
                        next_state = STATE_READING_NUMBER;
                        token_start = current;
                        break;
                    case EVENT_PUNCT:
                        next_state = STATE_PUNCTUATION;
                        token_start = current;
                        break;
                    case EVENT_SPACE:
                        next_state = STATE_START;
                        break;
                    case EVENT_END:
                        next_state = STATE_END;
                        break;
                    default:
                        next_state = STATE_ERROR;
                        break;
                }
                break;
                
            case STATE_READING_WORD:
                switch (event) {
                    case EVENT_SPACE:
                    case EVENT_END:
                        process_token(token_start, current, &word_count, &number_count);
                        next_state = (event == EVENT_END) ? STATE_END : STATE_START;
                        break;
                    case EVENT_PUNCT:
                        process_token(token_start, current, &word_count, &number_count);
                        next_state = STATE_PUNCTUATION;
                        token_start = current;
                        break;
                    case EVENT_ALPHA:
                    case EVENT_DIGIT:
                        next_state = STATE_READING_WORD;
                        break;
                    default:
                        next_state = STATE_ERROR;
                        break;
                }
                break;
                
            case STATE_READING_NUMBER:
                switch (event) {
                    case EVENT_SPACE:
                    case EVENT_END:
                        process_token(token_start, current, &word_count, &number_count);
                        next_state = (event == EVENT_END) ? STATE_END : STATE_START;
                        break;
                    case EVENT_PUNCT:
                        process_token(token_start, current, &word_count, &number_count);
                        next_state = STATE_PUNCTUATION;
                        token_start = current;
                        break;
                    case EVENT_DIGIT:
                        next_state = STATE_READING_NUMBER;
                        break;
                    default:
                        next_state = STATE_ERROR;
                        break;
                }
                break;
                
            case STATE_PUNCTUATION:
                switch (event) {
                    case EVENT_SPACE:
                    case EVENT_END:
                        next_state = (event == EVENT_END) ? STATE_END : STATE_START;
                        break;
                    case EVENT_ALPHA:
                        next_state = STATE_READING_WORD;
                        token_start = current;
                        break;
                    case EVENT_DIGIT:
                        next_state = STATE_READING_NUMBER;
                        token_start = current;
                        break;
                    case EVENT_PUNCT:
                        next_state = STATE_PUNCTUATION;
                        break;
                    default:
                        next_state = STATE_ERROR;
                        break;
                }
                break;
                
            default:
                next_state = STATE_ERROR;
                break;
        }
        
        if (state == STATE_ERROR