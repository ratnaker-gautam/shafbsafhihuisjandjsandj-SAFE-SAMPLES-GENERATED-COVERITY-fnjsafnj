//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define STATE_IDLE 0
#define STATE_READING 1
#define STATE_PROCESSING 2
#define STATE_WRITING 3
#define STATE_ERROR 4

#define MAX_INPUT_LEN 100
#define MAX_BUFFER 256

#define TRANSITION_OK 0
#define TRANSITION_ERROR -1

#define EVENT_START 0
#define EVENT_DATA 1
#define EVENT_PROCESS 2
#define EVENT_WRITE 3
#define EVENT_ERROR 4
#define EVENT_RESET 5

typedef struct {
    int current_state;
    char buffer[MAX_BUFFER];
    size_t buffer_len;
} StateMachine;

int state_transition(StateMachine *sm, int event) {
    if (sm == NULL) return TRANSITION_ERROR;
    
    int new_state = sm->current_state;
    
    switch(sm->current_state) {
        case STATE_IDLE:
            if (event == EVENT_START) new_state = STATE_READING;
            else if (event == EVENT_ERROR) new_state = STATE_ERROR;
            break;
            
        case STATE_READING:
            if (event == EVENT_DATA) new_state = STATE_READING;
            else if (event == EVENT_PROCESS) new_state = STATE_PROCESSING;
            else if (event == EVENT_ERROR) new_state = STATE_ERROR;
            break;
            
        case STATE_PROCESSING:
            if (event == EVENT_WRITE) new_state = STATE_WRITING;
            else if (event == EVENT_ERROR) new_state = STATE_ERROR;
            break;
            
        case STATE_WRITING:
            if (event == EVENT_RESET) new_state = STATE_IDLE;
            else if (event == EVENT_ERROR) new_state = STATE_ERROR;
            break;
            
        case STATE_ERROR:
            if (event == EVENT_RESET) new_state = STATE_IDLE;
            break;
            
        default:
            return TRANSITION_ERROR;
    }
    
    if (new_state != sm->current_state) {
        sm->current_state = new_state;
        return TRANSITION_OK;
    }
    
    return (event >= EVENT_START && event <= EVENT_RESET) ? TRANSITION_OK : TRANSITION_ERROR;
}

void state_machine_init(StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->buffer_len = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

int process_input(StateMachine *sm, const char *input) {
    if (sm == NULL || input == NULL) return TRANSITION_ERROR;
    
    size_t input_len = strlen(input);
    if (input_len == 0 || input_len > MAX_INPUT_LEN) return TRANSITION_ERROR;
    
    switch(sm->current_state) {
        case STATE_IDLE:
            return state_transition(sm, EVENT_START);
            
        case STATE_READING:
            if (sm->buffer_len + input_len < sizeof(sm->buffer)) {
                memcpy(sm->buffer + sm->buffer_len, input, input_len);
                sm->buffer_len += input_len;
                return state_transition(sm, EVENT_DATA);
            }
            return state_transition(sm, EVENT_ERROR);
            
        case STATE_PROCESSING:
            for (size_t i = 0; i < sm->buffer_len; i++) {
                if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
                    sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
                }
            }
            return state_transition(sm, EVENT_WRITE);
            
        case STATE_WRITING:
            printf("Processed output: %.*s\n", (int)sm->buffer_len, sm->buffer);
            state_machine_init(sm);
            return state_transition(sm, EVENT_RESET);
            
        case STATE_ERROR:
            printf("Error state detected. Resetting.\n");
            state_machine_init(sm);
            return state_transition(sm, EVENT_RESET);
            
        default:
            return TRANSITION_ERROR;
    }
}

int main(void) {
    StateMachine sm;
    char input[MAX_INPUT_LEN + 1];
    int result;
    
    state_machine_init(&sm);
    
    printf("State Machine Demo - Enter text (or 'quit' to exit):\n");
    
    while (1) {
        printf("Current state: %d > ", sm.current_state);
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
            len--;
        }
        
        if (len == 0) continue;
        
        if (strcmp(input, "quit") == 0) {
            break;
        }
        
        if (strcmp(input, "process") == 0) {
            if (sm.current_state == STATE_READING && sm.buffer_len > 0) {
                result = state_transition(&sm, EVENT_PROCESS);
            } else {
                printf("Invalid command for current state\n");
                continue;
            }
        } else if (strcmp(input, "reset") == 0) {
            result = state_transition(&sm, EVENT_RESET);
        } else {
            result = process_input(&sm, input);
        }
        
        if (result