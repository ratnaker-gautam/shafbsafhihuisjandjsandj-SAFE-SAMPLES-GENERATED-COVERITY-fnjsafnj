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
    if (c == '\0') return EVENT_TERMINATOR;
    if (isspace(c)) return EVENT_SPACE;
    if (isdigit(c)) return EVENT_DIGIT;
    if (isalpha(c)) return EVENT_ALPHA;
    return EVENT_INVALID;
}

State transition_table[STATE_COUNT][5] = {
    {STATE_START, STATE_READING, STATE_NUMBER, STATE_WORD, STATE_END},
    {STATE_READING, STATE_READING, STATE_NUMBER, STATE_WORD, STATE_END},
    {STATE_READING, STATE_READING, STATE_NUMBER, STATE_READING, STATE_END},
    {STATE_READING, STATE_READING, STATE_READING, STATE_WORD, STATE_END},
    {STATE_END, STATE_END, STATE_END, STATE_END, STATE_END}
};

int main(void) {
    char input[MAX_INPUT + 1] = {0};
    char *ptr = input;
    int numbers = 0;
    int words = 0;
    State current_state = STATE_START;
    
    printf("Enter text: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len >= sizeof(input)) {
        return EXIT_FAILURE;
    }
    
    while (*ptr != '\0' && current_state != STATE_END) {
        Event evt = get_event(*ptr);
        if (evt == EVENT_INVALID) {
            return EXIT_FAILURE;
        }
        
        State new_state = transition_table[current_state][evt];
        
        if (current_state == STATE_START && new_state == STATE_NUMBER) {
            numbers++;
        } else if (current_state == STATE_START && new_state == STATE_WORD) {
            words++;
        } else if (current_state == STATE_READING && new_state == STATE_NUMBER) {
            numbers++;
        } else if (current_state == STATE_READING && new_state == STATE_WORD) {
            words++;
        }
        
        current_state = new_state;
        ptr++;
    }
    
    printf("Numbers found: %d\n", numbers);
    printf("Words found: %d\n", words);
    
    return EXIT_SUCCESS;
}