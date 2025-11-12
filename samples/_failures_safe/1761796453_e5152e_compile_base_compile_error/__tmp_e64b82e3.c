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
                    strncpy(sm->buffer, data->message, sizeof(sm->buffer)-1);
                    sm->buffer[sizeof(sm->buffer)-1] = '\0';
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
    struct EventData data;
    int choice;
    
    state_machine_init(&sm);
    
    printf("State Machine Demo\n");
    printf("Available events:\n");
    printf("1: Initialize counter (IDLE -> READY)\n");
    printf("2: Set message (READY -> PROCESSING)\n");
    printf("3: Process step (PROCESSING -> PROCESSING/COMPLETE)\n");
    printf("4: Error (PROCESSING -> ERROR)\n");
    printf("5: Reset (COMPLETE/ERROR -> IDLE)\n");
    printf("0: Exit\n");
    
    while (1) {
        printf("\nCurrent state: %s\n", state_to_string(sm.current_state));
        printf("Counter: %d\n", sm.counter);
        printf("Buffer: %s\n", sm.buffer);
        printf("Enter event (0-5): ");
        
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input\n");
            while (getchar() != '\n');
            continue;
        }
        
        if (choice == 0) {
            break;
        }
        
        int result = -1;
        
        switch (choice) {
            case 1:
                if (sm.current_state == STATE_IDLE) {
                    printf("Enter counter value: ");
                    if (scanf("%d", &data.value) == 1 && data.value > 0) {
                        result = state_machine_handle_event(&sm, 1, &data);
                    }
                }
                break;
                
            case 2:
                if (sm.current_state == STATE_READY) {
                    printf("Enter message: ");
                    if (scanf("%31s", data.message) == 1) {
                        result = state_machine_handle_event(&sm, 2, &data);
                    }
                }
                break;
                
            case 3:
                if (sm.current_state == STATE_PROCESSING) {
                    result = state_machine_handle_event(&sm, 3, NULL);
                }
                break;
                
            case 4:
                if (sm.current_state == STATE_PROCESSING) {
                    result = state_machine_handle_event(&sm, 4, NULL);
                }
                break;
                
            case 5:
                if (sm.current_state == STATE_COMPLETE || sm.current_state == STATE_ERROR) {
                    result = state_machine_handle_event(&sm, 5, NULL);
                }
                break;
        }
        
        if (result != 0) {
            printf("Invalid event for current state\n");