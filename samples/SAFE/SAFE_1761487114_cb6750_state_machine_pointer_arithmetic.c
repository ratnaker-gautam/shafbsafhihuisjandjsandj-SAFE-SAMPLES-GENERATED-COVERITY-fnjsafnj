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
    char *ptr = input_buffer;
    int word_count = 0;
    int char_count = 0;
    State current_state = STATE_START;
    
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
    
    while (ptr < input_buffer + input_len + 1) {
        InputType input_type = classify_input(*ptr);
        
        if (input_type == INPUT_INVALID) {
            fprintf(stderr, "Invalid character encountered\n");
            return 1;
        }
        
        process_state_action(current_state, *ptr, &word_count, &char_count);
        
        current_state = transition_table[current_state][input_type];
        
        if (current_state >= STATE_COUNT) {
            fprintf(stderr, "Invalid state transition\n");
            return 1;
        }
        
        if (current_state == STATE_DONE) {
            break;
        }
        
        ptr++;
    }
    
    if (current_state == STATE_FINALIZING) {
        word_count++;
    }
    
    printf("Words: %d\n", word_count);
    printf("Characters: %d\n", char_count);
    
    return 0;
}