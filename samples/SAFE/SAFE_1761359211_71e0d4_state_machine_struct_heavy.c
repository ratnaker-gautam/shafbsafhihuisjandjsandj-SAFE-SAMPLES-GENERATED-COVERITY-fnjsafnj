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
    
    printf("State Machine Demo\n");
    printf("Commands: 1=start, 2=load, 3=process, 4=error, 5=reset, 0=quit\n");
    
    int running = 1;
    while (running) {
        printf("\nCurrent state: %s\n", state_to_string(sm.current_state));
        printf("Enter command: ");
        
        int command;
        if (scanf("%d", &command) != 1) {
            printf("Invalid input\n");
            while (getchar() != '\n');
            continue;
        }
        
        if (command == 0) {
            running = 0;
            continue;
        }
        
        struct EventData data;
        int result = -1;
        
        switch (command) {
            case 1:
                printf("Enter initial counter value: ");
                if (scanf("%d", &data.value) == 1 && data.value > 0) {
                    result = state_machine_handle_event(&sm, command, &data);
                }
                break;
                
            case 2:
                printf("Enter message (max 31 chars): ");
                if (scanf("%31s", data.message) == 1) {
                    result = state_machine_handle_event(&sm, command, &data);
                }
                break;
                
            case 3:
            case 4:
            case 5:
                result = state_machine_handle_event(&sm, command, NULL);
                break;
                
            default:
                printf("Unknown command\n");
                break;
        }
        
        if (result == 0) {
            printf("Event handled successfully\n");
        } else {
            printf("Event failed - invalid transition or data\n");
        }
    }
    
    printf("State machine terminated\n");
    return 0;
}