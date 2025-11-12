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
    if (input == NULL || len == 0 || len > MAX_INPUT_LEN) return 0;
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
            if (input_char == 'F') return STATE_FINALIZING;
            break;
        case STATE_FINALIZING:
            if (input_char == 'X') return STATE_DONE;
            break;
        default:
            break;
    }
    return sm->current;
}

void process_buffer(StateMachine* sm) {
    if (sm->buffer == NULL || sm->length == 0) return;
    
    char* current = sm->buffer;
    char* end = sm->buffer + sm->length;
    
    while (current < end) {
        sm->current = process_state(sm, *current);
        if (sm->current == STATE_DONE) break;
        current++;
    }
}

int main(void) {
    char input_buffer[MAX_INPUT_LEN + 1];
    StateMachine sm;
    
    printf("Enter input string (max %d chars): ", MAX_INPUT_LEN);
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = strnlen(input_buffer, sizeof(input_buffer));
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    if (!validate_input(input_buffer, input_len)) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    
    sm.current = STATE_START;
    sm.buffer = input_buffer;
    sm.position = 0;
    sm.length = input_len;
    
    process_buffer(&sm);
    
    printf("Final state: ");
    switch (sm.current) {
        case STATE_START: printf("START\n"); break;
        case STATE_READING: printf("READING\n"); break;
        case STATE_PROCESSING: printf("PROCESSING\n"); break;
        case STATE_FINALIZING: printf("FINALIZING\n"); break;
        case STATE_DONE: printf("DONE\n"); break;
        default: printf("UNKNOWN\n"); break;
    }
    
    return EXIT_SUCCESS;
}