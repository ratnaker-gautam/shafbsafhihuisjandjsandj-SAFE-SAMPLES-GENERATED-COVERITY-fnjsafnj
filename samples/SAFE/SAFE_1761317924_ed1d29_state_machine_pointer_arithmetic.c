//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define STATE_COUNT 5
#define INPUT_SIZE 256

typedef enum {
    STATE_START,
    STATE_READING,
    STATE_PROCESSING,
    STATE_WRITING,
    STATE_DONE
} state_t;

typedef enum {
    INPUT_CHAR,
    INPUT_SPACE,
    INPUT_NEWLINE,
    INPUT_EOF
} input_type_t;

static input_type_t classify_input(int c) {
    if (c == EOF) return INPUT_EOF;
    if (c == ' ') return INPUT_SPACE;
    if (c == '\n') return INPUT_NEWLINE;
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) return INPUT_CHAR;
    return INPUT_SPACE;
}

static state_t transition_table[STATE_COUNT][4] = {
    {STATE_READING, STATE_START, STATE_START, STATE_DONE},
    {STATE_READING, STATE_PROCESSING, STATE_PROCESSING, STATE_PROCESSING},
    {STATE_READING, STATE_PROCESSING, STATE_WRITING, STATE_WRITING},
    {STATE_READING, STATE_WRITING, STATE_WRITING, STATE_DONE},
    {STATE_DONE, STATE_DONE, STATE_DONE, STATE_DONE}
};

int main(void) {
    state_t current_state = STATE_START;
    char buffer[INPUT_SIZE];
    char *buffer_ptr = buffer;
    char *buffer_end = buffer + INPUT_SIZE - 1;
    int input_char;
    
    while (current_state != STATE_DONE) {
        input_char = getchar();
        input_type_t input_type = classify_input(input_char);
        
        state_t next_state = transition_table[current_state][input_type];
        
        switch (current_state) {
            case STATE_START:
                buffer_ptr = buffer;
                *buffer_ptr = '\0';
                break;
                
            case STATE_READING:
                if (input_type == INPUT_CHAR) {
                    if (buffer_ptr < buffer_end) {
                        *buffer_ptr = (char)input_char;
                        buffer_ptr++;
                        *buffer_ptr = '\0';
                    }
                }
                break;
                
            case STATE_PROCESSING:
                if (buffer_ptr > buffer) {
                    char *start = buffer;
                    char *end = buffer_ptr - 1;
                    
                    while (start < end) {
                        char temp = *start;
                        *start = *end;
                        *end = temp;
                        start++;
                        end--;
                    }
                }
                break;
                
            case STATE_WRITING:
                if (buffer_ptr > buffer) {
                    char *ptr = buffer;
                    while (ptr < buffer_ptr) {
                        putchar(*ptr);
                        ptr++;
                    }
                    putchar('\n');
                    buffer_ptr = buffer;
                    *buffer_ptr = '\0';
                }
                break;
                
            case STATE_DONE:
                break;
        }
        
        current_state = next_state;
    }
    
    if (ferror(stdin)) {
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}