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
                    snprintf(sm->buffer, sizeof(sm->buffer), "Started: %s", data->message);
                    sm->current_state = STATE_READY;
                    return 0;
                }
            }
            break;
            
        case STATE_READY:
            if (event_type == 2) {
                if (sm->counter > 0 && sm->counter < 1000) {
                    sm->current_state = STATE_PROCESSING;
                    return 0;
                }
            } else if (event_type == 0) {
                sm->current_state = STATE_IDLE;
                return 0;
            }
            break;
            
        case STATE_PROCESSING:
            if (event_type == 3) {
                if (sm->counter > 0) {
                    int result;
                    if (__builtin_sadd_overflow(sm->counter, 10, &result)) {
                        sm->current_state = STATE_ERROR;
                        return -1;
                    }
                    sm->counter = result;
                    sm->current_state = STATE_COMPLETE;
                    return 0;
                }
            } else if (event_type == 0) {
                sm->current_state = STATE_IDLE;
                return 0;
            }
            break;
            
        case STATE_COMPLETE:
            if (event_type == 4) {
                sm->current_state = STATE_IDLE;
                return 0;
            }
            break;
            
        case STATE_ERROR:
            if (event_type == 0) {
                sm->current_state = STATE_IDLE;
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
    
    printf("State Machine Demo\n");
    printf("Initial state: %s\n", state_to_string(sm.current_state));
    
    struct EventData events[] = {
        {25, "System startup"},
        {2, ""},
        {3, ""},
        {4, ""},
        {150, "Invalid value"},
        {0, "Reset"}
    };
    
    for (size_t i = 0; i < sizeof(events)/sizeof(events[0]); i++) {
        printf("\nEvent %zu: type=%d, value=%d, message='%s'\n", 
               i, (int)i, events[i].value, events[i].message);
        
        int result = state_machine_handle_event(&sm, (int)i, &events[i]);
        
        printf("Result: %s, New state: %s\n", 
               result == 0 ? "Success" : "Failed", 
               state_to_string(sm.current_state));
        
        if (sm.current_state == STATE_PROCESSING || sm.current_state == STATE_COMPLETE) {
            printf("Counter: %d, Buffer: %s\n", sm.counter, sm.buffer);
        }
        
        if (i == 2 && sm.current_state == STATE_COMPLETE) {
            printf("Processing completed successfully!\n");
        }
    }
    
    return 0;
}