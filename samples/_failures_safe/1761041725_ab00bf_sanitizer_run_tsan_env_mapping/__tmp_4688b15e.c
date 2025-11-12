//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: state_machine
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

State current_state = STATE_START;
int number_count = 0;
int word_count = 0;

Event get_event(char c) {
    if (c == EOF) return EVENT_EOF;
    if (isspace(c)) return EVENT_SPACE;
    if (isdigit(c)) return EVENT_DIGIT;
    if (isalpha(c)) return EVENT_LETTER;
    return EVENT_INVALID;
}

void process_state_machine(char c) {
    Event event = get_event(c);
    
    switch (current_state) {
        case STATE_START:
            if (event == EVENT_DIGIT) {
                current_state = STATE_READING_NUMBER;
            } else if (event == EVENT_LETTER) {
                current_state = STATE_READING_WORD;
            } else if (event == EVENT_SPACE) {
                current_state = STATE_START;
            } else if (event == EVENT_EOF) {
                current_state = STATE_END;
            } else {
                current_state = STATE_ERROR;
            }
            break;
            
        case STATE_READING_NUMBER:
            if (event == EVENT_SPACE) {
                number_count++;
                current_state = STATE_START;
            } else if (event == EVENT_EOF) {
                number_count++;
                current_state = STATE_END;
            } else if (event != EVENT_DIGIT) {
                current_state = STATE_ERROR;
            }
            break;
            
        case STATE_READING_WORD:
            if (event == EVENT_SPACE) {
                word_count++;
                current_state = STATE_START;
            } else if (event == EVENT_EOF) {
                word_count++;
                current_state = STATE_END;
            } else if (event != EVENT_LETTER) {
                current_state = STATE_ERROR;
            }
            break;
            
        case STATE_ERROR:
            if (event == EVENT_SPACE) {
                current_state = STATE_START;
            } else if (event == EVENT_EOF) {
                current_state = STATE_END;
            }
            break;
            
        case STATE_END:
            break;
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    int input_len;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    for (int i = 0; i <= input_len; i++) {
        char c = (i < input_len) ? input[i] : EOF;
        process_state_machine(c);
        
        if (current_state == STATE_ERROR) {
            printf("Invalid input format at position %d\n", i + 1);
            return 1;
        }
        
        if (current_state == STATE_END) {
            break;
        }
    }
    
    printf("Numbers found: %d\n", number_count);
    printf("Words found: %d\n", word_count);
    
    return 0;
}