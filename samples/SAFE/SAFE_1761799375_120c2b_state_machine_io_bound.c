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
    int pos = 0;
    char current_char;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) >= MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    while (current_state != STATE_DONE && current_state != STATE_ERROR) {
        current_char = input[pos];
        Event event = get_event(current_char);
        
        switch (current_state) {
            case STATE_START:
                switch (event) {
                    case EVENT_DIGIT:
                        current_state = STATE_READING_NUMBER;
                        number_count++;
                        break;
                    case EVENT_LETTER:
                        current_state = STATE_READING_WORD;
                        word_count++;
                        break;
                    case EVENT_SPACE:
                        break;
                    case EVENT_EOF:
                        current_state = STATE_DONE;
                        break;
                    case EVENT_INVALID:
                        current_state = STATE_ERROR;
                        break;
                }
                break;
                
            case STATE_READING_NUMBER:
                switch (event) {
                    case EVENT_DIGIT:
                        break;
                    case EVENT_SPACE:
                        current_state = STATE_START;
                        break;
                    case EVENT_LETTER:
                    case EVENT_INVALID:
                        current_state = STATE_ERROR;
                        break;
                    case EVENT_EOF:
                        current_state = STATE_DONE;
                        break;
                }
                break;
                
            case STATE_READING_WORD:
                switch (event) {
                    case EVENT_LETTER:
                        break;
                    case EVENT_SPACE:
                        current_state = STATE_START;
                        break;
                    case EVENT_DIGIT:
                    case EVENT_INVALID:
                        current_state = STATE_ERROR;
                        break;
                    case EVENT_EOF:
                        current_state = STATE_DONE;
                        break;
                }
                break;
                
            case STATE_ERROR:
                fprintf(stderr, "Invalid input format at position %d\n", pos);
                return 1;
                
            case STATE_DONE:
                break;
        }
        
        if (current_state != STATE_DONE && current_state != STATE_ERROR) {
            pos++;
            if (pos > MAX_INPUT_LEN) {
                current_state = STATE_ERROR;
            }
        }
    }
    
    if (current_state == STATE_ERROR) {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }
    
    printf("Numbers found: %d\n", number_count);
    printf("Words found: %d\n", word_count);
    
    return 0;
}