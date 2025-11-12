//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 5

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

Event get_event(char c) {
    if (c == '\0' || c == '\n') return EVENT_EOF;
    if (isspace(c)) return EVENT_SPACE;
    if (isdigit(c)) return EVENT_DIGIT;
    if (isalpha(c)) return EVENT_LETTER;
    return EVENT_INVALID;
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
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
        printf("No input provided\n");
        return 0;
    }
    
    State current_state = STATE_START;
    int pos = 0;
    
    while (current_state != STATE_END && current_state != STATE_ERROR && pos < len) {
        Event ev = get_event(input[pos]);
        
        switch (current_state) {
            case STATE_START:
                if (ev == EVENT_DIGIT) {
                    current_state = STATE_READING_NUMBER;
                    numbers_found++;
                } else if (ev == EVENT_LETTER) {
                    current_state = STATE_READING_WORD;
                    words_found++;
                } else if (ev == EVENT_SPACE) {
                    pos++;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_NUMBER:
                if (ev == EVENT_DIGIT) {
                    pos++;
                } else if (ev == EVENT_SPACE || ev == EVENT_EOF) {
                    current_state = STATE_START;
                    pos++;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_WORD:
                if (ev == EVENT_LETTER) {
                    pos++;
                } else if (ev == EVENT_SPACE || ev == EVENT_EOF) {
                    current_state = STATE_START;
                    pos++;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_ERROR:
                printf("Invalid input format at position %d\n", pos);
                return 1;
                
            case STATE_END:
                break;
        }
    }
    
    if (current_state == STATE_ERROR) {
        printf("Invalid input format at position %d\n", pos);
        return 1;
    }
    
    printf("Analysis complete:\n");
    printf("Numbers found: %d\n", numbers_found);
    printf("Words found: %d\n", words_found);
    
    return 0;
}