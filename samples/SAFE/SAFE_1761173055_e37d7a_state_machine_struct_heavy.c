//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

enum State {
    STATE_IDLE,
    STATE_READY,
    STATE_PROCESSING,
    STATE_COMPLETE,
    STATE_ERROR
};

struct EventData {
    int value;
    char message[32];
};

struct StateMachine {
    enum State current_state;
    int counter;
    char buffer[64];
};

void state_machine_init(struct StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->counter = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

int state_machine_handle_event(struct StateMachine *sm, int event_type, struct EventData *data) {
    if (sm == NULL || data == NULL) return -1;
    
    switch (sm->current_state) {
        case STATE_IDLE:
            if (event_type == 1) {
                if (data->value >= 0 && data->value < 100) {
                    sm->counter = data->value;
                    sm->current_state = STATE_READY;
                    return 0;
                }
                return -1;
            }
            break;
            
        case STATE_READY:
            if (event_type == 2) {
                size_t len = strlen(data->message);
                if (len > 0 && len < sizeof(sm->buffer)) {
                    strncpy(sm->buffer, data->message, sizeof(sm->buffer) - 1);
                    sm->buffer[sizeof(sm->buffer) - 1] = '\0';
                    sm->current_state = STATE_PROCESSING;
                    return 0;
                }
                return -1;
            }
            break;
            
        case STATE_PROCESSING:
            if (event_type == 3) {
                if (sm->counter > 0) {
                    sm->counter--;
                    if (sm->counter == 0) {
                        sm->current_state = STATE_COMPLETE;
                    }
                    return 0;
                }
                return -1;
            } else if (event_type == 4) {
                sm->current_state = STATE_ERROR;
                return 0;
            }
            break;
            
        case STATE_COMPLETE:
            if (event_type == 5) {
                state_machine_init(sm);
                return 0;
            }
            break;
            
        case STATE_ERROR:
            if (event_type == 5) {
                state_machine_init(sm);
                return 0;
            }
            break;
    }
    
    return -1;
}

const char* state_to_string(enum State state) {
    switch (state) {
        case STATE_IDLE: return "IDLE";
        case STATE_READY: return "READY";
        case STATE_PROCESSING: return "PROCESSING";
        case STATE_COMPLETE: return "COMPLETE";
        case STATE_ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

int main(void) {
    struct StateMachine sm;
    state_machine_init(&sm);
    
    struct EventData events[] = {
        {10, "Start processing"},
        {0, "Invalid value"},
        {50, "Valid message"},
        {0, ""},
        {0, "Another test"}
    };
    
    int event_sequence[] = {1, 1, 2, 3, 3, 4, 5, 2, 3, 5};
    int num_events = sizeof(event_sequence) / sizeof(event_sequence[0]);
    
    printf("State Machine Simulation\n");
    printf("Initial state: %s\n", state_to_string(sm.current_state));
    
    for (int i = 0; i < num_events; i++) {
        int event_type = event_sequence[i];
        int data_index = (i < 5) ? i : 4;
        
        printf("Event %d: type=%d, value=%d, message='%s'\n", 
               i, event_type, events[data_index].value, events[data_index].message);
        
        int result = state_machine_handle_event(&sm, event_type, &events[data_index]);
        
        if (result == 0) {
            printf("State transition successful: %s\n", state_to_string(sm.current_state));
            printf("Counter: %d, Buffer: '%s'\n", sm.counter, sm.buffer);
        } else {
            printf("State transition failed: %s\n", state_to_string(sm.current_state));
        }
        
        printf("---\n");
    }
    
    return 0;
}