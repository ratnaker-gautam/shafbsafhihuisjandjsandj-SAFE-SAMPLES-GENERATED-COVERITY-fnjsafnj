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
    STATE_COUNT
};

enum Event {
    EVENT_START,
    EVENT_DATA_READY,
    EVENT_PROCESS_COMPLETE,
    EVENT_WRITE_COMPLETE,
    EVENT_ERROR,
    EVENT_RESET,
    EVENT_COUNT
};

struct StateMachine {
    enum State current_state;
    uint32_t data_count;
    uint32_t max_data;
    char buffer[256];
    uint32_t buffer_pos;
};

struct Transition {
    enum State current;
    enum Event event;
    enum State next;
};

struct StateMachine* create_state_machine(uint32_t max_data) {
    if (max_data == 0 || max_data > 1000000) {
        return NULL;
    }
    
    struct StateMachine* sm = malloc(sizeof(struct StateMachine));
    if (sm == NULL) {
        return NULL;
    }
    
    sm->current_state = STATE_IDLE;
    sm->data_count = 0;
    sm->max_data = max_data;
    sm->buffer_pos = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    
    return sm;
}

void destroy_state_machine(struct StateMachine* sm) {
    if (sm != NULL) {
        free(sm);
    }
}

enum State handle_event(struct StateMachine* sm, enum Event event) {
    if (sm == NULL || event >= EVENT_COUNT) {
        return sm != NULL ? sm->current_state : STATE_ERROR;
    }
    
    static const struct Transition transitions[] = {
        {STATE_IDLE, EVENT_START, STATE_READING},
        {STATE_READING, EVENT_DATA_READY, STATE_PROCESSING},
        {STATE_READING, EVENT_ERROR, STATE_ERROR},
        {STATE_PROCESSING, EVENT_PROCESS_COMPLETE, STATE_WRITING},
        {STATE_PROCESSING, EVENT_ERROR, STATE_ERROR},
        {STATE_WRITING, EVENT_WRITE_COMPLETE, STATE_READING},
        {STATE_WRITING, EVENT_ERROR, STATE_ERROR},
        {STATE_ERROR, EVENT_RESET, STATE_IDLE}
    };
    
    for (size_t i = 0; i < sizeof(transitions) / sizeof(transitions[0]); i++) {
        if (transitions[i].current == sm->current_state && 
            transitions[i].event == event) {
            sm->current_state = transitions[i].next;
            
            if (sm->current_state == STATE_IDLE) {
                sm->data_count = 0;
                sm->buffer_pos = 0;
                memset(sm->buffer, 0, sizeof(sm->buffer));
            }
            
            break;
        }
    }
    
    return sm->current_state;
}

int simulate_data_processing(struct StateMachine* sm) {
    if (sm == NULL) {
        return -1;
    }
    
    if (sm->current_state != STATE_READING) {
        return -1;
    }
    
    if (sm->data_count >= sm->max_data) {
        handle_event(sm, EVENT_RESET);
        return 0;
    }
    
    handle_event(sm, EVENT_DATA_READY);
    
    if (sm->current_state == STATE_PROCESSING) {
        if (sm->buffer_pos < sizeof(sm->buffer) - 1) {
            sm->buffer[sm->buffer_pos++] = 'A' + (sm->data_count % 26);
            sm->buffer[sm->buffer_pos] = '\0';
        }
        
        handle_event(sm, EVENT_PROCESS_COMPLETE);
    }
    
    if (sm->current_state == STATE_WRITING) {
        printf("Data %u: %s\n", sm->data_count + 1, sm->buffer);
        sm->data_count++;
        sm->buffer_pos = 0;
        memset(sm->buffer, 0, sizeof(sm->buffer));
        
        handle_event(sm, EVENT_WRITE_COMPLETE);
    }
    
    return 1;
}

int main(void) {
    struct StateMachine* sm = create_state_machine(10);
    if (sm == NULL) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    handle_event(sm, EVENT_START);
    
    printf("Starting data processing simulation...\n");
    
    for (int i = 0; i < 15; i++) {
        int result = simulate_data_processing(sm);
        if (result == 0) {
            printf("Processing complete. Resetting...\n");
            handle_event(sm, EVENT_RESET);
            handle_event(sm, EVENT_START);
        } else if (result < 0) {
            printf("Error in processing cycle %d\n", i + 1);
            break;
        }
        
        if (sm->current_state == STATE_ERROR) {
            printf("State machine entered error state. Resetting...\n");
            handle_event(sm, EVENT_RESET);
            handle_event(sm, EVENT_START);
        }
    }
    
    destroy_state_machine(sm);
    return 0;
}