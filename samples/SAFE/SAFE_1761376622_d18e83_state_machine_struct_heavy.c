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
    int32_t value;
    char description[32];
};

struct StateMachine {
    enum State current_state;
    uint32_t transition_count;
    struct EventData last_event;
};

void state_machine_init(struct StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->transition_count = 0;
    memset(&sm->last_event, 0, sizeof(sm->last_event));
}

int state_machine_handle_event(struct StateMachine *sm, int event_type, const struct EventData *event_data) {
    if (sm == NULL || event_data == NULL) return 0;
    
    enum State next_state = sm->current_state;
    
    switch (sm->current_state) {
        case STATE_IDLE:
            if (event_type == 1 && event_data->value > 0) {
                next_state = STATE_READY;
            }
            break;
            
        case STATE_READY:
            if (event_type == 2) {
                next_state = STATE_PROCESSING;
            } else if (event_type == 0) {
                next_state = STATE_IDLE;
            }
            break;
            
        case STATE_PROCESSING:
            if (event_type == 3 && event_data->value >= 0) {
                next_state = STATE_COMPLETE;
            } else if (event_type == 4) {
                next_state = STATE_ERROR;
            }
            break;
            
        case STATE_COMPLETE:
            if (event_type == 0) {
                next_state = STATE_IDLE;
            }
            break;
            
        case STATE_ERROR:
            if (event_type == 0) {
                next_state = STATE_IDLE;
            }
            break;
            
        default:
            return 0;
    }
    
    if (next_state != sm->current_state) {
        sm->current_state = next_state;
        sm->transition_count++;
        if (sm->transition_count < sm->transition_count - 1) {
            sm->transition_count = 0;
        }
    }
    
    memcpy(&sm->last_event, event_data, sizeof(struct EventData));
    return 1;
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
        {10, "Initialize"},
        {25, "Start processing"},
        {100, "Complete task"},
        {0, "Reset"},
        {-5, "Invalid input"}
    };
    
    int event_types[] = {1, 2, 3, 0, 4};
    size_t num_events = sizeof(events) / sizeof(events[0]);
    
    printf("State Machine Simulation\n");
    printf("Initial state: %s\n", state_to_string(sm.current_state));
    
    for (size_t i = 0; i < num_events; i++) {
        if (i >= sizeof(events) / sizeof(events[0])) break;
        
        printf("\nEvent %zu: %s (value: %d)\n", 
               i + 1, events[i].description, events[i].value);
        
        int result = state_machine_handle_event(&sm, event_types[i], &events[i]);
        
        if (result) {
            printf("New state: %s\n", state_to_string(sm.current_state));
            printf("Transitions: %u\n", sm.transition_count);
        } else {
            printf("Event handling failed\n");
        }
    }
    
    printf("\nFinal state: %s\n", state_to_string(sm.current_state));
    printf("Total transitions: %u\n", sm.transition_count);
    
    return 0;
}