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

Event get_event(int c) {
    if (c == EOF) return EVENT_EOF;
    if (isspace(c)) return EVENT_SPACE;
    if (isdigit(c)) return EVENT_DIGIT;
    if (isalpha(c)) return EVENT_ALPHA;
    return EVENT_INVALID;
}

int main(void) {
    char input[MAX_INPUT + 1];
    char *ptr = input;
    char *end = input + MAX_INPUT;
    int numbers = 0;
    int words = 0;
    
    printf("Enter text (max %d chars): ", MAX_INPUT);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char *input_end = input + strlen(input);
    if (input_end > end) {
        input_end = end;
    }
    
    State current_state = STATE_START;
    
    while (ptr < input_end) {
        Event ev = get_event(*ptr);
        
        switch (current_state) {
            case STATE_START:
                if (ev == EVENT_DIGIT) {
                    current_state = STATE_NUMBER;
                    numbers++;
                } else if (ev == EVENT_ALPHA) {
                    current_state = STATE_WORD;
                    words++;
                } else if (ev == EVENT_SPACE) {
                    current_state = STATE_START;
                }
                break;
                
            case STATE_READING:
                if (ev == EVENT_SPACE) {
                    current_state = STATE_START;
                } else if (ev == EVENT_DIGIT) {
                    current_state = STATE_NUMBER;
                } else if (ev == EVENT_ALPHA) {
                    current_state = STATE_WORD;
                }
                break;
                
            case STATE_NUMBER:
                if (ev == EVENT_SPACE) {
                    current_state = STATE_START;
                } else if (ev == EVENT_ALPHA) {
                    current_state = STATE_WORD;
                    words++;
                }
                break;
                
            case STATE_WORD:
                if (ev == EVENT_SPACE) {
                    current_state = STATE_START;
                } else if (ev == EVENT_DIGIT) {
                    current_state = STATE_NUMBER;
                    numbers++;
                }
                break;
                
            case STATE_END:
                break;
        }
        
        if (*ptr == '\n' || *ptr == '\0') {
            break;
        }
        
        ptr++;
    }
    
    printf("Numbers found: %d\n", numbers);
    printf("Words found: %d\n", words);
    
    return 0;
}