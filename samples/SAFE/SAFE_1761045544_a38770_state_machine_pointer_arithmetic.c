//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT 256
#define STATE_COUNT 5

typedef enum {
    STATE_START,
    STATE_READING_WORD,
    STATE_READING_NUMBER,
    STATE_SKIPPING,
    STATE_END
} State;

typedef enum {
    EVENT_SPACE,
    EVENT_DIGIT,
    EVENT_ALPHA,
    EVENT_OTHER,
    EVENT_END
} Event;

Event get_event(char c) {
    if (c == '\0') return EVENT_END;
    if (isspace(c)) return EVENT_SPACE;
    if (isdigit(c)) return EVENT_DIGIT;
    if (isalpha(c)) return EVENT_ALPHA;
    return EVENT_OTHER;
}

void process_state_machine(const char *input) {
    State current_state = STATE_START;
    const char *ptr = input;
    char buffer[MAX_INPUT];
    char *buf_ptr = buffer;
    
    while (current_state != STATE_END) {
        Event evt = get_event(*ptr);
        
        switch (current_state) {
            case STATE_START:
                if (evt == EVENT_ALPHA) {
                    current_state = STATE_READING_WORD;
                    *buf_ptr = *ptr;
                    buf_ptr++;
                } else if (evt == EVENT_DIGIT) {
                    current_state = STATE_READING_NUMBER;
                    *buf_ptr = *ptr;
                    buf_ptr++;
                } else if (evt == EVENT_SPACE || evt == EVENT_OTHER) {
                    current_state = STATE_SKIPPING;
                } else if (evt == EVENT_END) {
                    current_state = STATE_END;
                }
                ptr++;
                break;
                
            case STATE_READING_WORD:
                if (evt == EVENT_ALPHA) {
                    *buf_ptr = *ptr;
                    buf_ptr++;
                    ptr++;
                } else {
                    *buf_ptr = '\0';
                    printf("Found word: %s\n", buffer);
                    buf_ptr = buffer;
                    current_state = STATE_START;
                }
                break;
                
            case STATE_READING_NUMBER:
                if (evt == EVENT_DIGIT) {
                    *buf_ptr = *ptr;
                    buf_ptr++;
                    ptr++;
                } else {
                    *buf_ptr = '\0';
                    printf("Found number: %s\n", buffer);
                    buf_ptr = buffer;
                    current_state = STATE_START;
                }
                break;
                
            case STATE_SKIPPING:
                if (evt == EVENT_SPACE || evt == EVENT_OTHER) {
                    ptr++;
                } else {
                    current_state = STATE_START;
                }
                break;
                
            case STATE_END:
                break;
        }
        
        if (buf_ptr - buffer >= MAX_INPUT - 1) {
            *buf_ptr = '\0';
            printf("Buffer overflow protection triggered\n");
            break;
        }
    }
}

int main() {
    char input[MAX_INPUT];
    
    printf("Enter text to analyze: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    process_state_machine(input);
    return 0;
}