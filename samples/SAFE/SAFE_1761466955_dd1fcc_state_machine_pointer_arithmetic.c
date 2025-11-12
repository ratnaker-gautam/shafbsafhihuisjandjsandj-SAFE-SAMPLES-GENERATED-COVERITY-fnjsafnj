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
    STATE_FINALIZING,
    STATE_DONE
} State;

typedef enum {
    EVENT_NONE,
    EVENT_CHAR,
    EVENT_SPACE,
    EVENT_TERMINATOR,
    EVENT_ERROR
} Event;

State transition_table[STATE_COUNT][5] = {
    {STATE_START, STATE_READING, STATE_START, STATE_DONE, STATE_START},
    {STATE_READING, STATE_READING, STATE_PROCESSING, STATE_FINALIZING, STATE_START},
    {STATE_PROCESSING, STATE_READING, STATE_PROCESSING, STATE_FINALIZING, STATE_START},
    {STATE_FINALIZING, STATE_READING, STATE_PROCESSING, STATE_DONE, STATE_START},
    {STATE_DONE, STATE_DONE, STATE_DONE, STATE_DONE, STATE_DONE}
};

Event get_event(char c) {
    if (c == '\0') return EVENT_TERMINATOR;
    if (c == ' ') return EVENT_SPACE;
    if (c >= 'a' && c <= 'z') return EVENT_CHAR;
    if (c >= 'A' && c <= 'Z') return EVENT_CHAR;
    return EVENT_ERROR;
}

int main(void) {
    char input_buffer[MAX_INPUT_LEN + 1];
    char *input_ptr = input_buffer;
    char *end_ptr = input_buffer + MAX_INPUT_LEN;
    int word_count = 0;
    State current_state = STATE_START;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    char *newline_ptr = strchr(input_buffer, '\n');
    if (newline_ptr != NULL) {
        *newline_ptr = '\0';
    }
    
    if (strlen(input_buffer) == 0) {
        printf("No input provided\n");
        return EXIT_FAILURE;
    }
    
    while (input_ptr < end_ptr && *input_ptr != '\0') {
        Event current_event = get_event(*input_ptr);
        
        if (current_event == EVENT_ERROR) {
            fprintf(stderr, "Invalid character encountered: %c\n", *input_ptr);
            return EXIT_FAILURE;
        }
        
        State next_state = transition_table[current_state][current_event];
        
        if (current_state == STATE_READING && next_state == STATE_PROCESSING) {
            word_count++;
        }
        
        current_state = next_state;
        input_ptr++;
        
        if (current_state == STATE_DONE) {
            break;
        }
    }
    
    if (current_state == STATE_FINALIZING) {
        word_count++;
    }
    
    if (current_state != STATE_DONE && current_state != STATE_FINALIZING) {
        fprintf(stderr, "Input processing incomplete\n");
        return EXIT_FAILURE;
    }
    
    printf("Word count: %d\n", word_count);
    
    return EXIT_SUCCESS;
}