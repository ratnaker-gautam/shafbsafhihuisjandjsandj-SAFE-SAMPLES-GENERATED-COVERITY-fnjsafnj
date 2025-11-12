//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: state_machine
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
    STATE_DONE
} State;

typedef enum {
    EVENT_DIGIT,
    EVENT_LETTER,
    EVENT_SPACE,
    EVENT_EOF,
    EVENT_INVALID
} Event;

Event get_event(char c) {
    if (c == '\0' || c == EOF) return EVENT_EOF;
    if (isspace(c)) return EVENT_SPACE;
    if (isdigit(c)) return EVENT_DIGIT;
    if (isalpha(c)) return EVENT_LETTER;
    return EVENT_INVALID;
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    State current_state = STATE_START;
    int number_count = 0;
    int word_count = 0;
    char current_token[MAX_INPUT_LEN + 1];
    int token_pos = 0;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    for (int i = 0; input[i] != '\0'; i++) {
        Event event = get_event(input[i]);
        
        switch (current_state) {
            case STATE_START:
                if (event == EVENT_DIGIT) {
                    current_state = STATE_READING_NUMBER;
                    token_pos = 0;
                    if (token_pos < MAX_INPUT_LEN) {
                        current_token[token_pos++] = input[i];
                    }
                } else if (event == EVENT_LETTER) {
                    current_state = STATE_READING_WORD;
                    token_pos = 0;
                    if (token_pos < MAX_INPUT_LEN) {
                        current_token[token_pos++] = input[i];
                    }
                } else if (event == EVENT_SPACE) {
                    continue;
                } else if (event == EVENT_INVALID) {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_NUMBER:
                if (event == EVENT_DIGIT) {
                    if (token_pos < MAX_INPUT_LEN) {
                        current_token[token_pos++] = input[i];
                    }
                } else if (event == EVENT_SPACE || event == EVENT_EOF) {
                    current_token[token_pos] = '\0';
                    number_count++;
                    printf("Found number: %s\n", current_token);
                    current_state = STATE_START;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_WORD:
                if (event == EVENT_LETTER) {
                    if (token_pos < MAX_INPUT_LEN) {
                        current_token[token_pos++] = input[i];
                    }
                } else if (event == EVENT_SPACE || event == EVENT_EOF) {
                    current_token[token_pos] = '\0';
                    word_count++;
                    printf("Found word: %s\n", current_token);
                    current_state = STATE_START;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_ERROR:
                fprintf(stderr, "Invalid input at position %d\n", i);
                return 1;
                
            case STATE_DONE:
                break;
        }
        
        if (current_state == STATE_ERROR) {
            fprintf(stderr, "Invalid input at position %d\n", i);
            return 1;
        }
    }
    
    if (current_state == STATE_READING_NUMBER) {
        current_token[token_pos] = '\0';
        number_count++;
        printf("Found number: %s\n", current_token);
    } else if (current_state == STATE_READING_WORD) {
        current_token[token_pos] = '\0';
        word_count++;
        printf("Found word: %s\n", current_token);
    }
    
    printf("\nSummary: %d numbers, %d words\n", number_count, word_count);
    
    return 0;
}