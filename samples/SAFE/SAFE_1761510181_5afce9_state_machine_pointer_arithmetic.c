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
    EVENT_TERMINATOR,
    EVENT_INVALID
} Event;

State transition_table[STATE_COUNT][5] = {
    {STATE_START, STATE_READING, STATE_START, STATE_ERROR, STATE_ERROR},
    {STATE_READING, STATE_READING, STATE_PROCESSING, STATE_FINISHED, STATE_ERROR},
    {STATE_PROCESSING, STATE_READING, STATE_PROCESSING, STATE_FINISHED, STATE_ERROR},
    {STATE_FINISHED, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR},
    {STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR}
};

Event get_event(char c) {
    if (c == '\0') return EVENT_TERMINATOR;
    if (c == ' ' || c == '\t' || c == '\n') return EVENT_SPACE;
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) return EVENT_CHAR;
    return EVENT_INVALID;
}

int main(void) {
    char input_buffer[MAX_INPUT_LEN + 1];
    char *input_ptr = input_buffer;
    int word_count = 0;
    State current_state = STATE_START;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    char *end_ptr = input_ptr + strlen(input_buffer);
    if (end_ptr > input_ptr + MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return EXIT_FAILURE;
    }
    
    while (input_ptr < end_ptr && *input_ptr != '\0') {
        Event current_event = get_event(*input_ptr);
        
        if (current_event == EVENT_INVALID) {
            current_state = STATE_ERROR;
            break;
        }
        
        State next_state = *(*(transition_table + current_state) + current_event);
        
        if (next_state == STATE_ERROR) {
            current_state = STATE_ERROR;
            break;
        }
        
        if (current_state == STATE_START && next_state == STATE_READING) {
            word_count++;
        }
        
        if (current_state == STATE_PROCESSING && next_state == STATE_READING) {
            word_count++;
        }
        
        current_state = next_state;
        input_ptr++;
    }
    
    if (current_state == STATE_ERROR) {
        fprintf(stderr, "Invalid input detected\n");
        return EXIT_FAILURE;
    }
    
    if (current_state != STATE_FINISHED && current_state != STATE_PROCESSING) {
        fprintf(stderr, "Unexpected end of input\n");
        return EXIT_FAILURE;
    }
    
    printf("Word count: %d\n", word_count);
    return EXIT_SUCCESS;
}