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
    enum State from_state;
    enum Event event;
    enum State to_state;
    int (*action)(struct StateMachine*);
};

int action_idle_to_reading(struct StateMachine* sm) {
    if (sm == NULL) return 0;
    sm->data_count = 0;
    sm->buffer_pos = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    printf("Transition: IDLE -> READING\n");
    return 1;
}

int action_reading_to_processing(struct StateMachine* sm) {
    if (sm == NULL) return 0;
    if (sm->buffer_pos == 0) return 0;
    printf("Processing %u bytes of data\n", sm->buffer_pos);
    return 1;
}

int action_processing_to_writing(struct StateMachine* sm) {
    if (sm == NULL) return 0;
    printf("Writing processed data\n");
    return 1;
}

int action_writing_to_idle(struct StateMachine* sm) {
    if (sm == NULL) return 0;
    sm->data_count++;
    printf("Write complete. Total data processed: %u\n", sm->data_count);
    return 1;
}

int action_to_error(struct StateMachine* sm) {
    if (sm == NULL) return 0;
    printf("Error occurred in state machine\n");
    return 1;
}

int action_error_to_idle(struct StateMachine* sm) {
    if (sm == NULL) return 0;
    sm->data_count = 0;
    sm->buffer_pos = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    printf("Reset from error state\n");
    return 1;
}

struct Transition transitions[] = {
    {STATE_IDLE, EVENT_START, STATE_READING, action_idle_to_reading},
    {STATE_READING, EVENT_DATA_READY, STATE_PROCESSING, action_reading_to_processing},
    {STATE_PROCESSING, EVENT_PROCESS_COMPLETE, STATE_WRITING, action_processing_to_writing},
    {STATE_WRITING, EVENT_WRITE_COMPLETE, STATE_IDLE, action_writing_to_idle},
    {STATE_IDLE, EVENT_ERROR, STATE_ERROR, action_to_error},
    {STATE_READING, EVENT_ERROR, STATE_ERROR, action_to_error},
    {STATE_PROCESSING, EVENT_ERROR, STATE_ERROR, action_to_error},
    {STATE_WRITING, EVENT_ERROR, STATE_ERROR, action_to_error},
    {STATE_ERROR, EVENT_RESET, STATE_IDLE, action_error_to_idle}
};

const size_t num_transitions = sizeof(transitions) / sizeof(transitions[0]);

int process_event(struct StateMachine* sm, enum Event event) {
    if (sm == NULL) return 0;
    
    for (size_t i = 0; i < num_transitions; i++) {
        if (transitions[i].from_state == sm->current_state && 
            transitions[i].event == event) {
            if (transitions[i].action != NULL) {
                if (!transitions[i].action(sm)) {
                    return 0;
                }
            }
            sm->current_state = transitions[i].to_state;
            return 1;
        }
    }
    return 0;
}

int read_input_data(struct StateMachine* sm) {
    if (sm == NULL) return 0;
    if (sm->current_state != STATE_READING) return 0;
    
    printf("Enter data (max 255 chars): ");
    if (fgets(sm->buffer, sizeof(sm->buffer), stdin) == NULL) {
        return 0;
    }
    
    size_t len = strlen(sm->buffer);
    if (len > 0 && sm->buffer[len - 1] == '\n') {
        sm->buffer[len - 1] = '\0';
        len--;
    }
    
    if (len >= sizeof(sm->buffer)) {
        return 0;
    }
    
    sm->buffer_pos = len;
    return 1;
}

void print_state_info(struct StateMachine* sm) {
    if (sm == NULL) return;
    
    const char* state_names[] = {
        "IDLE", "READING", "PROCESSING", "WRITING", "ERROR"
    };
    
    if (sm->current_state < STATE_COUNT) {
        printf("Current state: %s\n", state_names[sm->current_state]);
    } else {
        printf("Current state: UNKNOWN\n");
    }
    printf("Data processed: %u/%u\n", sm->data_count, sm->max_data);
}

int main(void) {
    struct StateMachine sm;
    sm.current_state = STATE_IDLE;
    sm.data_count = 0;
    sm.max_data = 5;
    sm.buffer_pos =