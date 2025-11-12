//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define STATE_COUNT 5

typedef enum {
    STATE_START,
    STATE_READING_DIGITS,
    STATE_READING_LETTERS,
    STATE_ERROR,
    STATE_ACCEPT
} parser_state_t;

typedef enum {
    EVENT_DIGIT,
    EVENT_LETTER,
    EVENT_SPACE,
    EVENT_OTHER,
    EVENT_END
} event_t;

static event_t classify_char(char c) {
    if (isdigit(c)) return EVENT_DIGIT;
    if (isalpha(c)) return EVENT_LETTER;
    if (isspace(c)) return EVENT_SPACE;
    return EVENT_OTHER;
}

static parser_state_t transition_table[STATE_COUNT][5] = {
    {STATE_READING_DIGITS, STATE_READING_LETTERS, STATE_START, STATE_ERROR, STATE_ACCEPT},
    {STATE_READING_DIGITS, STATE_ERROR, STATE_ACCEPT, STATE_ERROR, STATE_ACCEPT},
    {STATE_ERROR, STATE_READING_LETTERS, STATE_ACCEPT, STATE_ERROR, STATE_ACCEPT},
    {STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR},
    {STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR}
};

static void process_state_machine(const char *input) {
    parser_state_t current_state = STATE_START;
    size_t input_len = strlen(input);
    
    for (size_t i = 0; i <= input_len; i++) {
        event_t event;
        if (i == input_len) {
            event = EVENT_END;
        } else {
            event = classify_char(input[i]);
        }
        
        if (event < 0 || event > 4) {
            current_state = STATE_ERROR;
            break;
        }
        
        parser_state_t next_state = transition_table[current_state][event];
        if (next_state < 0 || next_state >= STATE_COUNT) {
            current_state = STATE_ERROR;
            break;
        }
        
        current_state = next_state;
        
        if (current_state == STATE_ERROR) {
            break;
        }
        
        if (current_state == STATE_ACCEPT && i < input_len) {
            current_state = STATE_START;
        }
    }
    
    if (current_state == STATE_ACCEPT || current_state == STATE_START) {
        printf("Input accepted: valid sequence of digits or letters\n");
    } else {
        printf("Input rejected: invalid sequence\n");
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    
    printf("Enter a string (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    } else if (len >= MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    if (len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    process_state_machine(input);
    
    return 0;
}