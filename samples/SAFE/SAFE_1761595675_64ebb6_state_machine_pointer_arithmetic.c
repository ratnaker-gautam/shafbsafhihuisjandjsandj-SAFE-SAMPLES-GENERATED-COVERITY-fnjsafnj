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
    EVENT_TERMINATOR,
    EVENT_INVALID
} Event;

Event get_event(char c) {
    if (c == ' ' || c == '\t') return EVENT_SPACE;
    if (c == '\0' || c == '\n') return EVENT_TERMINATOR;
    if (isdigit(c)) return EVENT_DIGIT;
    if (isalpha(c)) return EVENT_ALPHA;
    return EVENT_INVALID;
}

State transition_table[STATE_COUNT][5] = {
    {STATE_READING, STATE_NUMBER, STATE_WORD, STATE_END, STATE_START},
    {STATE_READING, STATE_NUMBER, STATE_WORD, STATE_END, STATE_START},
    {STATE_READING, STATE_NUMBER, STATE_READING, STATE_END, STATE_START},
    {STATE_READING, STATE_READING, STATE_WORD, STATE_END, STATE_START},
    {STATE_END, STATE_END, STATE_END, STATE_END, STATE_END}
};

int main(void) {
    char input[MAX_INPUT + 1];
    char *ptr = input;
    int numbers = 0;
    int words = 0;
    State current_state = STATE_START;
    
    printf("Enter text: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    char *end = input + strlen(input);
    if (end > input && *(end - 1) == '\n') {
        *(end - 1) = '\0';
        end--;
    }
    
    while (ptr < end && current_state != STATE_END) {
        Event ev = get_event(*ptr);
        State next_state = transition_table[current_state][ev];
        
        if (current_state == STATE_NUMBER && next_state != STATE_NUMBER) {
            numbers++;
        } else if (current_state == STATE_WORD && next_state != STATE_WORD) {
            words++;
        }
        
        current_state = next_state;
        ptr++;
    }
    
    if (current_state == STATE_NUMBER) numbers++;
    if (current_state == STATE_WORD) words++;
    
    printf("Numbers: %d\n", numbers);
    printf("Words: %d\n", words);
    
    return 0;
}