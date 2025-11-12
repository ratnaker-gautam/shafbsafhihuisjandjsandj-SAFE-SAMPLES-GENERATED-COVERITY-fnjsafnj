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
    STATE_PROCESSING,
    STATE_FINAL,
    STATE_ERROR
} State;

typedef enum {
    EVENT_SPACE,
    EVENT_ALPHA,
    EVENT_DIGIT,
    EVENT_OTHER,
    EVENT_END
} Event;

Event get_event(char c) {
    if (c == '\0') return EVENT_END;
    if (isspace(c)) return EVENT_SPACE;
    if (isalpha(c)) return EVENT_ALPHA;
    if (isdigit(c)) return EVENT_DIGIT;
    return EVENT_OTHER;
}

State transition_table[STATE_COUNT][5] = {
    {STATE_START, STATE_READING, STATE_READING, STATE_ERROR, STATE_ERROR},
    {STATE_READING, STATE_READING, STATE_PROCESSING, STATE_ERROR, STATE_FINAL},
    {STATE_PROCESSING, STATE_READING, STATE_PROCESSING, STATE_ERROR, STATE_FINAL},
    {STATE_FINAL, STATE_FINAL, STATE_FINAL, STATE_FINAL, STATE_FINAL},
    {STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR}
};

void process_word(char *start, char *end) {
    if (end - start <= 0) return;
    
    char *p = start;
    while (p < end) {
        if (isalpha(*p)) {
            *p = toupper(*p);
        }
        p++;
    }
    
    char temp = *end;
    *end = '\0';
    printf("Processed: %s\n", start);
    *end = temp;
}

int main(void) {
    char input[MAX_INPUT + 1];
    
    printf("Enter text (max %d chars): ", MAX_INPUT);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("No input provided\n");
        return 0;
    }
    
    State current_state = STATE_START;
    char *input_ptr = input;
    char *word_start = NULL;
    
    while (current_state != STATE_FINAL && current_state != STATE_ERROR) {
        Event evt = get_event(*input_ptr);
        State next_state = transition_table[current_state][evt];
        
        switch (current_state) {
            case STATE_START:
                if (evt == EVENT_ALPHA || evt == EVENT_DIGIT) {
                    word_start = input_ptr;
                }
                break;
                
            case STATE_READING:
                if (evt == EVENT_SPACE || evt == EVENT_END) {
                    process_word(word_start, input_ptr);
                    word_start = NULL;
                }
                break;
                
            case STATE_PROCESSING:
                if (evt == EVENT_SPACE || evt == EVENT_END) {
                    process_word(word_start, input_ptr);
                    word_start = NULL;
                } else if (evt == EVENT_ALPHA || evt == EVENT_DIGIT) {
                    word_start = input_ptr;
                }
                break;
                
            default:
                break;
        }
        
        current_state = next_state;
        if (evt != EVENT_END) {
            input_ptr++;
        }
        
        if (input_ptr - input > (long)len) {
            current_state = STATE_ERROR;
            break;
        }
    }
    
    if (current_state == STATE_ERROR) {
        fprintf(stderr, "Error processing input\n");
        return 1;
    }
    
    printf("Processing complete\n");
    return 0;
}