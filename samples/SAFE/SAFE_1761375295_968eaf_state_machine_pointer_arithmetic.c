//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10

typedef enum {
    STATE_START,
    STATE_READING_WORD,
    STATE_READING_NUMBER,
    STATE_ERROR,
    STATE_END
} State;

typedef enum {
    EVENT_SPACE,
    EVENT_DIGIT,
    EVENT_LETTER,
    EVENT_OTHER,
    EVENT_EOF
} Event;

Event get_event(char c) {
    if (c == ' ' || c == '\t') return EVENT_SPACE;
    if (c >= '0' && c <= '9') return EVENT_DIGIT;
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) return EVENT_LETTER;
    if (c == '\0') return EVENT_EOF;
    return EVENT_OTHER;
}

State transition_table[5][5] = {
    {STATE_START, STATE_READING_WORD, STATE_READING_NUMBER, STATE_ERROR, STATE_END},
    {STATE_START, STATE_READING_WORD, STATE_ERROR, STATE_ERROR, STATE_END},
    {STATE_START, STATE_ERROR, STATE_READING_NUMBER, STATE_ERROR, STATE_END},
    {STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_END},
    {STATE_END, STATE_END, STATE_END, STATE_END, STATE_END}
};

void process_input(const char *input) {
    State current_state = STATE_START;
    const char *ptr = input;
    char buffer[MAX_INPUT_LEN];
    char *buf_ptr = buffer;
    
    while (current_state != STATE_END && current_state != STATE_ERROR) {
        Event ev = get_event(*ptr);
        State next_state = transition_table[current_state][ev];
        
        switch (current_state) {
            case STATE_START:
                if (next_state == STATE_READING_WORD || next_state == STATE_READING_NUMBER) {
                    buf_ptr = buffer;
                    *buf_ptr = *ptr;
                    buf_ptr++;
                }
                break;
                
            case STATE_READING_WORD:
                if (next_state == STATE_READING_WORD) {
                    if ((size_t)(buf_ptr - buffer) < MAX_INPUT_LEN - 1) {
                        *buf_ptr = *ptr;
                        buf_ptr++;
                    }
                } else if (next_state == STATE_START) {
                    *buf_ptr = '\0';
                    printf("Word: %s\n", buffer);
                }
                break;
                
            case STATE_READING_NUMBER:
                if (next_state == STATE_READING_NUMBER) {
                    if ((size_t)(buf_ptr - buffer) < MAX_INPUT_LEN - 1) {
                        *buf_ptr = *ptr;
                        buf_ptr++;
                    }
                } else if (next_state == STATE_START) {
                    *buf_ptr = '\0';
                    printf("Number: %s\n", buffer);
                }
                break;
                
            case STATE_ERROR:
                printf("Invalid input sequence\n");
                return;
                
            default:
                break;
        }
        
        current_state = next_state;
        if (*ptr != '\0' && ev != EVENT_EOF) {
            ptr++;
        }
    }
    
    if (current_state == STATE_ERROR) {
        printf("Invalid input sequence\n");
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 0 || (len == 1 && input[0] == '\0')) {
        printf("Empty input\n");
        return 0;
    }
    
    process_input(input);
    return 0;
}