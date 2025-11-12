//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>

#define STATE_COUNT 5
#define INPUT_SIZE 128

typedef enum {
    STATE_START,
    STATE_READING,
    STATE_PROCESSING,
    STATE_FINAL,
    STATE_ERROR
} State;

typedef enum {
    INPUT_CHAR,
    INPUT_SPACE,
    INPUT_TERMINATOR,
    INPUT_INVALID
} InputType;

State transition_table[STATE_COUNT][4] = {
    {STATE_READING, STATE_START, STATE_FINAL, STATE_ERROR},
    {STATE_READING, STATE_PROCESSING, STATE_FINAL, STATE_ERROR},
    {STATE_READING, STATE_PROCESSING, STATE_FINAL, STATE_ERROR},
    {STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR},
    {STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR}
};

InputType classify_input(char c) {
    if (c == '\0') return INPUT_TERMINATOR;
    if (c == ' ') return INPUT_SPACE;
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) return INPUT_CHAR;
    return INPUT_INVALID;
}

void process_word(char* start, char* end) {
    if (start == NULL || end == NULL || end <= start) return;
    
    char temp = *(end - 1);
    *(end - 1) = *start;
    *start = temp;
}

int main(void) {
    char input_buffer[INPUT_SIZE];
    
    printf("Enter text: ");
    if (fgets(input_buffer, INPUT_SIZE, stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len >= INPUT_SIZE - 1) {
        fprintf(stderr, "Input too long\n");
        return EXIT_FAILURE;
    }
    
    State current_state = STATE_START;
    char* buffer_ptr = input_buffer;
    char* word_start = NULL;
    
    while (current_state != STATE_FINAL && current_state != STATE_ERROR) {
        InputType input = classify_input(*buffer_ptr);
        if (input < 0 || input >= 4) {
            current_state = STATE_ERROR;
            break;
        }
        State next_state = transition_table[current_state][input];
        
        switch (current_state) {
            case STATE_START:
                if (input == INPUT_CHAR) {
                    word_start = buffer_ptr;
                }
                break;
                
            case STATE_READING:
                if (input == INPUT_SPACE || input == INPUT_TERMINATOR) {
                    process_word(word_start, buffer_ptr);
                }
                break;
                
            case STATE_PROCESSING:
                if (input == INPUT_CHAR) {
                    word_start = buffer_ptr;
                }
                break;
                
            default:
                break;
        }
        
        current_state = next_state;
        if (current_state != STATE_FINAL && current_state != STATE_ERROR) {
            buffer_ptr++;
            if (buffer_ptr - input_buffer >= (ptrdiff_t)INPUT_SIZE) {
                current_state = STATE_ERROR;
            }
        }
    }
    
    if (current_state == STATE_ERROR) {
        fprintf(stderr, "Processing error\n");
        return EXIT_FAILURE;
    }
    
    printf("Processed: %s\n", input_buffer);
    return EXIT_SUCCESS;
}