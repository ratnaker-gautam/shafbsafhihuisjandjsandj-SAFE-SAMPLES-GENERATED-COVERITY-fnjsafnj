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
    char input[MAX_INPUT];
    char *ptr = input;
    char *end = input + MAX_INPUT - 1;
    int numbers = 0;
    int words = 0;
    
    printf("Enter text (max %d chars): ", MAX_INPUT - 1);
    
    if (fgets(input, MAX_INPUT, stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    State current_state = STATE_START;
    char *token_start = NULL;
    
    while (ptr < end && *ptr != '\0') {
        Event ev = get_event(*ptr);
        
        switch (current_state) {
            case STATE_START:
                if (ev == EVENT_DIGIT) {
                    current_state = STATE_NUMBER;
                    token_start = ptr;
                } else if (ev == EVENT_ALPHA) {
                    current_state = STATE_WORD;
                    token_start = ptr;
                } else if (ev == EVENT_SPACE) {
                    current_state = STATE_READING;
                }
                break;
                
            case STATE_READING:
                if (ev == EVENT_DIGIT) {
                    current_state = STATE_NUMBER;
                    token_start = ptr;
                } else if (ev == EVENT_ALPHA) {
                    current_state = STATE_WORD;
                    token_start = ptr;
                } else if (ev == EVENT_EOF) {
                    current_state = STATE_END;
                }
                break;
                
            case STATE_NUMBER:
                if (ev == EVENT_SPACE || ev == EVENT_EOF) {
                    numbers++;
                    current_state = STATE_READING;
                    *ptr = '\0';
                } else if (ev != EVENT_DIGIT) {
                    current_state = STATE_READING;
                }
                break;
                
            case STATE_WORD:
                if (ev == EVENT_SPACE || ev == EVENT_EOF) {
                    words++;
                    current_state = STATE_READING;
                    *ptr = '\0';
                } else if (ev != EVENT_ALPHA) {
                    current_state = STATE_READING;
                }
                break;
                
            case STATE_END:
                break;
        }
        
        if (current_state == STATE_END) {
            break;
        }
        
        ptr++;
    }
    
    if (current_state == STATE_NUMBER) {
        numbers++;
    } else if (current_state == STATE_WORD) {
        words++;
    }
    
    printf("Found %d numbers and %d words\n", numbers, words);
    
    ptr = input;
    current_state = STATE_START;
    printf("Tokens found:\n");
    
    while (ptr < end && *ptr != '\0') {
        Event ev = get_event(*ptr);
        
        switch (current_state) {
            case STATE_START:
            case STATE_READING:
                if (ev == EVENT_DIGIT || ev == EVENT_ALPHA) {
                    printf("  ");
                    current_state = (ev == EVENT_DIGIT) ? STATE_NUMBER : STATE_WORD;
                    putchar(*ptr);
                }
                break;
                
            case STATE_NUMBER:
            case STATE_WORD:
                if (ev == EVENT_SPACE || ev == EVENT_EOF) {
                    putchar('\n');
                    current_state = STATE_READING;
                } else {
                    putchar(*ptr);
                }
                break;
                
            case STATE_END:
                break;
        }
        
        if (current_state == STATE_END) {
            break;
        }
        
        ptr++;
    }
    
    if (current_state == STATE_NUMBER || current_state == STATE_WORD) {
        putchar('\n');
    }
    
    return 0;
}