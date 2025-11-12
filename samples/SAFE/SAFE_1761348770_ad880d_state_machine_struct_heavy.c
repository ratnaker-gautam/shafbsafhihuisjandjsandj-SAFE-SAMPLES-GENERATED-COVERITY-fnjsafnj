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

enum Event {
    EVENT_START,
    EVENT_DATA_READY,
    EVENT_PROCESS,
    EVENT_SUCCESS,
    EVENT_FAILURE,
    EVENT_RESET
};

struct StateMachine {
    enum State current_state;
    uint32_t data_count;
    uint32_t max_data;
    char buffer[256];
};

struct EventData {
    enum Event event_type;
    char payload[128];
};

void state_machine_init(struct StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->data_count = 0;
    sm->max_data = 100;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

int state_machine_handle_event(struct StateMachine *sm, const struct EventData *event) {
    if (sm == NULL || event == NULL) return -1;
    
    switch (sm->current_state) {
        case STATE_IDLE:
            if (event->event_type == EVENT_START) {
                sm->current_state = STATE_READY;
                printf("Transition: IDLE -> READY\n");
                return 0;
            }
            break;
            
        case STATE_READY:
            if (event->event_type == EVENT_DATA_READY) {
                if (strlen(event->payload) < sizeof(sm->buffer)) {
                    strncpy(sm->buffer, event->payload, sizeof(sm->buffer)-1);
                    sm->buffer[sizeof(sm->buffer)-1] = '\0';
                    sm->current_state = STATE_PROCESSING;
                    printf("Transition: READY -> PROCESSING\n");
                    return 0;
                }
            }
            break;
            
        case STATE_PROCESSING:
            if (event->event_type == EVENT_SUCCESS) {
                if (sm->data_count < sm->max_data) {
                    sm->data_count++;
                    sm->current_state = STATE_COMPLETE;
                    printf("Transition: PROCESSING -> COMPLETE\n");
                    return 0;
                }
            } else if (event->event_type == EVENT_FAILURE) {
                sm->current_state = STATE_ERROR;
                printf("Transition: PROCESSING -> ERROR\n");
                return 0;
            }
            break;
            
        case STATE_COMPLETE:
            if (event->event_type == EVENT_RESET) {
                sm->current_state = STATE_IDLE;
                sm->data_count = 0;
                memset(sm->buffer, 0, sizeof(sm->buffer));
                printf("Transition: COMPLETE -> IDLE\n");
                return 0;
            }
            break;
            
        case STATE_ERROR:
            if (event->event_type == EVENT_RESET) {
                sm->current_state = STATE_IDLE;
                sm->data_count = 0;
                memset(sm->buffer, 0, sizeof(sm->buffer));
                printf("Transition: ERROR -> IDLE\n");
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
    printf("Data Count: %u/%u\n", sm->data_count, sm->max_data);
    if (strlen(sm->buffer) > 0) {
        printf("Buffer: %s\n", sm->buffer);
    }
}

int main(void) {
    struct StateMachine sm;
    struct EventData events[6];
    int i;
    
    state_machine_init(&sm);
    
    events[0].event_type = EVENT_START;
    strncpy(events[0].payload, "", sizeof(events[0].payload)-1);
    
    events[1].event_type = EVENT_DATA_READY;
    strncpy(events[1].payload, "Sample data payload", sizeof(events[1].payload)-1);
    
    events[2].event_type = EVENT_SUCCESS;
    strncpy(events[2].payload, "", sizeof(events[2].payload)-1);
    
    events[3].event_type = EVENT_RESET;
    strncpy(events[3].payload, "", sizeof(events[3].payload)-1);
    
    events[4].event_type = EVENT_DATA_READY;
    strncpy(events[4].payload, "Another data sample", sizeof(events[4].payload)-1);
    
    events[5].event_type = EVENT_FAILURE;
    strncpy(events[5].payload, "", sizeof(events[5].payload)-1);
    
    for (i = 0; i < 6; i++) {
        printf("\nProcessing Event %d:\n", i+1);
        if (state_machine_handle_event(&sm, &events[i]) == 0) {
            print_state_info(&sm);
        } else {
            printf("Event %d not handled in current state\n", i+1);
            print_state_info(&sm);
        }
    }
    
    printf("\nFinal State:\n");
    print_state_info(&sm);
    
    return 0;
}