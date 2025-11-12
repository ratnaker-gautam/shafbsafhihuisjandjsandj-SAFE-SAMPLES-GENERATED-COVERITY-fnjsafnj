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
                    snprintf(sm->buffer, sizeof(sm->buffer), "%s", data->message);
                    sm->current_state = STATE_READY;
                    return 0;
                }
                return -1;
            }
            break;
            
        case STATE_READY:
            if (event_type == 2) {
                if (sm->counter > 0 && sm->counter < 1000) {
                    sm->current_state = STATE_PROCESSING;
                    return 0;
                }
                return -1;
            } else if (event_type == 0) {
                sm->current_state = STATE_IDLE;
                return 0;
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
            if (event_type == 0) {
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
    printf("Commands: 0=Reset, 1=Start(value,msg), 2=Begin, 3=Process, 4=Error\n");
    printf("Initial state: %s\n", state_to_string(sm.current_state));
    
    char input[64];
    int command;
    struct EventData data;
    
    while (1) {
        printf("\nCurrent state: %s, Counter: %d\n", state_to_string(sm.current_state), sm.counter);
        printf("Enter command (0-4, -1 to exit): ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) break;
        
        if (sscanf(input, "%d", &command) != 1) {
            printf("Invalid input\n");
            continue;
        }
        
        if (command == -1) break;
        
        if (command == 1) {
            printf("Enter value (0-99): ");
            if (fgets(input, sizeof(input), stdin) == NULL) continue;
            if (sscanf(input, "%d", &data.value) != 1) {
                printf("Invalid value\n");
                continue;
            }
            
            printf("Enter message (max 31 chars): ");
            if (fgets(input, sizeof(input), stdin) == NULL) continue;
            input[strcspn(input, "\n")] = 0;
            if (strlen(input) >= sizeof(data.message)) {
                printf("Message too long\n");
                continue;
            }
            strncpy(data.message, input, sizeof(data.message) - 1);
            data.message[sizeof(data.message) - 1] = 0;
        } else {
            data.value = 0;
            data.message[0] = 0;
        }
        
        int result = state_machine_handle_event(&sm, command, &data);
        if (result == 0) {
            printf("Event processed successfully\n");
        } else {
            printf("Event failed - invalid for current state or bad data\n");
        }
    }
    
    printf("Final state: %s\n", state_to_string(sm.current_state));
    return 0;
}