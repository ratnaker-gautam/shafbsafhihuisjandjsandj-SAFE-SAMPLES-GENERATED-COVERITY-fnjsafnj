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
    if (c == '\0') return EVENT_END;
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

static void process_input(const char* input) {
    parser_state_t current_state = STATE_START;
    size_t input_len = strlen(input);
    
    if (input_len >= MAX_INPUT_LEN) {
        printf("Input too long\n");
        return;
    }
    
    for (size_t i = 0; i <= input_len; i++) {
        char current_char = input[i];
        event_t event = classify_char(current_char);
        
        if (event >= 5) {
            current_state = STATE_ERROR;
            break;
        }
        
        current_state = transition_table[current_state][event];
        
        if (current_state == STATE_ERROR) {
            break;
        }
        
        if (current_state == STATE_ACCEPT && event != EVENT_END) {
            current_state = STATE_START;
        }
    }
    
    switch (current_state) {
        case STATE_ACCEPT:
            printf("Valid input: '%s'\n", input);
            break;
        case STATE_READING_DIGITS:
            printf("Invalid: incomplete number sequence\n");
            break;
        case STATE_READING_LETTERS:
            printf("Invalid: incomplete letter sequence\n");
            break;
        case STATE_ERROR:
            printf("Invalid: mixed content or invalid characters\n");
            break;
        default:
            printf("Invalid: unexpected state\n");
            break;
    }
}

int main(void) {
    char input_buffer[MAX_INPUT_LEN + 1];
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    if (strlen(input_buffer) == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    for (int i = 0; i < 1000; i++) {
        process_input(input_buffer);
    }
    
    return 0;
}