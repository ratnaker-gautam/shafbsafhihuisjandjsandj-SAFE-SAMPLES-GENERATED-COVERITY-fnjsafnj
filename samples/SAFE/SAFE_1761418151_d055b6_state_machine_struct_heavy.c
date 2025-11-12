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
                        strncpy(sm->buffer, data->message, sizeof(sm->buffer)-1);
                        sm->buffer[sizeof(sm->buffer)-1] = '\0';
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

void print_state_info(const struct StateMachine *sm) {
    if (sm == NULL) return;
    
    const char *state_names[] = {
        "IDLE", "READY", "PROCESSING", "COMPLETE", "ERROR"
    };
    
    printf("Current State: %s\n", state_names[sm->current_state]);
    printf("Counter: %d\n", sm->counter);
    printf("Buffer: %s\n", sm->buffer);
    printf("---\n");
}

int main(void) {
    struct StateMachine sm;
    struct EventData data;
    int event;
    int result;
    
    state_machine_init(&sm);
    
    printf("State Machine Demo\n");
    printf("Events: 0=Reset, 1=Start, 2=Load, 3=Process, 4=Error\n");
    printf("Enter event number (0-4, -1 to quit): ");
    
    while (scanf("%d", &event) == 1) {
        if (event == -1) break;
        
        if (event < 0 || event > 4) {
            printf("Invalid event. Must be 0-4.\n");
            printf("Enter event number (0-4, -1 to quit): ");
            continue;
        }
        
        if (event == 2) {
            printf("Enter value (0-99): ");
            if (scanf("%d", &data.value) != 1 || data.value < 0 || data.value >= 100) {
                printf("Invalid value\n");
                printf("Enter event number (0-4, -1 to quit): ");
                continue;
            }
            printf("Enter message (max 31 chars): ");
            if (scanf("%31s", data.message) != 1) {
                printf("Invalid message\n");
                printf("Enter event number (0-4, -1 to quit): ");
                continue;
            }
            result = state_machine_handle_event(&sm, event, &data);
        } else {
            result = state_machine_handle_event(&sm, event, NULL);
        }
        
        if (result == 0) {
            printf("Event handled successfully\n");
        } else {
            printf("Event failed\n");
        }
        
        print_state_info(&sm);
        printf("Enter event number (0-4, -1 to quit): ");
    }
    
    return 0;
}