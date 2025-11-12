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
    if (c == EOF) return EVENT_EOF;
    if (isspace(c)) return EVENT_SPACE;
    if (isdigit(c)) return EVENT_DIGIT;
    if (isalpha(c)) return EVENT_LETTER;
    return EVENT_INVALID;
}

int main(void) {
    State current_state = STATE_START;
    char input[MAX_INPUT_LEN + 1];
    char buffer[MAX_INPUT_LEN + 1];
    int buf_pos = 0;
    int numbers_found = 0;
    int words_found = 0;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("Empty input\n");
        return 0;
    }
    
    for (size_t i = 0; i <= len; i++) {
        char c = input[i];
        Event event = get_event(c);
        
        switch (current_state) {
            case STATE_START:
                if (event == EVENT_DIGIT) {
                    buffer[buf_pos++] = c;
                    current_state = STATE_READING_NUMBER;
                } else if (event == EVENT_LETTER) {
                    buffer[buf_pos++] = c;
                    current_state = STATE_READING_WORD;
                } else if (event == EVENT_SPACE || event == EVENT_EOF) {
                    continue;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_NUMBER:
                if (event == EVENT_DIGIT) {
                    if (buf_pos < MAX_INPUT_LEN) {
                        buffer[buf_pos++] = c;
                    } else {
                        current_state = STATE_ERROR;
                    }
                } else if (event == EVENT_SPACE || event == EVENT_EOF) {
                    buffer[buf_pos] = '\0';
                    printf("Found number: %s\n", buffer);
                    numbers_found++;
                    buf_pos = 0;
                    current_state = STATE_START;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_WORD:
                if (event == EVENT_LETTER) {
                    if (buf_pos < MAX_INPUT_LEN) {
                        buffer[buf_pos++] = c;
                    } else {
                        current_state = STATE_ERROR;
                    }
                } else if (event == EVENT_SPACE || event == EVENT_EOF) {
                    buffer[buf_pos] = '\0';
                    printf("Found word: %s\n", buffer);
                    words_found++;
                    buf_pos = 0;
                    current_state = STATE_START;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_ERROR:
                fprintf(stderr, "Invalid input format\n");
                return 1;
                
            case STATE_DONE:
                break;
        }
        
        if (current_state == STATE_ERROR) {
            fprintf(stderr, "Invalid input format\n");
            return 1;
        }
    }
    
    printf("Summary: %d numbers, %d words\n", numbers_found, words_found);
    return 0;
}