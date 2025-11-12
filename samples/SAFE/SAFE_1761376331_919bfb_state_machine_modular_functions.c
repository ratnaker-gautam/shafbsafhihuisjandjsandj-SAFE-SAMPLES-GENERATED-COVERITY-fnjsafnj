//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10

typedef enum {
    STATE_START,
    STATE_READING,
    STATE_PROCESSING,
    STATE_FINISHED,
    STATE_ERROR
} State;

typedef enum {
    EVENT_START,
    EVENT_DATA,
    EVENT_PROCESS,
    EVENT_COMPLETE,
    EVENT_ERROR,
    EVENT_RESET
} Event;

typedef struct {
    State current_state;
    char buffer[MAX_INPUT_LEN];
    size_t buffer_len;
    int data_count;
} StateMachine;

State handle_event(State current, Event event, StateMachine* sm);
void initialize_machine(StateMachine* sm);
int validate_input(const char* input);
Event get_next_event(void);
void process_data(StateMachine* sm);

State handle_event(State current, Event event, StateMachine* sm) {
    switch(current) {
        case STATE_START:
            if (event == EVENT_START) {
                printf("Machine started\n");
                return STATE_READING;
            }
            break;
            
        case STATE_READING:
            if (event == EVENT_DATA) {
                if (sm->buffer_len < MAX_INPUT_LEN - 1) {
                    printf("Data received: %s\n", sm->buffer);
                    return STATE_PROCESSING;
                } else {
                    return STATE_ERROR;
                }
            }
            break;
            
        case STATE_PROCESSING:
            if (event == EVENT_PROCESS) {
                process_data(sm);
                return STATE_FINISHED;
            }
            break;
            
        case STATE_FINISHED:
            if (event == EVENT_RESET) {
                initialize_machine(sm);
                return STATE_START;
            }
            break;
            
        case STATE_ERROR:
            if (event == EVENT_RESET) {
                initialize_machine(sm);
                return STATE_START;
            }
            break;
            
        default:
            break;
    }
    
    if (event == EVENT_ERROR) {
        printf("Error occurred\n");
        return STATE_ERROR;
    }
    
    return current;
}

void initialize_machine(StateMachine* sm) {
    if (sm == NULL) return;
    
    sm->current_state = STATE_START;
    memset(sm->buffer, 0, MAX_INPUT_LEN);
    sm->buffer_len = 0;
    sm->data_count = 0;
}

int validate_input(const char* input) {
    if (input == NULL) return 0;
    
    size_t len = strlen(input);
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    
    for (size_t i = 0; i < len; i++) {
        if (input[i] < 32 || input[i] > 126) return 0;
    }
    
    return 1;
}

Event get_next_event(void) {
    static int call_count = 0;
    
    if (call_count >= 10) return EVENT_COMPLETE;
    
    Event events[] = {EVENT_START, EVENT_DATA, EVENT_PROCESS, EVENT_RESET};
    Event event = events[call_count % 4];
    call_count++;
    
    return event;
}

void process_data(StateMachine* sm) {
    if (sm == NULL) return;
    
    printf("Processing data: ");
    for (size_t i = 0; i < sm->buffer_len && i < MAX_INPUT_LEN; i++) {
        if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
            printf("%c", sm->buffer[i] - 32);
        } else {
            printf("%c", sm->buffer[i]);
        }
    }
    printf("\n");
    
    sm->data_count++;
}

int main(void) {
    StateMachine sm;
    initialize_machine(&sm);
    
    char test_inputs[][MAX_INPUT_LEN] = {
        "hello",
        "world",
        "test123",
        "data"
    };
    size_t test_count = sizeof(test_inputs) / sizeof(test_inputs[0]);
    size_t test_index = 0;
    
    for (int i = 0; i < 15; i++) {
        Event event = get_next_event();
        
        if (event == EVENT_DATA) {
            if (test_index < test_count) {
                const char* input = test_inputs[test_index];
                if (validate_input(input)) {
                    size_t len = strlen(input);
                    if (len < MAX_INPUT_LEN) {
                        strncpy(sm.buffer, input, MAX_INPUT_LEN - 1);
                        sm.buffer[MAX_INPUT_LEN - 1] = '\0';
                        sm.buffer_len = len;
                        test_index++;
                    }
                }
            }
        }
        
        State new_state = handle_event(sm.current_state, event, &sm);
        sm.current_state = new_state;
        
        if (event == EVENT_COMPLETE) break;
        
        if (sm.current_state == STATE_ERROR) {
            printf("Entering error state, resetting...\n");
            event = EVENT_RESET;
            new_state = handle_event(sm.current_state, event, &sm);
            sm.current_state = new_state;
        }
    }
    
    printf("Final state: %d, Data processed: %d\n", sm.current_state, sm.data_count);
    
    return 0;
}