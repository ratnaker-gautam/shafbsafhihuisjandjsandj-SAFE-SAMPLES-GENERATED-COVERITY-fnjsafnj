//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
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
    EVENT_EOF,
    EVENT_INVALID
} Event;

Event get_event(char c) {
    if (c == '\0') return EVENT_EOF;
    if (isspace(c)) return EVENT_SPACE;
    if (isdigit(c)) return EVENT_DIGIT;
    if (isalpha(c)) return EVENT_ALPHA;
    return EVENT_INVALID;
}

void process_state_machine(const char *input) {
    State current_state = STATE_START;
    const char *ptr = input;
    char buffer[MAX_INPUT];
    char *buf_ptr = buffer;
    
    while (current_state != STATE_END) {
        Event event = get_event(*ptr);
        
        switch (current_state) {
            case STATE_START:
                if (event == EVENT_SPACE) {
                    current_state = STATE_START;
                    ptr++;
                } else if (event == EVENT_DIGIT) {
                    current_state = STATE_NUMBER;
                    *buf_ptr = *ptr;
                    buf_ptr++;
                    ptr++;
                } else if (event == EVENT_ALPHA) {
                    current_state = STATE_WORD;
                    *buf_ptr = *ptr;
                    buf_ptr++;
                    ptr++;
                } else if (event == EVENT_EOF) {
                    current_state = STATE_END;
                } else {
                    ptr++;
                }
                break;
                
            case STATE_NUMBER:
                if (event == EVENT_DIGIT) {
                    if ((buf_ptr - buffer) < MAX_INPUT - 1) {
                        *buf_ptr = *ptr;
                        buf_ptr++;
                    }
                    ptr++;
                } else {
                    *buf_ptr = '\0';
                    printf("Found number: %s\n", buffer);
                    buf_ptr = buffer;
                    current_state = STATE_START;
                }
                break;
                
            case STATE_WORD:
                if (event == EVENT_ALPHA) {
                    if ((buf_ptr - buffer) < MAX_INPUT - 1) {
                        *buf_ptr = *ptr;
                        buf_ptr++;
                    }
                    ptr++;
                } else {
                    *buf_ptr = '\0';
                    printf("Found word: %s\n", buffer);
                    buf_ptr = buffer;
                    current_state = STATE_START;
                }
                break;
                
            default:
                current_state = STATE_END;
                break;
        }
        
        if (ptr - input > MAX_INPUT * 2) {
            break;
        }
    }
}

int main() {
    char input[MAX_INPUT];
    
    printf("Enter text to analyze: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    process_state_machine(input);
    return 0;
}