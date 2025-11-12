//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT 256
#define MAX_STATES 8

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
    EVENT_EOF,
    EVENT_OTHER
} Event;

Event get_event(char c) {
    if (c == ' ' || c == '\t') return EVENT_SPACE;
    if (c == '\0' || c == '\n') return EVENT_EOF;
    if (isdigit(c)) return EVENT_DIGIT;
    if (isalpha(c)) return EVENT_LETTER;
    return EVENT_OTHER;
}

State transition_table[MAX_STATES][5] = {
    {STATE_START, STATE_READING_WORD, STATE_READING_NUMBER, STATE_END, STATE_ERROR},
    {STATE_START, STATE_READING_WORD, STATE_READING_NUMBER, STATE_END, STATE_ERROR},
    {STATE_START, STATE_READING_WORD, STATE_READING_NUMBER, STATE_END, STATE_ERROR},
    {STATE_END, STATE_END, STATE_END, STATE_END, STATE_END},
    {STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR}
};

void process_input(const char *input) {
    State current_state = STATE_START;
    const char *ptr = input;
    int word_count = 0;
    int number_count = 0;
    
    while (current_state != STATE_END && current_state != STATE_ERROR && *ptr != '\0') {
        Event event = get_event(*ptr);
        State next_state = transition_table[current_state][event];
        
        if (current_state == STATE_START && next_state == STATE_READING_WORD) {
            word_count++;
        } else if (current_state == STATE_START && next_state == STATE_READING_NUMBER) {
            number_count++;
        }
        
        current_state = next_state;
        ptr++;
    }
    
    if (current_state == STATE_ERROR) {
        printf("Invalid input format\n");
    } else {
        printf("Words: %d, Numbers: %d\n", word_count, number_count);
    }
}

int main() {
    char input[MAX_INPUT + 1];
    
    printf("Enter text: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len >= MAX_INPUT) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    process_input(input);
    return 0;
}