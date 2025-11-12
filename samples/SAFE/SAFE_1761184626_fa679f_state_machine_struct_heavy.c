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
    if (sm == NULL) return -1;
    
    switch (sm->current_state) {
        case STATE_IDLE:
            if (event_type == 1) {
                if (data != NULL && data->value >= 0 && data->value < 100) {
                    sm->counter = data->value;
                    if (strlen(data->message) < sizeof(sm->buffer)) {
                        strncpy(sm->buffer, data->message, sizeof(sm->buffer) - 1);
                        sm->buffer[sizeof(sm->buffer) - 1] = '\0';
                    }
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
                if (sm->counter > 0 && sm->counter < 5000) {
                    sm->counter *= 2;
                    if (sm->counter < 10000) {
                        sm->current_state = STATE_COMPLETE;
                        return 0;
                    }
                }
            } else if (event_type == 4) {
                sm->current_state = STATE_ERROR;
                return 0;
            }
            break;
            
        case STATE_COMPLETE:
            if (event_type == 0) {
                state_machine_init(sm);
                return 0;
            }
            break;
            
        case STATE_ERROR:
            if (event_type == 0) {
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
    
    printf("State Machine Demo\n");
    printf("Initial state: %s\n", state_to_string(sm.current_state));
    
    struct EventData events[] = {
        {10, "Start processing"},
        {0, ""},
        {150, "Invalid value"},
        {25, "Valid data"},
        {0, ""},
        {0, ""},
        {0, ""},
        {0, ""}
    };
    
    int event_sequence[] = {1, 2, 3, 0, 1, 2, 4, 0};
    
    for (size_t i = 0; i < sizeof(event_sequence) / sizeof(event_sequence[0]); i++) {
        printf("\nEvent %zu: type %d\n", i + 1, event_sequence[i]);
        
        int result = state_machine_handle_event(&sm, event_sequence[i], &events[i]);
        
        if (result == 0) {
            printf("State transition successful\n");
        } else {
            printf("State transition failed\n");
        }
        
        printf("Current state: %s\n", state_to_string(sm.current_state));
        printf("Counter: %d\n", sm.counter);
        printf("Buffer: %s\n", sm.buffer);
        
        if (i == 3 || i == 7) {
            printf("--- Reset ---\n");
        }
    }
    
    return 0;
}