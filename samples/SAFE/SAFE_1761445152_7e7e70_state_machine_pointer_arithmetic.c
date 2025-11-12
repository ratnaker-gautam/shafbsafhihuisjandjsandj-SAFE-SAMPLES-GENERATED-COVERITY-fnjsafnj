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
    static State transitions[][5] = {
        {STATE_START, STATE_READING_WORD, STATE_READING_NUMBER, STATE_ERROR, STATE_END},
        {STATE_START, STATE_READING_WORD, STATE_ERROR, STATE_ERROR, STATE_END},
        {STATE_START, STATE_ERROR, STATE_READING_NUMBER, STATE_ERROR, STATE_END},
        {STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_END},
        {STATE_END, STATE_END, STATE_END, STATE_END, STATE_END}
    };
    
    if (current >= STATE_START && current <= STATE_ERROR && 
        event >= EVENT_SPACE && event <= EVENT_END) {
        return transitions[current][event];
    }
    return STATE_ERROR;
}

Event get_event(char c) {
    if (c == ' ' || c == '\t') return EVENT_SPACE;
    if (c == '\0') return EVENT_END;
    if (isdigit(c)) return EVENT_DIGIT;
    if (isalpha(c)) return EVENT_LETTER;
    return EVENT_OTHER;
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    char *ptr = input;
    int word_count = 0;
    int number_count = 0;
    State current_state = STATE_START;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 0 || (len == 1 && input[0] == '\n')) {
        printf("No input provided\n");
        return 0;
    }
    
    while (*ptr != '\0' && current_state != STATE_END && current_state != STATE_ERROR) {
        Event event = get_event(*ptr);
        State next_state = transition(current_state, event);
        
        if (next_state == STATE_READING_WORD && current_state != STATE_READING_WORD) {
            word_count++;
        } else if (next_state == STATE_READING_NUMBER && current_state != STATE_READING_NUMBER) {
            number_count++;
        }
        
        current_state = next_state;
        ptr++;
    }
    
    if (current_state == STATE_ERROR) {
        printf("Invalid input format\n");
        return 1;
    }
    
    printf("Words: %d\n", word_count);
    printf("Numbers: %d\n", number_count);
    
    return 0;
}