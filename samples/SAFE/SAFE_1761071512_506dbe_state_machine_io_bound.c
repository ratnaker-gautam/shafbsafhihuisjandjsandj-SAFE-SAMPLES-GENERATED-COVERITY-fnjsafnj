//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 5

typedef enum {
    STATE_START,
    STATE_READING,
    STATE_PROCESSING,
    STATE_FINAL,
    STATE_ERROR
} State;

typedef enum {
    EVENT_CHAR,
    EVENT_SPACE,
    EVENT_EOF,
    EVENT_INVALID
} Event;

Event get_event(char c) {
    if (c == EOF) return EVENT_EOF;
    if (isspace(c)) return EVENT_SPACE;
    if (isprint(c)) return EVENT_CHAR;
    return EVENT_INVALID;
}

State transition(State current, Event event) {
    static const State transitions[MAX_STATES][4] = {
        {STATE_READING, STATE_START, STATE_FINAL, STATE_ERROR},
        {STATE_READING, STATE_PROCESSING, STATE_FINAL, STATE_ERROR},
        {STATE_READING, STATE_PROCESSING, STATE_FINAL, STATE_ERROR},
        {STATE_START, STATE_START, STATE_FINAL, STATE_ERROR},
        {STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR}
    };
    
    if (current >= MAX_STATES || event >= EVENT_INVALID) {
        return STATE_ERROR;
    }
    
    return transitions[current][event];
}

int main(void) {
    State current_state = STATE_START;
    char input[MAX_INPUT_LEN + 1];
    int word_count = 0;
    int char_count = 0;
    int in_word = 0;
    int input_len = 0;
    
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
    
    if (input_len >= MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    for (int i = 0; i <= input_len; i++) {
        char c = input[i];
        Event event = get_event(c);
        
        State next_state = transition(current_state, event);
        
        switch (current_state) {
            case STATE_START:
                if (event == EVENT_CHAR) {
                    word_count++;
                    in_word = 1;
                    char_count++;
                }
                break;
                
            case STATE_READING:
                if (event == EVENT_CHAR) {
                    char_count++;
                    if (!in_word) {
                        word_count++;
                        in_word = 1;
                    }
                } else if (event == EVENT_SPACE) {
                    in_word = 0;
                }
                break;
                
            case STATE_PROCESSING:
                if (event == EVENT_CHAR) {
                    char_count++;
                    if (!in_word) {
                        word_count++;
                        in_word = 1;
                    }
                } else if (event == EVENT_SPACE) {
                    in_word = 0;
                }
                break;
                
            case STATE_ERROR:
                fprintf(stderr, "Invalid input character\n");
                return 1;
                
            default:
                break;
        }
        
        current_state = next_state;
        
        if (current_state == STATE_FINAL) {
            break;
        }
    }
    
    if (current_state == STATE_ERROR) {
        fprintf(stderr, "Processing error\n");
        return 1;
    }
    
    printf("Words: %d\n", word_count);
    printf("Characters: %d\n", char_count);
    
    return 0;
}