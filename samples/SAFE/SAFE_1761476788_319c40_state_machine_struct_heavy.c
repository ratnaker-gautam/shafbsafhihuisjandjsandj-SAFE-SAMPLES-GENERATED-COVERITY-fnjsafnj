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

int state_machine_handle_event(struct StateMachine *sm, int event, const struct EventData *data) {
    if (sm == NULL) return -1;
    
    switch (sm->current_state) {
        case STATE_IDLE:
            if (event == 1) {
                if (data != NULL && data->value >= 0 && data->value < 100) {
                    sm->counter = data->value;
                    snprintf(sm->buffer, sizeof(sm->buffer), "%s", data->message);
                    sm->current_state = STATE_READY;
                    return 0;
                }
                return -1;
            }
            break;
            
        case STATE_READY:
            if (event == 2) {
                if (sm->counter > 0 && sm->counter < 100) {
                    sm->current_state = STATE_PROCESSING;
                    return 0;
                }
                return -1;
            } else if (event == 0) {
                sm->current_state = STATE_IDLE;
                return 0;
            }
            break;
            
        case STATE_PROCESSING:
            if (event == 3) {
                if (sm->counter > 0) {
                    sm->counter--;
                    if (sm->counter == 0) {
                        sm->current_state = STATE_COMPLETE;
                    }
                    return 0;
                }
                return -1;
            } else if (event == 4) {
                sm->current_state = STATE_ERROR;
                return 0;
            }
            break;
            
        case STATE_COMPLETE:
            if (event == 0) {
                sm->current_state = STATE_IDLE;
                return 0;
            }
            break;
            
        case STATE_ERROR:
            if (event == 0) {
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
    printf("Commands: 1=Start, 2=Process, 3=Step, 4=Error, 0=Reset, 9=Quit\n");
    
    int running = 1;
    while (running) {
        printf("\nCurrent state: %s, Counter: %d\n", state_to_string(sm.current_state), sm.counter);
        printf("Enter command: ");
        
        int command;
        if (scanf("%d", &command) != 1) {
            printf("Invalid input\n");
            while (getchar() != '\n');
            continue;
        }
        
        if (command == 9) {
            running = 0;
            continue;
        }
        
        struct EventData data;
        if (command == 1) {
            printf("Enter value (0-99): ");
            if (scanf("%d", &data.value) != 1 || data.value < 0 || data.value >= 100) {
                printf("Invalid value\n");
                while (getchar() != '\n');
                continue;
            }
            printf("Enter message: ");
            if (scanf("%31s", data.message) != 1) {
                printf("Invalid message\n");
                while (getchar() != '\n');
                continue;
            }
        } else {
            data.value = 0;
            memset(data.message, 0, sizeof(data.message));
        }
        
        int result = state_machine_handle_event(&sm, command, &data);
        if (result != 0) {
            printf("Invalid transition from state %s with command %d\n", 
                   state_to_string(sm.current_state), command);
        }
    }
    
    printf("Goodbye!\n");
    return 0;
}