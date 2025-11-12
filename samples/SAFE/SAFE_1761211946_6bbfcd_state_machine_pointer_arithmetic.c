//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 256
#define MAX_STATES 10

typedef enum {
    STATE_START,
    STATE_READING_NUMBER,
    STATE_READING_WORD,
    STATE_END,
    STATE_ERROR
} State;

typedef enum {
    EVENT_DIGIT,
    EVENT_LETTER,
    EVENT_SPACE,
    EVENT_TERMINATOR,
    EVENT_INVALID
} Event;

Event get_event(char c) {
    if (c == '\0') return EVENT_TERMINATOR;
    if (isspace(c)) return EVENT_SPACE;
    if (isdigit(c)) return EVENT_DIGIT;
    if (isalpha(c)) return EVENT_LETTER;
    return EVENT_INVALID;
}

State transition(State current, Event evt) {
    static const State transitions[5][5] = {
        {STATE_READING_NUMBER, STATE_READING_WORD, STATE_START, STATE_END, STATE_ERROR},
        {STATE_READING_NUMBER, STATE_ERROR, STATE_START, STATE_END, STATE_ERROR},
        {STATE_ERROR, STATE_READING_WORD, STATE_START, STATE_END, STATE_ERROR},
        {STATE_END, STATE_END, STATE_END, STATE_END, STATE_END},
        {STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR}
    };
    
    if (current >= 0 && current < 5 && evt >= 0 && evt < 5) {
        return transitions[current][evt];
    }
    return STATE_ERROR;
}

void process_input(const char *input) {
    State current_state = STATE_START;
    int number_count = 0;
    int word_count = 0;
    const char *ptr = input;
    
    while (current_state != STATE_END && current_state != STATE_ERROR) {
        Event evt = get_event(*ptr);
        State next_state = transition(current_state, evt);
        
        if (current_state == STATE_READING_NUMBER && next_state == STATE_START) {
            number_count++;
        } else if (current_state == STATE_READING_WORD && next_state == STATE_START) {
            word_count++;
        }
        
        current_state = next_state;
        
        if (current_state != STATE_END && current_state != STATE_ERROR) {
            ptr++;
            if (ptr - input >= MAX_INPUT_LEN) {
                current_state = STATE_ERROR;
            }
        }
    }
    
    if (current_state == STATE_ERROR) {
        printf("Error: Invalid input format\n");
    } else {
        printf("Numbers: %d, Words: %d\n", number_count, word_count);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 0 || (len == 1 && input[0] == '\n')) {
        printf("Empty input\n");
        return 1;
    }
    
    process_input(input);
    return 0;
}