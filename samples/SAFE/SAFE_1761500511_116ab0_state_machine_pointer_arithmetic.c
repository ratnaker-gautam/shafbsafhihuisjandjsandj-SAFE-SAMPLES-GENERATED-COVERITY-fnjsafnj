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
    STATE_PROCESSING,
    STATE_FINAL,
    STATE_ERROR
} State;

typedef enum {
    EVENT_SPACE,
    EVENT_ALPHA,
    EVENT_DIGIT,
    EVENT_OTHER,
    EVENT_END
} Event;

Event get_event(char c) {
    if (c == '\0') return EVENT_END;
    if (isspace(c)) return EVENT_SPACE;
    if (isalpha(c)) return EVENT_ALPHA;
    if (isdigit(c)) return EVENT_DIGIT;
    return EVENT_OTHER;
}

State transition_table[STATE_COUNT][5] = {
    {STATE_START, STATE_READING, STATE_READING, STATE_ERROR, STATE_ERROR},
    {STATE_READING, STATE_READING, STATE_PROCESSING, STATE_ERROR, STATE_FINAL},
    {STATE_PROCESSING, STATE_READING, STATE_PROCESSING, STATE_ERROR, STATE_FINAL},
    {STATE_FINAL, STATE_FINAL, STATE_FINAL, STATE_FINAL, STATE_FINAL},
    {STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR}
};

void process_word(char *start, char *end) {
    if (end > start) {
        char temp = *end;
        *end = '\0';
        printf("Word: %s\n", start);
        *end = temp;
    }
}

int main(void) {
    char input[MAX_INPUT + 1];
    char *buffer = input;
    
    printf("Enter text: ");
    if (fgets(buffer, MAX_INPUT, stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
    
    State current_state = STATE_START;
    char *word_start = NULL;
    char *current = buffer;
    
    while (*current != '\0' && current_state != STATE_ERROR && current_state != STATE_FINAL) {
        Event ev = get_event(*current);
        State next_state = transition_table[current_state][ev];
        
        switch (current_state) {
            case STATE_START:
                if (ev == EVENT_ALPHA || ev == EVENT_DIGIT) {
                    word_start = current;
                }
                break;
                
            case STATE_READING:
                if (ev == EVENT_SPACE) {
                    process_word(word_start, current);
                    word_start = NULL;
                }
                break;
                
            case STATE_PROCESSING:
                if (ev == EVENT_SPACE) {
                    process_word(word_start, current);
                    word_start = NULL;
                }
                break;
                
            default:
                break;
        }
        
        current_state = next_state;
        current++;
    }
    
    if (current_state == STATE_READING || current_state == STATE_PROCESSING) {
        process_word(word_start, current);
    } else if (current_state == STATE_ERROR) {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }
    
    return 0;
}