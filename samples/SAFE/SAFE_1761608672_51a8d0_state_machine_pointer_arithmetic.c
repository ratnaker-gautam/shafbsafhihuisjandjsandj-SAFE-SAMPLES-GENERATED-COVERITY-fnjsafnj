//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 256

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
    if (c == '\0') return EVENT_END;
    if (isspace((unsigned char)c)) return EVENT_SPACE;
    if (isdigit((unsigned char)c)) return EVENT_DIGIT;
    if (isalpha((unsigned char)c)) return EVENT_ALPHA;
    return EVENT_OTHER;
}

State transition_table[5][5] = {
    {STATE_START, STATE_READING_WORD, STATE_READING_NUMBER, STATE_ERROR, STATE_END},
    {STATE_START, STATE_READING_WORD, STATE_ERROR, STATE_ERROR, STATE_END},
    {STATE_START, STATE_ERROR, STATE_READING_NUMBER, STATE_ERROR, STATE_END},
    {STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR},
    {STATE_END, STATE_END, STATE_END, STATE_END, STATE_END}
};

int main(void) {
    char input[MAX_INPUT_LEN];
    char *ptr = input;
    int word_count = 0;
    int number_count = 0;
    State current_state = STATE_START;
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    while (*ptr != '\0' && current_state != STATE_END && current_state != STATE_ERROR) {
        Event ev = get_event(*ptr);
        if (ev < 0 || ev > 4) {
            current_state = STATE_ERROR;
            break;
        }
        if (current_state < 0 || current_state > 4) {
            current_state = STATE_ERROR;
            break;
        }
        State next_state = transition_table[current_state][ev];
        
        switch (current_state) {
            case STATE_START:
                if (next_state == STATE_READING_WORD) word_count++;
                else if (next_state == STATE_READING_NUMBER) number_count++;
                break;
            case STATE_READING_WORD:
                if (next_state == STATE_START) {
                    Event next_ev = get_event(*(ptr + 1));
                    if (next_ev == EVENT_ALPHA) word_count++;
                }
                break;
            case STATE_READING_NUMBER:
                if (next_state == STATE_START) {
                    Event next_ev = get_event(*(ptr + 1));
                    if (next_ev == EVENT_DIGIT) number_count++;
                }
                break;
            default:
                break;
        }
        
        current_state = next_state;
        ptr++;
    }
    
    if (current_state == STATE_ERROR) {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }
    
    printf("Words: %d\nNumbers: %d\n", word_count, number_count);
    return 0;
}