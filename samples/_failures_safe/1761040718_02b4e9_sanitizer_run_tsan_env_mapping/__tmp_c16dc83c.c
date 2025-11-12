//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
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
    STATE_WRITING,
    STATE_END
} State;

typedef enum {
    INPUT_CHAR,
    INPUT_SPACE,
    INPUT_NEWLINE,
    INPUT_EOF
} InputType;

State transition_table[STATE_COUNT][4] = {
    {STATE_READING, STATE_START, STATE_START, STATE_END},
    {STATE_READING, STATE_PROCESSING, STATE_PROCESSING, STATE_PROCESSING},
    {STATE_READING, STATE_PROCESSING, STATE_WRITING, STATE_WRITING},
    {STATE_READING, STATE_WRITING, STATE_WRITING, STATE_END},
    {STATE_END, STATE_END, STATE_END, STATE_END}
};

InputType classify_input(int c) {
    if (c == EOF) return INPUT_EOF;
    if (c == ' ') return INPUT_SPACE;
    if (c == '\n') return INPUT_NEWLINE;
    if (c >= 0 && c <= 127) return INPUT_CHAR;
    return INPUT_EOF;
}

void process_buffer(char *start, char *end) {
    if (start >= end) return;
    
    char *left = start;
    char *right = end - 1;
    
    while (left < right) {
        char temp = *left;
        *left = *right;
        *right = temp;
        left++;
        right--;
    }
}

int main(void) {
    State current_state = STATE_START;
    char buffer[INPUT_SIZE];
    char *buf_ptr = buffer;
    char *buf_end = buffer + INPUT_SIZE - 1;
    int input_char;
    
    while (current_state != STATE_END) {
        input_char = getchar();
        InputType input_type = classify_input(input_char);
        State next_state = transition_table[current_state][input_type];
        
        switch (current_state) {
            case STATE_START:
                buf_ptr = buffer;
                *buf_ptr = '\0';
                break;
                
            case STATE_READING:
                if (input_type == INPUT_CHAR && buf_ptr < buf_end) {
                    *buf_ptr = (char)input_char;
                    buf_ptr++;
                    *buf_ptr = '\0';
                }
                break;
                
            case STATE_PROCESSING:
                if (buf_ptr > buffer) {
                    process_buffer(buffer, buf_ptr);
                }
                break;
                
            case STATE_WRITING:
                if (buf_ptr > buffer) {
                    char *write_ptr = buffer;
                    while (write_ptr < buf_ptr) {
                        putchar(*write_ptr);
                        write_ptr++;
                    }
                    putchar('\n');
                    buf_ptr = buffer;
                    *buf_ptr = '\0';
                }
                break;
                
            case STATE_END:
                break;
        }
        
        current_state = next_state;
    }
    
    if (buf_ptr > buffer) {
        char *write_ptr = buffer;
        while (write_ptr < buf_ptr) {
            putchar(*write_ptr);
            write_ptr++;
        }
        putchar('\n');
    }
    
    return 0;
}