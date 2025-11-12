//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10

typedef enum {
    STATE_START,
    STATE_READING_WORD,
    STATE_READING_NUMBER,
    STATE_ERROR,
    STATE_END
} State;

typedef enum {
    EVENT_SPACE,
    EVENT_DIGIT,
    EVENT_LETTER,
    EVENT_OTHER,
    EVENT_END
} Event;

Event get_event(char c) {
    if (c == '\0') return EVENT_END;
    if (isspace(c)) return EVENT_SPACE;
    if (isdigit(c)) return EVENT_DIGIT;
    if (isalpha(c)) return EVENT_LETTER;
    return EVENT_OTHER;
}

State transition_table[5][5] = {
    {STATE_START, STATE_READING_WORD, STATE_READING_NUMBER, STATE_ERROR, STATE_END},
    {STATE_START, STATE_READING_WORD, STATE_ERROR, STATE_ERROR, STATE_END},
    {STATE_START, STATE_ERROR, STATE_READING_NUMBER, STATE_ERROR, STATE_END},
    {STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_END},
    {STATE_END, STATE_END, STATE_END, STATE_END, STATE_END}
};

void process_input(const char *input) {
    if (input == NULL) return;
    
    State current_state = STATE_START;
    const char *ptr = input;
    int word_count = 0;
    int number_count = 0;
    
    while (current_state != STATE_END && current_state != STATE_ERROR) {
        Event evt = get_event(*ptr);
        State next_state = transition_table[current_state][evt];
        
        if (next_state == STATE_READING_WORD && current_state != STATE_READING_WORD) {
            word_count++;
        } else if (next_state == STATE_READING_NUMBER && current_state != STATE_READING_NUMBER) {
            number_count++;
        }
        
        current_state = next_state;
        
        if (current_state != STATE_END) {
            ptr++;
            if (ptr - input >= MAX_INPUT_LEN) {
                current_state = STATE_ERROR;
                break;
            }
        }
    }
    
    if (current_state == STATE_ERROR) {
        printf("Error: Invalid input format\n");
    } else {
        printf("Words: %d, Numbers: %d\n", word_count, number_count);
    }
}

int main() {
    char input[MAX_INPUT_LEN + 1] = {0};
    
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
        printf("No input provided\n");
        return 1;
    }
    
    process_input(input);
    return 0;
}