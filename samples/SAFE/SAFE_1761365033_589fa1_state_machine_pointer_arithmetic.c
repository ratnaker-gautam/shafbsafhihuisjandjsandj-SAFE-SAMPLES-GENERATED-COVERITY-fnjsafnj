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

typedef struct {
    State current;
    char* buffer;
    size_t position;
    size_t length;
} StateMachine;

int validate_input(const char* input, size_t len) {
    if (input == NULL || len == 0 || len >= MAX_INPUT_LEN) return 0;
    const char* ptr = input;
    while (ptr < input + len) {
        if (*ptr < 32 || *ptr > 126) return 0;
        ptr++;
    }
    return 1;
}

State process_state(StateMachine* sm, char input_char) {
    switch (sm->current) {
        case STATE_START:
            if (input_char == 'B') return STATE_READING;
            break;
        case STATE_READING:
            if (input_char == 'E') return STATE_PROCESSING;
            break;
        case STATE_PROCESSING:
            if (input_char == 'N') return STATE_FINALIZING;
            break;
        case STATE_FINALIZING:
            if (input_char == 'D') return STATE_DONE;
            break;
        default:
            break;
    }
    return sm->current;
}

void init_state_machine(StateMachine* sm, char* buffer, size_t buf_size) {
    if (sm == NULL || buffer == NULL || buf_size == 0) return;
    sm->current = STATE_START;
    sm->buffer = buffer;
    sm->position = 0;
    sm->length = buf_size - 1;
    memset(buffer, 0, buf_size);
}

int main(void) {
    char input_buffer[MAX_INPUT_LEN];
    char process_buffer[MAX_INPUT_LEN];
    StateMachine sm;
    
    init_state_machine(&sm, process_buffer, sizeof(process_buffer));
    
    printf("Enter input sequence (max %d chars): ", MAX_INPUT_LEN - 1);
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = strnlen(input_buffer, sizeof(input_buffer));
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[--input_len] = '\0';
    }
    
    if (!validate_input(input_buffer, input_len)) {
        fprintf(stderr, "Invalid input characters\n");
        return EXIT_FAILURE;
    }
    
    char* input_ptr = input_buffer;
    while (input_ptr < input_buffer + input_len && sm.position < sm.length) {
        char current_char = *input_ptr;
        State new_state = process_state(&sm, current_char);
        
        if (new_state != sm.current) {
            sm.current = new_state;
            *(sm.buffer + sm.position) = current_char;
            sm.position++;
        }
        
        input_ptr++;
        
        if (sm.current == STATE_DONE) {
            break;
        }
    }
    
    *(sm.buffer + sm.position) = '\0';
    
    printf("Final state: %d\n", sm.current);
    printf("Processed sequence: %s\n", process_buffer);
    
    if (sm.current == STATE_DONE) {
        printf("State machine completed successfully\n");
    } else {
        printf("State machine did not reach final state\n");
    }
    
    return EXIT_SUCCESS;
}