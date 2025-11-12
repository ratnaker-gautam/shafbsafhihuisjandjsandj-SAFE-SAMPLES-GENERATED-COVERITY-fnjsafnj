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
    EVENT_ALPHA,
    EVENT_OTHER,
    EVENT_END
} Event;

Event get_event(char c) {
    if (c == ' ' || c == '\t') return EVENT_SPACE;
    if (c >= '0' && c <= '9') return EVENT_DIGIT;
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) return EVENT_ALPHA;
    if (c == '\0') return EVENT_END;
    return EVENT_OTHER;
}

State transition_table[5][5] = {
    {STATE_START, STATE_READING_WORD, STATE_READING_NUMBER, STATE_ERROR, STATE_END},
    {STATE_START, STATE_READING_WORD, STATE_READING_NUMBER, STATE_ERROR, STATE_END},
    {STATE_START, STATE_READING_WORD, STATE_READING_NUMBER, STATE_ERROR, STATE_END},
    {STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR},
    {STATE_END, STATE_END, STATE_END, STATE_END, STATE_END}
};

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    char *ptr = input;
    int word_count = 0;
    int number_count = 0;
    State current_state = STATE_START;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char *end_ptr = ptr + strlen(ptr);
    if (end_ptr > ptr && *(end_ptr - 1) == '\n') {
        *(end_ptr - 1) = '\0';
        end_ptr--;
    }
    
    while (ptr <= end_ptr && current_state != STATE_END && current_state != STATE_ERROR) {
        Event evt = get_event(*ptr);
        State next_state = transition_table[current_state][evt];
        
        if (current_state == STATE_START && next_state == STATE_READING_WORD) {
            word_count++;
        } else if (current_state == STATE_START && next_state == STATE_READING_NUMBER) {
            number_count++;
        } else if (current_state == STATE_READING_WORD && next_state == STATE_START) {
            word_count++;
        } else if (current_state == STATE_READING_NUMBER && next_state == STATE_START) {
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