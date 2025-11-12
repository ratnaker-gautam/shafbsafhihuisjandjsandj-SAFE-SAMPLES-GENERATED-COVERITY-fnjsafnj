//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
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
    EVENT_OTHER,
    EVENT_EOF
} Event;

Event get_event(int c) {
    if (c == EOF) return EVENT_EOF;
    if (isspace(c)) return EVENT_SPACE;
    if (isdigit(c)) return EVENT_DIGIT;
    if (isalpha(c)) return EVENT_ALPHA;
    return EVENT_OTHER;
}

State transition_table[STATE_COUNT][5] = {
    {STATE_START, STATE_READING, STATE_NUMBER, STATE_WORD, STATE_END},
    {STATE_READING, STATE_READING, STATE_NUMBER, STATE_WORD, STATE_END},
    {STATE_READING, STATE_READING, STATE_NUMBER, STATE_READING, STATE_END},
    {STATE_READING, STATE_READING, STATE_READING, STATE_WORD, STATE_END},
    {STATE_END, STATE_END, STATE_END, STATE_END, STATE_END}
};

int main(void) {
    char input[MAX_INPUT + 1];
    char *ptr = input;
    char *end = input + MAX_INPUT;
    int numbers = 0;
    int words = 0;
    State current_state = STATE_START;
    
    printf("Enter text (max %d chars): ", MAX_INPUT);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char *input_end = input + strlen(input);
    if (input_end > end) {
        input_end = end;
    }
    
    while (ptr < input_end && current_state != STATE_END) {
        int c = *ptr;
        Event event = get_event(c);
        State next_state = transition_table[current_state][event];
        
        if (current_state == STATE_START && next_state == STATE_READING) {
            ptr++;
            continue;
        }
        
        if (current_state == STATE_READING && next_state == STATE_NUMBER) {
            numbers++;
            current_state = next_state;
        } else if (current_state == STATE_READING && next_state == STATE_WORD) {
            words++;
            current_state = next_state;
        } else if (current_state == STATE_NUMBER && next_state == STATE_READING) {
            current_state = next_state;
        } else if (current_state == STATE_WORD && next_state == STATE_READING) {
            current_state = next_state;
        } else {
            current_state = next_state;
        }
        
        ptr++;
    }
    
    printf("Numbers found: %d\n", numbers);
    printf("Words found: %d\n", words);
    
    return 0;
}