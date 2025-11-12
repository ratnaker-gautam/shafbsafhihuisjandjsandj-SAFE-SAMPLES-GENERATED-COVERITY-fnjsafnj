//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: state_machine
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
                if (data != NULL && data->value > 0) {
                    sm->counter = data->value;
                    sm->current_state = STATE_READY;
                    return 0;
                }
                return -1;
            }
            break;
            
        case STATE_READY:
            if (event_type == 2) {
                if (data != NULL && strlen(data->message) < sizeof(sm->buffer)) {
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
            }
            break;
            
        case STATE_COMPLETE:
            if (event_type == 4) {
                sm->current_state = STATE_IDLE;
                memset(sm->buffer, 0, sizeof(sm->buffer));
                return 0;
            }
            break;
            
        case STATE_ERROR:
            if (event_type == 5) {
                sm->current_state = STATE_IDLE;
                sm->counter = 0;
                memset(sm->buffer, 0, sizeof(sm->buffer));
                return 0;
            }
            break;
            
        default:
            return -1;
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
    
    struct EventData data;
    data.value = 3;
    strncpy(data.message, "Hello World", sizeof(data.message) - 1);
    data.message[sizeof(data.message) - 1] = '\0';
    
    if (state_machine_handle_event(&sm, 1, &data) == 0) {
        printf("Event 1 processed. State: %s\n", state_to_string(sm.current_state));
    } else {
        printf("Event 1 failed\n");
        return 1;
    }
    
    if (state_machine_handle_event(&sm, 2, &data) == 0) {
        printf("Event 2 processed. State: %s, Buffer: %s\n", state_to_string(sm.current_state), sm.buffer);
    } else {
        printf("Event 2 failed\n");
        return 1;
    }
    
    while (sm.current_state == STATE_PROCESSING) {
        if (state_machine_handle_event(&sm, 3, NULL) == 0) {
            printf("Event 3 processed. State: %s, Counter: %d\n", state_to_string(sm.current_state), sm.counter);
        } else {
            printf("Event 3 failed\n");
            return 1;
        }
    }
    
    if (state_machine_handle_event(&sm, 4, NULL) == 0) {
        printf("Event 4 processed. State: %s\n", state_to_string(sm.current_state));
    } else {
        printf("Event 4 failed\n");
        return 1;
    }
    
    printf("State machine demo completed successfully\n");
    return 0;
}