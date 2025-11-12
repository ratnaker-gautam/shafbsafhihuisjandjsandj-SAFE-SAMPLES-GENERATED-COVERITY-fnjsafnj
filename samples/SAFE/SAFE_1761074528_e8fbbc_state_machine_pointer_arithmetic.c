//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_LEN 100
#define STATE_COUNT 5

typedef enum {
    STATE_START,
    STATE_READING,
    STATE_PROCESSING,
    STATE_OUTPUT,
    STATE_END
} State;

typedef enum {
    EVENT_NONE,
    EVENT_INPUT_RECEIVED,
    EVENT_PROCESS_COMPLETE,
    EVENT_OUTPUT_DONE,
    EVENT_ERROR
} Event;

State transition_table[STATE_COUNT][5] = {
    {STATE_START, STATE_READING, STATE_START, STATE_START, STATE_END},
    {STATE_READING, STATE_READING, STATE_PROCESSING, STATE_READING, STATE_END},
    {STATE_PROCESSING, STATE_PROCESSING, STATE_PROCESSING, STATE_OUTPUT, STATE_END},
    {STATE_OUTPUT, STATE_OUTPUT, STATE_OUTPUT, STATE_OUTPUT, STATE_END},
    {STATE_END, STATE_END, STATE_END, STATE_END, STATE_END}
};

Event get_next_event(const char *input, size_t *pos) {
    if (input == NULL || pos == NULL) return EVENT_ERROR;
    if (*pos >= strlen(input)) return EVENT_OUTPUT_DONE;
    
    char current = *(input + *pos);
    if (current == '\0') return EVENT_OUTPUT_DONE;
    
    if (current >= '0' && current <= '9') {
        (*pos)++;
        return EVENT_INPUT_RECEIVED;
    } else if (current == ' ') {
        (*pos)++;
        return EVENT_PROCESS_COMPLETE;
    } else {
        (*pos)++;
        return EVENT_ERROR;
    }
}

void process_input(const char *input, size_t pos, int *sum) {
    if (input == NULL || sum == NULL) return;
    
    char buffer[32] = {0};
    char *buf_ptr = buffer;
    const char *input_ptr = input + pos;
    
    while (*input_ptr >= '0' && *input_ptr <= '9') {
        if ((size_t)(buf_ptr - buffer) < sizeof(buffer) - 1) {
            *buf_ptr = *input_ptr;
            buf_ptr++;
        }
        input_ptr++;
    }
    
    if (buffer[0] != '\0') {
        long value = strtol(buffer, NULL, 10);
        if (value >= 0 && value <= 1000) {
            if (*sum + value <= 10000) {
                *sum += (int)value;
            }
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1] = {0};
    State current_state = STATE_START;
    int sum = 0;
    size_t position = 0;
    
    printf("Enter numbers separated by spaces (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return EXIT_FAILURE;
    }
    
    char *newline = strchr(input, '\n');
    if (newline != NULL) {
        *newline = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("No input provided.\n");
        return EXIT_FAILURE;
    }
    
    while (current_state != STATE_END) {
        Event event = get_next_event(input, &position);
        
        if (event == EVENT_ERROR) {
            printf("Invalid input format.\n");
            current_state = STATE_END;
            continue;
        }
        
        State next_state = *(*(transition_table + (int)current_state) + (int)event);
        
        switch (current_state) {
            case STATE_START:
                if (next_state == STATE_READING) {
                    printf("Starting to process input...\n");
                }
                break;
                
            case STATE_READING:
                if (next_state == STATE_PROCESSING) {
                    process_input(input, position - 1, &sum);
                }
                break;
                
            case STATE_PROCESSING:
                if (next_state == STATE_OUTPUT) {
                    printf("Processing complete.\n");
                }
                break;
                
            case STATE_OUTPUT:
                if (next_state == STATE_END) {
                    printf("Final sum: %d\n", sum);
                }
                break;
                
            case STATE_END:
                break;
        }
        
        current_state = next_state;
    }
    
    return EXIT_SUCCESS;
}