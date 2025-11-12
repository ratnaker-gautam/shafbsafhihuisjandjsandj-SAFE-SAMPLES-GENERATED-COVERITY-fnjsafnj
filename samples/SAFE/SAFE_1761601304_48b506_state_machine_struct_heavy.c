//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

enum State {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_WRITING,
    STATE_ERROR,
    STATE_DONE
};

enum Event {
    EVENT_START,
    EVENT_DATA_READY,
    EVENT_PROCESS_COMPLETE,
    EVENT_WRITE_COMPLETE,
    EVENT_ERROR,
    EVENT_RESET
};

struct StateMachine {
    enum State current_state;
    uint32_t data_count;
    uint32_t max_data;
    char buffer[256];
    uint32_t buffer_pos;
};

struct EventData {
    enum Event event_type;
    char data[128];
    uint32_t data_len;
};

void state_machine_init(struct StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->data_count = 0;
    sm->max_data = 100;
    sm->buffer_pos = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

int state_machine_handle_event(struct StateMachine *sm, const struct EventData *event) {
    if (sm == NULL || event == NULL) return -1;
    
    switch (sm->current_state) {
        case STATE_IDLE:
            if (event->event_type == EVENT_START) {
                sm->current_state = STATE_READING;
                sm->data_count = 0;
                sm->buffer_pos = 0;
                printf("Transition: IDLE -> READING\n");
            }
            break;
            
        case STATE_READING:
            if (event->event_type == EVENT_DATA_READY) {
                if (event->data_len > 0 && event->data_len < sizeof(sm->buffer) - sm->buffer_pos) {
                    memcpy(sm->buffer + sm->buffer_pos, event->data, event->data_len);
                    sm->buffer_pos += event->data_len;
                    sm->current_state = STATE_PROCESSING;
                    printf("Transition: READING -> PROCESSING\n");
                } else {
                    sm->current_state = STATE_ERROR;
                    printf("Transition: READING -> ERROR\n");
                }
            } else if (event->event_type == EVENT_ERROR) {
                sm->current_state = STATE_ERROR;
                printf("Transition: READING -> ERROR\n");
            }
            break;
            
        case STATE_PROCESSING:
            if (event->event_type == EVENT_PROCESS_COMPLETE) {
                if (sm->data_count < sm->max_data) {
                    sm->data_count++;
                    sm->current_state = STATE_WRITING;
                    printf("Transition: PROCESSING -> WRITING\n");
                } else {
                    sm->current_state = STATE_DONE;
                    printf("Transition: PROCESSING -> DONE\n");
                }
            } else if (event->event_type == EVENT_ERROR) {
                sm->current_state = STATE_ERROR;
                printf("Transition: PROCESSING -> ERROR\n");
            }
            break;
            
        case STATE_WRITING:
            if (event->event_type == EVENT_WRITE_COMPLETE) {
                sm->buffer_pos = 0;
                sm->current_state = STATE_READING;
                printf("Transition: WRITING -> READING\n");
            } else if (event->event_type == EVENT_ERROR) {
                sm->current_state = STATE_ERROR;
                printf("Transition: WRITING -> ERROR\n");
            }
            break;
            
        case STATE_ERROR:
            if (event->event_type == EVENT_RESET) {
                state_machine_init(sm);
                printf("Transition: ERROR -> IDLE\n");
            }
            break;
            
        case STATE_DONE:
            if (event->event_type == EVENT_RESET) {
                state_machine_init(sm);
                printf("Transition: DONE -> IDLE\n");
            }
            break;
    }
    
    return 0;
}

void simulate_data_event(struct EventData *event) {
    if (event == NULL) return;
    event->event_type = EVENT_DATA_READY;
    const char *test_data = "Sample data chunk ";
    strncpy(event->data, test_data, sizeof(event->data) - 1);
    event->data[sizeof(event->data) - 1] = '\0';
    event->data_len = strlen(event->data);
}

int main(void) {
    struct StateMachine sm;
    struct EventData event;
    
    state_machine_init(&sm);
    
    event.event_type = EVENT_START;
    state_machine_handle_event(&sm, &event);
    
    for (int i = 0; i < 5; i++) {
        simulate_data_event(&event);
        state_machine_handle_event(&sm, &event);
        
        event.event_type = EVENT_PROCESS_COMPLETE;
        state_machine_handle_event(&sm, &event);
        
        event.event_type = EVENT_WRITE_COMPLETE;
        state_machine_handle_event(&sm, &event);
    }
    
    event.event_type = EVENT_DATA_READY;
    simulate_data_event(&event);
    state_machine_handle_event(&sm, &event);
    
    event.event_type = EVENT_PROCESS_COMPLETE;
    state_machine_handle_event(&sm, &event);
    
    printf("Final state: %d, Data processed: %u\n", sm.current_state, sm.data_count);
    
    return 0;
}