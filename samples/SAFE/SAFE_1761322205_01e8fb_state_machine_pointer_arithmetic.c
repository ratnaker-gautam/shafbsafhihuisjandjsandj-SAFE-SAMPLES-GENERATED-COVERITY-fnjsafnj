//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 256
#define MAX_STATES 10

typedef enum {
    STATE_START,
    STATE_READING_WORD,
    STATE_READING_NUMBER,
    STATE_END,
    STATE_ERROR
} State;

typedef enum {
    EVENT_SPACE,
    EVENT_DIGIT,
    EVENT_LETTER,
    EVENT_OTHER,
    EVENT_END
} Event;

State transition(State current, Event event) {
    static State transitions[5][5] = {
        {STATE_START, STATE_READING_NUMBER, STATE_READING_WORD, STATE_ERROR, STATE_END},
        {STATE_START, STATE_ERROR, STATE_READING_WORD, STATE_ERROR, STATE_END},
        {STATE_START, STATE_READING_NUMBER, STATE_ERROR, STATE_ERROR, STATE_END},
        {STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR},
        {STATE_END, STATE_END, STATE_END, STATE_END, STATE_END}
    };
    
    if (current >= 0 && current < 5 && event >= 0 && event < 5) {
        return transitions[current][event];
    }
    return STATE_ERROR;
}

Event get_event(char c) {
    if (c == '\0') return EVENT_END;
    if (isspace((unsigned char)c)) return EVENT_SPACE;
    if (isdigit((unsigned char)c)) return EVENT_DIGIT;
    if (isalpha((unsigned char)c)) return EVENT_LETTER;
    return EVENT_OTHER;
}

void process_input(const char *input) {
    State current_state = STATE_START;
    const char *start = NULL;
    const char *ptr = input;
    
    while (current_state != STATE_END && current_state != STATE_ERROR && *ptr != '\0') {
        Event event = get_event(*ptr);
        State next_state = transition(current_state, event);
        
        switch (current_state) {
            case STATE_START:
                if (next_state == STATE_READING_WORD || next_state == STATE_READING_NUMBER) {
                    start = ptr;
                }
                break;
                
            case STATE_READING_WORD:
                if (next_state == STATE_START) {
                    size_t len = ptr - start;
                    if (len > 0 && len < MAX_INPUT_LEN) {
                        printf("Found word: ");
                        const char *p = start;
                        while (p < ptr) {
                            putchar(*p);
                            p++;
                        }
                        putchar('\n');
                    }
                }
                break;
                
            case STATE_READING_NUMBER:
                if (next_state == STATE_START) {
                    size_t len = ptr - start;
                    if (len > 0 && len < MAX_INPUT_LEN) {
                        printf("Found number: ");
                        const char *p = start;
                        while (p < ptr) {
                            putchar(*p);
                            p++;
                        }
                        putchar('\n');
                    }
                }
                break;
                
            default:
                break;
        }
        
        current_state = next_state;
        ptr++;
    }
    
    if (current_state == STATE_ERROR) {
        printf("Error: Invalid input format\n");
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN);
    
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
        return 1;
    }
    
    process_input(input);
    
    return 0;
}