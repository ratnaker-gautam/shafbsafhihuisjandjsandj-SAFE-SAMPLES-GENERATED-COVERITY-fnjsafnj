//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_LEN 256
#define STATE_COUNT 5

typedef enum {
    STATE_START,
    STATE_READING,
    STATE_PROCESSING,
    STATE_FINISHED,
    STATE_ERROR
} State;

typedef enum {
    EVENT_NONE,
    EVENT_CHAR,
    EVENT_SPACE,
    EVENT_EOF,
    EVENT_INVALID
} Event;

State transition_table[STATE_COUNT][5] = {
    {STATE_START, STATE_READING, STATE_START, STATE_FINISHED, STATE_ERROR},
    {STATE_READING, STATE_READING, STATE_PROCESSING, STATE_FINISHED, STATE_ERROR},
    {STATE_PROCESSING, STATE_READING, STATE_PROCESSING, STATE_FINISHED, STATE_ERROR},
    {STATE_FINISHED, STATE_FINISHED, STATE_FINISHED, STATE_FINISHED, STATE_FINISHED},
    {STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR}
};

Event get_event(int c) {
    if (c == EOF) return EVENT_EOF;
    if (c >= 32 && c <= 126) {
        if (c == 32) return EVENT_SPACE;
        return EVENT_CHAR;
    }
    return EVENT_INVALID;
}

int main(void) {
    char input_buffer[MAX_INPUT_LEN + 1];
    char *input_ptr = input_buffer;
    char *const buffer_end = input_buffer + MAX_INPUT_LEN;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && *(input_buffer + input_len - 1) == '\n') {
        *(input_buffer + input_len - 1) = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("Empty input\n");
        return EXIT_SUCCESS;
    }
    
    State current_state = STATE_START;
    int word_count = 0;
    int char_count = 0;
    int in_word = 0;
    
    while (input_ptr < buffer_end && *input_ptr != '\0') {
        Event event = get_event(*input_ptr);
        State next_state = transition_table[current_state][event];
        
        switch (next_state) {
            case STATE_READING:
                char_count++;
                if (!in_word) {
                    in_word = 1;
                }
                break;
                
            case STATE_PROCESSING:
                if (in_word) {
                    word_count++;
                    in_word = 0;
                }
                break;
                
            case STATE_FINISHED:
                if (in_word) {
                    word_count++;
                    in_word = 0;
                }
                break;
                
            case STATE_ERROR:
                fprintf(stderr, "Invalid character encountered\n");
                return EXIT_FAILURE;
                
            default:
                break;
        }
        
        current_state = next_state;
        input_ptr++;
        
        if (current_state == STATE_FINISHED || current_state == STATE_ERROR) {
            break;
        }
    }
    
    if (current_state == STATE_READING || current_state == STATE_PROCESSING) {
        if (in_word) {
            word_count++;
        }
    }
    
    printf("Words: %d\n", word_count);
    printf("Characters: %d\n", char_count);
    
    return EXIT_SUCCESS;
}