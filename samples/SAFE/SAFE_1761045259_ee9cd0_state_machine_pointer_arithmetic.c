//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define STATE_COUNT 5
#define MAX_INPUT_LEN 100

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

static InputType classify_input(char c) {
    if (c == '\0') return INPUT_TERMINATOR;
    if (c == ' ') return INPUT_SPACE;
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) return INPUT_CHAR;
    return INPUT_INVALID;
}

static State transition_table[STATE_COUNT][4] = {
    {STATE_READING, STATE_START, STATE_FINAL, STATE_ERROR},
    {STATE_READING, STATE_PROCESSING, STATE_FINAL, STATE_ERROR},
    {STATE_READING, STATE_PROCESSING, STATE_FINAL, STATE_ERROR},
    {STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR},
    {STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR}
};

static void process_word(char *start, char *end) {
    if (end > start) {
        char temp = *end;
        *end = '\0';
        printf("Word: %s\n", start);
        *end = temp;
    }
}

int main(void) {
    char input_buffer[MAX_INPUT_LEN + 1];
    char *buffer_ptr = input_buffer;
    char *word_start = NULL;
    State current_state = STATE_START;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return EXIT_FAILURE;
    }
    
    char *newline_ptr = strchr(input_buffer, '\n');
    if (newline_ptr != NULL) {
        *newline_ptr = '\0';
    }
    
    while (*buffer_ptr != '\0' && current_state != STATE_ERROR && current_state != STATE_FINAL) {
        InputType input = classify_input(*buffer_ptr);
        if (input == INPUT_INVALID) {
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
                    word_start = NULL;
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
        buffer_ptr++;
    }
    
    if (current_state == STATE_ERROR) {
        fprintf(stderr, "Invalid input character encountered\n");
        return EXIT_FAILURE;
    }
    
    if (current_state == STATE_READING && word_start != NULL) {
        process_word(word_start, buffer_ptr);
    }
    
    if (current_state != STATE_FINAL && current_state != STATE_ERROR) {
        printf("Processing completed\n");
    }
    
    return EXIT_SUCCESS;
}