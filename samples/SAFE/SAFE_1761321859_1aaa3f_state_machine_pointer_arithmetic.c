//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT 256
#define STATE_COUNT 5

typedef enum {
    STATE_START,
    STATE_READING,
    STATE_NUMBER,
    STATE_WORD,
    STATE_END
} State;

typedef enum {
    EVENT_SPACE,
    EVENT_DIGIT,
    EVENT_ALPHA,
    EVENT_END,
    EVENT_INVALID
} Event;

Event get_event(char c) {
    if (c == '\0') return EVENT_END;
    if (isspace(c)) return EVENT_SPACE;
    if (isdigit(c)) return EVENT_DIGIT;
    if (isalpha(c)) return EVENT_ALPHA;
    return EVENT_INVALID;
}

void process_state_machine(const char *input) {
    State current_state = STATE_START;
    const char *ptr = input;
    int word_count = 0;
    int number_count = 0;
    
    while (current_state != STATE_END) {
        Event evt = get_event(*ptr);
        
        switch (current_state) {
            case STATE_START:
                if (evt == EVENT_SPACE) {
                    current_state = STATE_START;
                    ptr++;
                } else if (evt == EVENT_DIGIT) {
                    current_state = STATE_NUMBER;
                    number_count++;
                } else if (evt == EVENT_ALPHA) {
                    current_state = STATE_WORD;
                    word_count++;
                } else if (evt == EVENT_END) {
                    current_state = STATE_END;
                } else {
                    ptr++;
                }
                break;
                
            case STATE_READING:
                if (evt == EVENT_SPACE) {
                    current_state = STATE_START;
                } else if (evt == EVENT_END) {
                    current_state = STATE_END;
                }
                ptr++;
                break;
                
            case STATE_NUMBER:
                if (evt == EVENT_SPACE) {
                    current_state = STATE_START;
                } else if (evt == EVENT_END) {
                    current_state = STATE_END;
                } else if (evt != EVENT_DIGIT) {
                    current_state = STATE_READING;
                }
                ptr++;
                break;
                
            case STATE_WORD:
                if (evt == EVENT_SPACE) {
                    current_state = STATE_START;
                } else if (evt == EVENT_END) {
                    current_state = STATE_END;
                } else if (!isalpha(*ptr)) {
                    current_state = STATE_READING;
                }
                ptr++;
                break;
                
            case STATE_END:
                break;
        }
        
        if (ptr - input >= MAX_INPUT) {
            break;
        }
    }
    
    printf("Words: %d\nNumbers: %d\n", word_count, number_count);
}

int main() {
    char input[MAX_INPUT + 1] = {0};
    
    printf("Enter text (max %d characters): ", MAX_INPUT);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
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
    
    process_state_machine(input);
    return 0;
}