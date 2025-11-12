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
    EVENT_EOF,
    EVENT_INVALID
} Event;

Event get_event(char c) {
    if (c == ' ' || c == '\t') return EVENT_SPACE;
    if (c == '\0' || c == '\n') return EVENT_EOF;
    if (isdigit(c)) return EVENT_DIGIT;
    if (isalpha(c)) return EVENT_ALPHA;
    return EVENT_INVALID;
}

State transition_table[STATE_COUNT][5] = {
    {STATE_READING, STATE_NUMBER, STATE_WORD, STATE_END, STATE_READING},
    {STATE_READING, STATE_NUMBER, STATE_WORD, STATE_END, STATE_READING},
    {STATE_READING, STATE_NUMBER, STATE_READING, STATE_END, STATE_READING},
    {STATE_READING, STATE_READING, STATE_WORD, STATE_END, STATE_READING},
    {STATE_END, STATE_END, STATE_END, STATE_END, STATE_END}
};

void process_input(const char *input) {
    State current_state = STATE_START;
    const char *ptr = input;
    char buffer[MAX_INPUT];
    char *buf_ptr = buffer;
    
    while (current_state != STATE_END) {
        Event event = get_event(*ptr);
        State next_state = transition_table[current_state][event];
        
        switch (current_state) {
            case STATE_START:
                if (next_state == STATE_NUMBER || next_state == STATE_WORD) {
                    *buf_ptr = *ptr;
                    buf_ptr++;
                }
                break;
                
            case STATE_READING:
                if (next_state == STATE_NUMBER || next_state == STATE_WORD) {
                    *buf_ptr = *ptr;
                    buf_ptr++;
                }
                break;
                
            case STATE_NUMBER:
                if (next_state == STATE_READING) {
                    *buf_ptr = '\0';
                    printf("Found number: %s\n", buffer);
                    buf_ptr = buffer;
                    *buf_ptr = '\0';
                } else if (next_state == STATE_NUMBER) {
                    *buf_ptr = *ptr;
                    buf_ptr++;
                }
                break;
                
            case STATE_WORD:
                if (next_state == STATE_READING) {
                    *buf_ptr = '\0';
                    printf("Found word: %s\n", buffer);
                    buf_ptr = buffer;
                    *buf_ptr = '\0';
                } else if (next_state == STATE_WORD) {
                    *buf_ptr = *ptr;
                    buf_ptr++;
                }
                break;
                
            case STATE_END:
                break;
        }
        
        current_state = next_state;
        
        if (event != EVENT_EOF) {
            ptr++;
            if ((size_t)(ptr - input) >= MAX_INPUT) {
                break;
            }
        }
    }
    
    if (buf_ptr != buffer && current_state != STATE_START) {
        *buf_ptr = '\0';
        if (isdigit(buffer[0])) {
            printf("Found number: %s\n", buffer);
        } else if (isalpha(buffer[0])) {
            printf("Found word: %s\n", buffer);
        }
    }
}

int main() {
    char input[MAX_INPUT];
    
    printf("Enter text to analyze (max %d characters): ", MAX_INPUT - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 0 || (len == 1 && input[0] == '\0')) {
        printf("No input provided\n");
        return 0;
    }
    
    process_input(input);
    return 0;
}