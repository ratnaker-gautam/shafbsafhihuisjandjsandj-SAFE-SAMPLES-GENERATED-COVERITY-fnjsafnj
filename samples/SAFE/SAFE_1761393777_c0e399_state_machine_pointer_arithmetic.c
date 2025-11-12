//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define STATE_COUNT 5
#define INPUT_SIZE 128

typedef enum {
    STATE_START,
    STATE_READING,
    STATE_PROCESSING,
    STATE_FINALIZING,
    STATE_DONE
} State;

typedef enum {
    INPUT_CHAR,
    INPUT_SPACE,
    INPUT_TERMINATOR,
    INPUT_INVALID
} InputType;

State transition_table[STATE_COUNT][4] = {
    {STATE_READING, STATE_START, STATE_DONE, STATE_START},
    {STATE_READING, STATE_PROCESSING, STATE_FINALIZING, STATE_READING},
    {STATE_READING, STATE_PROCESSING, STATE_FINALIZING, STATE_PROCESSING},
    {STATE_READING, STATE_PROCESSING, STATE_DONE, STATE_FINALIZING},
    {STATE_DONE, STATE_DONE, STATE_DONE, STATE_DONE}
};

InputType classify_input(char c) {
    if (c == '\0') return INPUT_TERMINATOR;
    if (c == ' ' || c == '\t' || c == '\n') return INPUT_SPACE;
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || 
        (c >= '0' && c <= '9')) return INPUT_CHAR;
    return INPUT_INVALID;
}

void process_state_action(State current, char c, int *word_count, int *char_count) {
    switch (current) {
        case STATE_START:
            *word_count = 0;
            *char_count = 0;
            break;
        case STATE_READING:
            (*char_count)++;
            break;
        case STATE_PROCESSING:
            (*word_count)++;
            break;
        case STATE_FINALIZING:
            (*word_count)++;
            break;
        case STATE_DONE:
            break;
    }
}

int main(void) {
    char input_buffer[INPUT_SIZE];
    int word_count = 0;
    int char_count = 0;
    
    printf("Enter text (max %d chars): ", INPUT_SIZE - 1);
    
    if (fgets(input_buffer, INPUT_SIZE, stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len >= INPUT_SIZE - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    State current_state = STATE_START;
    char *ptr = input_buffer;
    
    while (current_state != STATE_DONE) {
        char current_char = *ptr;
        InputType input_type = classify_input(current_char);
        
        if (input_type >= 4) {
            input_type = INPUT_INVALID;
        }
        
        process_state_action(current_state, current_char, &word_count, &char_count);
        
        State next_state = transition_table[current_state][input_type];
        
        if (next_state >= STATE_COUNT) {
            next_state = STATE_START;
        }
        
        current_state = next_state;
        
        if (current_state != STATE_DONE) {
            ptr++;
            if (ptr >= input_buffer + INPUT_SIZE) {
                current_state = STATE_DONE;
            }
        }
    }
    
    printf("Words: %d\n", word_count);
    printf("Characters: %d\n", char_count);
    
    return 0;
}