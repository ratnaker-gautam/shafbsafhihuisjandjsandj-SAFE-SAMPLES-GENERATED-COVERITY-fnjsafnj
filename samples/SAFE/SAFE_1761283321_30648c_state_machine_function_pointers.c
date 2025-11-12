//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 5

typedef enum {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_OUTPUT,
    STATE_ERROR
} State;

typedef struct {
    State current_state;
    char buffer[MAX_INPUT_LEN];
    size_t buffer_len;
    int result;
} StateMachine;

typedef State (*StateHandler)(StateMachine*);

State handle_idle(StateMachine* sm) {
    printf("Enter a number (or 'quit' to exit): ");
    if (fgets(sm->buffer, MAX_INPUT_LEN, stdin) == NULL) {
        return STATE_ERROR;
    }
    
    sm->buffer_len = strlen(sm->buffer);
    if (sm->buffer_len > 0 && sm->buffer[sm->buffer_len - 1] == '\n') {
        sm->buffer[sm->buffer_len - 1] = '\0';
        sm->buffer_len--;
    }
    
    if (strcmp(sm->buffer, "quit") == 0) {
        return STATE_IDLE;
    }
    
    return STATE_READING;
}

State handle_reading(StateMachine* sm) {
    if (sm->buffer_len == 0) {
        return STATE_ERROR;
    }
    
    for (size_t i = 0; i < sm->buffer_len; i++) {
        if (sm->buffer[i] < '0' || sm->buffer[i] > '9') {
            return STATE_ERROR;
        }
    }
    
    return STATE_PROCESSING;
}

State handle_processing(StateMachine* sm) {
    long temp = 0;
    for (size_t i = 0; i < sm->buffer_len; i++) {
        if (temp > (LONG_MAX - (sm->buffer[i] - '0')) / 10) {
            return STATE_ERROR;
        }
        temp = temp * 10 + (sm->buffer[i] - '0');
    }
    
    if (temp > 1000000) {
        return STATE_ERROR;
    }
    
    sm->result = (int)temp;
    return STATE_OUTPUT;
}

State handle_output(StateMachine* sm) {
    long square = (long)sm->result * (long)sm->result;
    printf("Processed number: %d\n", sm->result);
    printf("Square: %ld\n", square);
    return STATE_IDLE;
}

State handle_error(StateMachine* sm) {
    printf("Error: Invalid input\n");
    return STATE_IDLE;
}

int main() {
    StateMachine sm;
    sm.current_state = STATE_IDLE;
    sm.buffer_len = 0;
    sm.result = 0;
    
    StateHandler handlers[MAX_STATES] = {
        handle_idle,
        handle_reading,
        handle_processing,
        handle_output,
        handle_error
    };
    
    while (1) {
        if (sm.current_state >= MAX_STATES) {
            break;
        }
        
        State next_state = handlers[sm.current_state](&sm);
        
        if (next_state == STATE_IDLE && sm.current_state == STATE_IDLE) {
            break;
        }
        
        sm.current_state = next_state;
    }
    
    printf("Goodbye!\n");
    return 0;
}