//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    int processed_count;
} StateMachine;

typedef State (*StateHandler)(StateMachine*);

State handle_idle(StateMachine* sm) {
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN - 1);
    
    if (fgets(sm->buffer, MAX_INPUT_LEN, stdin) == NULL) {
        return STATE_ERROR;
    }
    
    sm->buffer_len = strlen(sm->buffer);
    if (sm->buffer_len > 0 && sm->buffer[sm->buffer_len - 1] == '\n') {
        sm->buffer[sm->buffer_len - 1] = '\0';
        sm->buffer_len--;
    }
    
    if (sm->buffer_len == 0) {
        return STATE_IDLE;
    }
    
    return STATE_READING;
}

State handle_reading(StateMachine* sm) {
    if (sm->buffer_len >= MAX_INPUT_LEN) {
        return STATE_ERROR;
    }
    
    for (size_t i = 0; i < sm->buffer_len; i++) {
        if (sm->buffer[i] < 32 || sm->buffer[i] > 126) {
            return STATE_ERROR;
        }
    }
    
    return STATE_PROCESSING;
}

State handle_processing(StateMachine* sm) {
    if (sm->buffer_len == 0) {
        return STATE_ERROR;
    }
    
    for (size_t i = 0; i < sm->buffer_len; i++) {
        if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
            sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
        }
    }
    
    sm->processed_count++;
    return STATE_OUTPUT;
}

State handle_output(StateMachine* sm) {
    printf("Processed text: %s\n", sm->buffer);
    printf("Total processed: %d\n", sm->processed_count);
    return STATE_IDLE;
}

State handle_error(StateMachine* sm) {
    printf("Error occurred. Resetting state machine.\n");
    sm->buffer[0] = '\0';
    sm->buffer_len = 0;
    return STATE_IDLE;
}

int main() {
    StateMachine sm;
    sm.current_state = STATE_IDLE;
    sm.buffer[0] = '\0';
    sm.buffer_len = 0;
    sm.processed_count = 0;
    
    StateHandler handlers[MAX_STATES] = {
        handle_idle,
        handle_reading,
        handle_processing,
        handle_output,
        handle_error
    };
    
    printf("Text processing state machine. Enter 'quit' to exit.\n");
    
    while (1) {
        if (sm.current_state >= MAX_STATES || sm.current_state < 0) {
            sm.current_state = STATE_ERROR;
        }
        
        State next_state = handlers[sm.current_state](&sm);
        
        if (next_state == STATE_IDLE && sm.buffer_len > 0) {
            if (strcmp(sm.buffer, "quit") == 0) {
                break;
            }
        }
        
        sm.current_state = next_state;
    }
    
    printf("Final processed count: %d\n", sm.processed_count);
    return 0;
}