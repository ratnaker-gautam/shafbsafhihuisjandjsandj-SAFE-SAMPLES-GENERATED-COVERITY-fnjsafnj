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
    if (start == NULL || end == NULL || start >= end) return;
    
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
    char *buffer = input;
    
    printf("Enter text to analyze: ");
    if (fgets(buffer, MAX_INPUT, stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(buffer);
    if (input_len > 0 && buffer[input_len - 1] == '\n') {
        buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 0;
    }
    
    parser_state_t current_state = STATE_START;
    char *token_start = NULL;
    char *current_pos = buffer;
    int word_count = 0;
    int number_count = 0;
    
    while (current_state != STATE_END && current_state != STATE_ERROR) {
        char current_char = *current_pos;
        parser_event_t event = get_event(current_char);
        
        switch (current_state) {
            case STATE_START:
                switch (event) {
                    case EVENT_ALPHA:
                        current_state = STATE_READING_WORD;
                        token_start = current_pos;
                        break;
                    case EVENT_DIGIT:
                        current_state = STATE_READING_NUMBER;
                        token_start = current_pos;
                        break;
                    case EVENT_PUNCT:
                        current_state = STATE_PUNCTUATION;
                        token_start = current_pos;
                        break;
                    case EVENT_SPACE:
                        break;
                    case EVENT_END:
                        current_state = STATE_END;
                        break;
                    default:
                        current_state = STATE_ERROR;
                        break;
                }
                break;
                
            case STATE_READING_WORD:
                switch (event) {
                    case EVENT_SPACE:
                    case EVENT_END:
                        process_token(token_start, current_pos, &word_count, &number_count);
                        current_state = (event == EVENT_END) ? STATE_END : STATE_START;
                        break;
                    case EVENT_PUNCT:
                        process_token(token_start, current_pos, &word_count, &number_count);
                        current_state = STATE_PUNCTUATION;
                        token_start = current_pos;
                        break;
                    case EVENT_ALPHA:
                    case EVENT_DIGIT:
                        break;
                    default:
                        current_state = STATE_ERROR;
                        break;
                }
                break;
                
            case STATE_READING_NUMBER:
                switch (event) {
                    case EVENT_SPACE:
                    case EVENT_END:
                        process_token(token_start, current_pos, &word_count, &number_count);
                        current_state = (event == EVENT_END) ? STATE_END : STATE_START;
                        break;
                    case EVENT_PUNCT:
                        process_token(token_start, current_pos, &word_count, &number_count);
                        current_state = STATE_PUNCTUATION;
                        token_start = current_pos;
                        break;
                    case EVENT_DIGIT:
                        break;
                    default:
                        current_state = STATE_ERROR;
                        break;
                }
                break;
                
            case STATE_PUNCTUATION:
                switch (event) {
                    case EVENT_SPACE:
                    case EVENT_END:
                        current_state = (event == EVENT_END) ? STATE_END : STATE_START;
                        break;
                    case EVENT_ALPHA:
                        current_state = STATE_READING_WORD;
                        token_start = current_pos;
                        break;
                    case EVENT_DIGIT:
                        current_state = STATE_READING_NUMBER;
                        token_start = current_pos;
                        break;
                    case EVENT_PUNCT:
                        break;
                    default:
                        current_state = STATE_ERROR;
                        break;
                }
                break;
                
            default:
                current_state = STATE_ERROR;
                break;
        }
        
        if (current_state != STATE_END && current_state != STATE_ERROR) {
            current_pos++;
            if (current_pos - buffer > MAX_INPUT) {
                current_state = STATE_ERROR;
            }
        }
    }
    
    if (current_state == STATE_ERROR) {
        fprintf(stderr, "Error parsing input\n");
        return 1;
    }