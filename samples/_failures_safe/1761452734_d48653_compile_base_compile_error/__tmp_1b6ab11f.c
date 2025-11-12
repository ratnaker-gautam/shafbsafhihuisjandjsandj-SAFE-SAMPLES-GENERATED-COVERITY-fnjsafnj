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
                sm->current_state = STATE_READY;
                sm->counter = 0;
                return 0;
            }
            break;
            
        case STATE_READY:
            if (event == 2) {
                if (data != NULL && data->value >= 0 && data->value < 100) {
                    sm->current_state = STATE_PROCESSING;
                    sm->counter = data->value;
                    if (strlen(data->message) < sizeof(sm->buffer)) {
                        strncpy(sm->buffer, data->message, sizeof(sm->buffer) - 1);
                        sm->buffer[sizeof(sm->buffer) - 1] = '\0';
                    }
                    return 0;
                } else {
                    sm->current_state = STATE_ERROR;
                    return -1;
                }
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
            } else if (event == 0) {
                sm->current_state = STATE_IDLE;
                return 0;
            } else if (event == 4) {
                sm->current_state = STATE_ERROR;
                return -1;
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
    printf("Commands: 0=Reset, 1=Start, 2=Load, 3=Process, 4=Error\n");
    printf("Current state: %s\n", state_to_string(sm.current_state));
    
    char input[16];
    int running = 1;
    
    while (running) {
        printf("\nEnter command (0-4, or 9 to quit): ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        int command;
        if (sscanf(input, "%d", &command) != 1) {
            printf("Invalid input\n");
            continue;
        }
        
        if (command == 9) {
            running = 0;
            continue;
        }
        
        struct EventData data;
        if (command == 2) {
            printf("Enter value (0-99): ");
            if (fgets(input, sizeof(input), stdin) == NULL) continue;
            if (sscanf(input, "%d", &data.value) != 1) {
                printf("Invalid value\n");
                continue;
            }
            
            printf("Enter message: ");
            if (fgets(data.message, sizeof(data.message), stdin) == NULL) continue;
            size_t len = strlen(data.message);
            if (len > 0 && data.message[len - 1] == '\n') {
                data.message[len - 1] = '\0';
            }
        }
        
        int result;
        if (command == 2) {
            result = state_machine_handle_event(&sm, command, &data);
        } else {
            result = state_machine_handle_event(&sm, command, NULL);
        }
        
        printf("Command %d -> Result: %d, State: %s", command, result, state_to_string(sm.current_state));
        
        if (sm.current_state == STATE_PROCESSING) {
            printf(", Counter: %d", sm.counter);
            if (strlen(sm.buffer) > 0) {
                printf(", Buffer: %s", sm.buffer);
            }
        }
        printf("\n");
    }
    
    printf("State machine terminated