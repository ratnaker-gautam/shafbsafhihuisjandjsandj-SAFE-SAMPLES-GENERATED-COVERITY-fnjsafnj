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

struct Event {
    int type;
    char data[32];
};

struct StateMachine {
    enum State current_state;
    uint32_t counter;
    char buffer[256];
    size_t buffer_len;
};

void state_machine_init(struct StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->counter = 0;
    sm->buffer[0] = '\0';
    sm->buffer_len = 0;
}

int state_machine_handle_event(struct StateMachine *sm, const struct Event *event) {
    if (sm == NULL || event == NULL) return -1;
    
    switch (sm->current_state) {
        case STATE_IDLE:
            if (event->type == 1) {
                sm->current_state = STATE_READY;
                sm->counter = 0;
                return 0;
            }
            break;
            
        case STATE_READY:
            if (event->type == 2) {
                if (strlen(event->data) < sizeof(sm->buffer)) {
                    strncpy(sm->buffer, event->data, sizeof(sm->buffer) - 1);
                    sm->buffer[sizeof(sm->buffer) - 1] = '\0';
                    sm->buffer_len = strlen(sm->buffer);
                    sm->current_state = STATE_PROCESSING;
                    return 0;
                } else {
                    sm->current_state = STATE_ERROR;
                    return -1;
                }
            } else if (event->type == 0) {
                sm->current_state = STATE_IDLE;
                return 0;
            }
            break;
            
        case STATE_PROCESSING:
            if (event->type == 3) {
                if (sm->buffer_len > 0) {
                    for (size_t i = 0; i < sm->buffer_len; i++) {
                        if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
                            sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
                        }
                    }
                    sm->current_state = STATE_COMPLETE;
                    return 0;
                } else {
                    sm->current_state = STATE_ERROR;
                    return -1;
                }
            } else if (event->type == 0) {
                sm->current_state = STATE_IDLE;
                return 0;
            }
            break;
            
        case STATE_COMPLETE:
            if (event->type == 0) {
                state_machine_init(sm);
                return 0;
            }
            break;
            
        case STATE_ERROR:
            if (event->type == 0) {
                state_machine_init(sm);
                return 0;
            }
            break;
    }
    
    return -1;
}

void print_state_info(const struct StateMachine *sm) {
    if (sm == NULL) return;
    
    printf("Current state: ");
    switch (sm->current_state) {
        case STATE_IDLE: printf("IDLE"); break;
        case STATE_READY: printf("READY"); break;
        case STATE_PROCESSING: printf("PROCESSING"); break;
        case STATE_COMPLETE: printf("COMPLETE"); break;
        case STATE_ERROR: printf("ERROR"); break;
    }
    printf("\n");
    
    if (sm->current_state == STATE_COMPLETE) {
        printf("Processed data: %s\n", sm->buffer);
    }
    
    printf("Counter: %u\n", sm->counter);
    printf("---\n");
}

int main(void) {
    struct StateMachine sm;
    state_machine_init(&sm);
    
    struct Event events[] = {
        {1, ""},
        {2, "hello world"},
        {3, ""},
        {0, ""}
    };
    
    size_t num_events = sizeof(events) / sizeof(events[0]);
    
    printf("State Machine Demo\n");
    printf("==================\n\n");
    
    for (size_t i = 0; i < num_events; i++) {
        printf("Processing event type %d\n", events[i].type);
        
        int result = state_machine_handle_event(&sm, &events[i]);
        if (result != 0) {
            printf("Error handling event type %d\n", events[i].type);
        }
        
        print_state_info(&sm);
        
        if (sm.current_state == STATE_ERROR) {
            printf("Resetting from error state...\n");
            struct Event reset_event = {0, ""};
            state_machine_handle_event(&sm, &reset_event);
        }
    }
    
    printf("Demo complete.\n");
    return 0;
}