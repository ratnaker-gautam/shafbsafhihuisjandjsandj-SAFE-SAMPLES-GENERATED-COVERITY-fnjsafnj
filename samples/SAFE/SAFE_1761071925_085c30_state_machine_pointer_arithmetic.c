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
    {STATE_START, STATE_READING, STATE_NUMBER, STATE_WORD, STATE_START},
    {STATE_READING, STATE_READING, STATE_NUMBER, STATE_WORD, STATE_END},
    {STATE_READING, STATE_READING, STATE_NUMBER, STATE_READING, STATE_END},
    {STATE_READING, STATE_READING, STATE_READING, STATE_WORD, STATE_END},
    {STATE_END, STATE_END, STATE_END, STATE_END, STATE_END}
};

void process_input(const char *input) {
    State current_state = STATE_START;
    const char *ptr = input;
    int number_count = 0;
    int word_count = 0;
    
    while (current_state != STATE_END && *ptr != '\0') {
        Event event = get_event(*ptr);
        State next_state = transition_table[current_state][event];
        
        if (current_state == STATE_START && next_state == STATE_NUMBER) {
            number_count++;
        } else if (current_state == STATE_START && next_state == STATE_WORD) {
            word_count++;
        } else if (current_state == STATE_READING && next_state == STATE_NUMBER) {
            number_count++;
        } else if (current_state == STATE_READING && next_state == STATE_WORD) {
            word_count++;
        }
        
        current_state = next_state;
        ptr++;
    }
    
    printf("Numbers found: %d\n", number_count);
    printf("Words found: %d\n", word_count);
}

int main() {
    char input[MAX_INPUT + 1];
    
    printf("Enter text to analyze: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 1 && input[0] == '\n') {
        printf("Empty input\n");
        return 0;
    }
    
    process_input(input);
    return 0;
}