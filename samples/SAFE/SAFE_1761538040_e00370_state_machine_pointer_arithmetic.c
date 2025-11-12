//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT 256
#define STATE_COUNT 5

typedef enum {
    STATE_START,
    STATE_READING,
    STATE_PROCESSING,
    STATE_FINAL,
    STATE_ERROR
} State;

typedef enum {
    EVENT_SPACE,
    EVENT_ALPHA,
    EVENT_DIGIT,
    EVENT_OTHER,
    EVENT_END
} Event;

Event get_event(char c) {
    if (c == ' ') return EVENT_SPACE;
    if (isalpha(c)) return EVENT_ALPHA;
    if (isdigit(c)) return EVENT_DIGIT;
    if (c == '\0') return EVENT_END;
    return EVENT_OTHER;
}

State transition_table[STATE_COUNT][5] = {
    {STATE_START, STATE_READING, STATE_ERROR, STATE_ERROR, STATE_ERROR},
    {STATE_READING, STATE_READING, STATE_PROCESSING, STATE_PROCESSING, STATE_FINAL},
    {STATE_PROCESSING, STATE_READING, STATE_PROCESSING, STATE_PROCESSING, STATE_FINAL},
    {STATE_FINAL, STATE_FINAL, STATE_FINAL, STATE_FINAL, STATE_FINAL},
    {STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR}
};

int main(void) {
    char input[MAX_INPUT + 1] = {0};
    char output[MAX_INPUT * 2] = {0};
    char *output_ptr = output;
    
    printf("Enter text (max %d chars): ", MAX_INPUT);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("Empty input\n");
        return 0;
    }
    
    State current_state = STATE_START;
    char *ptr = input;
    char *end = input + len;
    
    while (ptr <= end) {
        Event evt = get_event(*ptr);
        State next_state = transition_table[current_state][evt];
        
        switch (current_state) {
            case STATE_START:
                if (next_state == STATE_READING) {
                    *output_ptr++ = '[';
                    *output_ptr++ = *ptr;
                }
                break;
                
            case STATE_READING:
                if (next_state == STATE_PROCESSING) {
                    *output_ptr++ = ']';
                    *output_ptr++ = '[';
                    *output_ptr++ = *ptr;
                } else if (next_state == STATE_READING) {
                    *output_ptr++ = *ptr;
                }
                break;
                
            case STATE_PROCESSING:
                if (next_state == STATE_READING) {
                    *output_ptr++ = ']';
                    *output_ptr++ = '[';
                    *output_ptr++ = *ptr;
                } else if (next_state == STATE_PROCESSING) {
                    *output_ptr++ = *ptr;
                } else if (next_state == STATE_FINAL) {
                    *output_ptr++ = ']';
                }
                break;
                
            case STATE_FINAL:
                break;
                
            case STATE_ERROR:
                fprintf(stderr, "Invalid input format\n");
                return 1;
        }
        
        current_state = next_state;
        
        if (current_state == STATE_FINAL) {
            break;
        }
        
        if (current_state == STATE_ERROR) {
            fprintf(stderr, "Invalid input format\n");
            return 1;
        }
        
        ptr++;
        
        if (output_ptr >= output + sizeof(output) - 4) {
            fprintf(stderr, "Output buffer overflow\n");
            return 1;
        }
    }
    
    *output_ptr = '\0';
    printf("Processed: %s\n", output);
    
    return 0;
}